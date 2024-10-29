#include "esp_idf_wifi_manager.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "esp_err.h"
#include "esp_event.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "lwip/sockets.h"
#include "nvs_flash.h"
#include "nvs_handle.hpp"

#include "assets.hpp"
#include "constants.hpp"
#include "my_abort.hpp"
#include "types.hpp"

const char* EspIdfWifiManager::TAG = "wifi_manager";
void (*EspIdfWifiManager::get_config_callback)(wm_config) = nullptr;

EspIdfWifiManager::EspIdfWifiManager(const std::string ap_ssid,
                                     const std::string ap_password)
    : ap_ssid{ap_ssid}, ap_password{ap_password} {
  if (ap_ssid.size() > EspIdfWifiManagerConstants::MAX_SSID_SIZE ||
      ap_password.size() > EspIdfWifiManagerConstants::MAX_PASSWORD_SIZE) {
    ESP_LOGE(TAG, "ssid and password must be at most 31 characters");
    my_abort();
  }

  if (ap_password.size() < EspIdfWifiManagerConstants::MIN_PASSWORD_SIZE) {
    ESP_LOGE(TAG, "Password must be at least 8 characters");
    my_abort();
  }

  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  load_config();

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  esp_netif_create_default_wifi_ap();
}

std::optional<wm_config> EspIdfWifiManager::get_config(
    void set_config(wm_config)) {
  if (!config.ssid.empty() && !config.password.empty() && !config.id.empty()) {
    return config;
  }

  init_ap();
  start_web_server();
  start_dns_server();

  get_config_callback = set_config;
  return std::nullopt;
}

void EspIdfWifiManager::clear_config() {
  esp_err_t err;
  std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(
      EspIdfWifiManagerConstants::NVS_NAMESPACE.c_str(), NVS_READWRITE, &err);
  ESP_ERROR_CHECK(err);

  err = handle->erase_all();
  ESP_ERROR_CHECK(err);
}

void EspIdfWifiManager::string_to_uint8_array(const std::string& str,
                                              uint8_t* arr) {
  memset(arr, 0, str.size() + 1);
  memcpy(arr, str.c_str(), str.size());
}

void EspIdfWifiManager::init_ap() {
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

  wifi_ap_config_t ap_config = {};
  string_to_uint8_array(ap_ssid, ap_config.ssid);
  string_to_uint8_array(ap_password, ap_config.password);
  ap_config.authmode = WIFI_AUTH_WPA_WPA2_PSK;
  ap_config.max_connection = 4;

  wifi_config_t wifi_config = {.ap = ap_config};

  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  esp_netif_ip_info_t ip_info;
  esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_AP_DEF"),
                        &ip_info);

  char ip_addr[16];
  inet_ntoa_r(ip_info.ip.addr, ip_addr, 16);
  ESP_LOGD(TAG, "Set up softAP with IP: %s", ip_addr);
}

esp_err_t EspIdfWifiManager::config_page_handler(httpd_req_t* req) {
  const uint32_t config_page_len =
      get_config_page_end() - get_config_page_start();

  ESP_LOGD(TAG, "Serve config page");
  httpd_resp_set_type(req, "text/html");
  httpd_resp_send(req, get_config_page_start(), config_page_len);

  return ESP_OK;
}

esp_err_t EspIdfWifiManager::save_page_handler(httpd_req_t* req) {
  ESP_LOGI(TAG, "Save page handler");

  size_t buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    char* buf = (char*)malloc(buf_len);
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      const std::string query_str(buf);
      std::optional<wm_config> config_opt = config_from_path(query_str);

      httpd_resp_set_type(req, "text/html");
      if (config_opt.has_value()) {
        save_to_nvs(config_opt.value());

        // TODO: Call callback
        if (get_config_callback != nullptr) {
          get_config_callback(config_opt.value());
          get_config_callback = nullptr;
        }

        ESP_LOGD(TAG, "Serve save success page");
        const uint32_t save_page_len =
            get_save_page_end() - get_save_page_start();
        httpd_resp_send(req, get_save_page_start(), save_page_len);
      } else {
        ESP_LOGD(TAG, "Serve save success page");
        const uint32_t config_page_len =
            get_config_page_end() - get_config_page_start();
        httpd_resp_send(req, get_config_page_start(), config_page_len);
      }
    }
    free(buf);
  }

  return ESP_OK;
}

esp_err_t EspIdfWifiManager::http_404_error_handler(httpd_req_t* req,
                                                    httpd_err_code_t err) {
  (void)err;  // To silence argument not used warning
  httpd_resp_set_status(req, "302 Temporary Redirect");
  httpd_resp_set_hdr(req, "Location", "/");

  // iOS requires content in the response to detect a captive portal, simply
  // redirecting is not sufficient.
  httpd_resp_send(req, "Redirect to the captive portal", HTTPD_RESP_USE_STRLEN);

  ESP_LOGD(TAG, "Redirecting to confg page");
  return ESP_OK;
}

void EspIdfWifiManager::start_web_server() {
  httpd_handle_t server = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  ESP_LOGD(TAG, "Starting server on port: '%d'", config.server_port);
  ESP_ERROR_CHECK(httpd_start(&server, &config));

  // Set URI handlers
  httpd_register_uri_handler(server, &config_page);
  httpd_register_uri_handler(server, &save_page);
  httpd_register_err_handler(server, HTTPD_404_NOT_FOUND,
                             EspIdfWifiManager::http_404_error_handler);
}

void EspIdfWifiManager::dns_server_task(void* pvParameters) {
  (void)pvParameters;
  char rx_buffer[128];

  while (true) {
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(53);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
      ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
      break;
    }

    int err = bind(sock, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
      ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
    }

    while (true) {
      ESP_LOGD(TAG, "Waiting for data");
      struct sockaddr_in source_addr;
      socklen_t addrlen = sizeof(source_addr);
      int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0,
                         (struct sockaddr*)&source_addr, &addrlen);

      // Error occurred during receiving
      if (len < 0) {
        ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
        close(sock);
        break;
      } else {
        char dns_response[256] = {};
        memcpy(dns_response, rx_buffer, len);
        dns_response[2] |= (1 << 7);
        dns_response[6] = dns_response[4];
        dns_response[7] = dns_response[5];

        int dns_response_len = sizeof(struct dns_answer) + len;

        // Move to the start of the questions section. Since the DNS header is
        // 12 bytes, we just need to move 12 bytes from the start of the
        // response
        char* qn_ptr = dns_response + 12;

        // Move to the end of the request
        char* ans_ptr = dns_response + len;

        // Cast the memory to our dns_aswer type
        struct dns_answer* answer = (struct dns_answer*)ans_ptr;

        // 0x0c is the same as 1100_0000. We use | to make sure set those bits
        // on the pointer. To convert to network order, call htons
        answer->ptr_offset = htons(0xc000 | (qn_ptr - dns_response));

        char* qn_type_ptr = qn_ptr;
        while (qn_type_ptr[0] != 0x0) {
          qn_type_ptr++;
        }
        qn_type_ptr++;
        answer->type = *(uint16_t*)qn_type_ptr;
        qn_type_ptr += 2;
        answer->klass = *(uint16_t*)qn_type_ptr;

        answer->ttl = htonl(300);

        // We are using IPv4 so we know it'll always be 4 bytes
        answer->addr_len = htons(4);

        // Get the IP information from default AP device
        esp_netif_ip_info_t ip_info;
        esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_AP_DEF"),
                              &ip_info);
        answer->ip_addr = ip_info.ip.addr;

        if (dns_response_len <= 0) {
          ESP_LOGE(TAG, "Failed to prepare a DNS dns_response");
        } else {
          int err = sendto(sock, dns_response, dns_response_len, 0,
                           (struct sockaddr*)&source_addr, sizeof(source_addr));
          if (err < 0) {
            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
            break;
          }
        }
      }
    }

    if (sock != -1) {
      ESP_LOGD(TAG, "Shutting down socket");
      shutdown(sock, 0);
      close(sock);
    }
  }
}

void EspIdfWifiManager::start_dns_server() {
  struct dns_server_handle* handle =
      (dns_server_handle*)calloc(1, sizeof(struct dns_server_handle));
  xTaskCreate(dns_server_task, "dns_server", 4096, handle, 5, &handle->task);
}

void EspIdfWifiManager::replace(std::string& in, const char o, const char n) {
  for (long unsigned int i = 0; i < in.length(); i++) {
    if (in[i] == o) {
      in[i] = n;
    }
  }
}

std::optional<wm_config> EspIdfWifiManager::config_from_path(
    const std::string& path) {
  wm_config config;

  int start = path.find_first_of("?") + 1;
  int end = path.find_first_of(" ", start);
  std::string query_string = path.substr(start, end);
  replace(query_string, '+', ' ');

  long unsigned int current_start = 0;
  while (current_start < query_string.length()) {
    int current_end = query_string.find_first_of("&", current_start);
    if (current_end == -1) {
      current_end = query_string.length();
    }

    std::string current_pair =
        query_string.substr(current_start, current_end - current_start);
    int equalPos = current_pair.find_first_of("=");
    if (equalPos != -1) {
      // If there is no equal sign, we skip adding it to the result
      std::string key = current_pair.substr(0, equalPos);
      std::string value =
          current_pair.substr(equalPos + 1, current_pair.length());

      if (key == EspIdfWifiManagerConstants::SSID_KEY) {
        config.ssid = value;
      }
      if (key == EspIdfWifiManagerConstants::PASSWORD_KEY) {
        config.password = value;
      }
      if (key == EspIdfWifiManagerConstants::DEVICE_ID_KEY) {
        config.id = value;
      }
    }

    current_start = current_end + 1;
  }

  if (is_config_valid(config)) {
    return config;
  }

  return std::nullopt;
}

bool EspIdfWifiManager::is_config_valid(const wm_config& config) {
  return config.ssid.size() > 0 && config.ssid.size() <= 32 &&
         config.password.size() >= 8 && config.password.size() <= 32 &&
         config.id.size() <= 50;
}

void EspIdfWifiManager::save_to_nvs(const wm_config& config) {
  esp_err_t err;
  std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(
      EspIdfWifiManagerConstants::NVS_NAMESPACE.c_str(), NVS_READWRITE, &err);
  ESP_ERROR_CHECK(err);

  err = handle->set_string(EspIdfWifiManagerConstants::SSID_KEY.c_str(),
                           config.ssid.c_str());
  ESP_ERROR_CHECK(err);
  err = handle->set_string(EspIdfWifiManagerConstants::PASSWORD_KEY.c_str(),
                           config.password.c_str());
  ESP_ERROR_CHECK(err);
  err = handle->set_string(EspIdfWifiManagerConstants::DEVICE_ID_KEY.c_str(),
                           config.id.c_str());
  ESP_ERROR_CHECK(err);

  err = handle->commit();
  ESP_ERROR_CHECK(err);
}

bool EspIdfWifiManager::load_config() {
  wm_config loaded;

  std::unique_ptr<char[]> ssid = std::make_unique<char[]>(100);
  std::unique_ptr<char[]> pass = std::make_unique<char[]>(100);
  std::unique_ptr<char[]> id = std::make_unique<char[]>(100);

  esp_err_t err;
  std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(
      EspIdfWifiManagerConstants::NVS_NAMESPACE.c_str(), NVS_READONLY, &err);
  ESP_ERROR_CHECK(err);

  err = handle->get_string(EspIdfWifiManagerConstants::SSID_KEY.c_str(),
                           ssid.get(), 100);
  switch (err) {
    case ESP_OK:
      loaded.ssid = std::string(ssid.get());
      break;
    default:
      return false;
  }

  err = handle->get_string(EspIdfWifiManagerConstants::PASSWORD_KEY.c_str(),
                           pass.get(), 100);
  switch (err) {
    case ESP_OK:
      loaded.password = std::string(pass.get());
      break;
    default:
      return false;
  }

  err = handle->get_string(EspIdfWifiManagerConstants::DEVICE_ID_KEY.c_str(),
                           id.get(), 100);
  switch (err) {
    case ESP_OK:
      loaded.id = std::string(id.get());
      break;
    default:
      return false;
  }

  config = loaded;
  return true;
}
