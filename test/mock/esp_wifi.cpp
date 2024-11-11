#include "esp_wifi.h"

esp_netif_t* esp_netif_create_default_wifi_ap() {
  return {};
}

void esp_netif_destroy_default_wifi(esp_netif_t* ap_wifi) {
  (void)ap_wifi;
}

esp_err_t esp_wifi_init(const wifi_init_config_t* config) {
  (void)config;

  return ESP_OK;
}

void esp_wifi_deinit() {}

esp_err_t esp_wifi_start() {
  return ESP_OK;
}

esp_err_t esp_wifi_set_config(wifi_interface_t interface, wifi_config_t* conf) {
  (void)interface;
  (void)conf;

  return ESP_OK;
}

esp_err_t esp_wifi_set_mode(wifi_mode_t mode) {
  (void)mode;

  return ESP_OK;
}

void esp_wifi_stop() {}
