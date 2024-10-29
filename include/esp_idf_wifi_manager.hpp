#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "esp_err.h"
#include "esp_http_server.h"
#include "http_parser.h"

#include "types.hpp"

class EspIdfWifiManager {
 public:
  static const char* TAG;
  static void (*get_config_callback)(wm_config);

  /**
   * @param - ap_ssid - The SSID that will be used for the created AP
   * @param - ap_password - The password for the created AP
   */
  EspIdfWifiManager(const std::string ap_ssid, const std::string ap_password);

  /**
   * If a config already exists in NVS, it will immediately return that config.
   * Otherwise, it will return empty and will start the configuration portal.
   * There can only be one callback at a time and it will only be called once.
   * If get_config is called again, the old callback will be ovewritten.
   *
   * @param set_config - Once a new configuration is set via the AP, this
   *                     function will be called with the new configuration as
   *                     argument
   */
  std::optional<wm_config> get_config(void set_config(wm_config));

  /**
   * Clears config from NVS
   */
  void clear_config();

 private:
  const std::string ap_ssid;
  const std::string ap_password;
  const httpd_uri_t config_page = {.uri = "/",
                                   .method = HTTP_GET,
                                   .handler = config_page_handler,
                                   .user_ctx = NULL};
  const httpd_uri_t save_page = {.uri = "/save",
                                 .method = HTTP_GET,
                                 .handler = save_page_handler,
                                 .user_ctx = NULL};

  wm_config config;

  static esp_err_t http_404_error_handler(httpd_req_t* req,
                                          httpd_err_code_t err);
  static esp_err_t config_page_handler(httpd_req_t* req);
  static esp_err_t save_page_handler(httpd_req_t* req);
  static void dns_server_task(void* pvParameters);

  /**
   * Replace all ocurrences of a character in a string
   *
   * @param in - The String where the replacement will take place
   * @param o - The character that will be replaced
   * @param n - The new character
   */
  static void replace(std::string& in, const char o, const char n);

  /**
   * Given the URL path. It returns then wm_config from the query parameters.
   *
   * @param path - e.g. /save?ssid=My+network&password=asdf1234&id=device-5564
   *
   * @return If the query params could be parsed to a wm_config, it returns is.
   *         Returns an empty optional otherwise
   */
  static std::optional<wm_config> config_from_path(const std::string& path);

  /**
   * Verifies that the map contains all the necessary keys and they are valid
   */
  static bool is_config_valid(const wm_config& config);

  /**
   * Saves configuration to NVS
   * @param config - Configuration to save
   */
  static void save_to_nvs(const wm_config& config);

  /**
   * Load configs from NVS into the `config`, private variable
   * @return true if load was succesful, false otherwise
   */
  bool load_config();

  void init_ap();
  void start_web_server();
  void start_dns_server();
  void string_to_uint8_array(const std::string& str, uint8_t* arr);
};
