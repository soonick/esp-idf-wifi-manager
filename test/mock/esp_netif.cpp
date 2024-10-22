#include "esp_netif.h"

esp_err_t esp_netif_init() {
  return ESP_OK;
}

esp_netif_t* esp_netif_get_handle_from_ifkey(const char* if_key) {
  (void)if_key;

  return {};
}

esp_err_t esp_netif_get_ip_info(esp_netif_t* esp_netif,
                                esp_netif_ip_info_t* ip_info) {
  (void)esp_netif;
  (void)ip_info;

  return ESP_OK;
}
