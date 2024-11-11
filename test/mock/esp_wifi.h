#pragma once

#include "esp_err.h"
#include "esp_netif.h"

#define WIFI_INIT_CONFIG_DEFAULT() \
  {}

#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_WPA2_PSK

typedef enum {
  WIFI_AUTH_OPEN = 0,
  WIFI_AUTH_WPA_WPA2_PSK,
} wifi_auth_mode_t;

typedef enum {
  WIFI_MODE_NULL = 0,
  WIFI_MODE_STA,
  WIFI_MODE_AP,
} wifi_mode_t;

typedef enum {
  ESP_IF_WIFI_STA = 0,
  ESP_IF_WIFI_AP,
} esp_interface_t;

typedef enum {
  WIFI_IF_STA = ESP_IF_WIFI_STA,
  WIFI_IF_AP = ESP_IF_WIFI_AP,
} wifi_interface_t;

typedef struct {
} wifi_init_config_t;

typedef struct {
  uint8_t ssid[32];
  uint8_t password[64];
  uint8_t ssid_len;
  uint8_t channel;
  wifi_auth_mode_t authmode;
  uint8_t ssid_hidden;
  uint8_t max_connection;
} wifi_ap_config_t;

typedef union {
  wifi_ap_config_t ap;
} wifi_config_t;

esp_netif_t* esp_netif_create_default_wifi_ap();
void esp_netif_destroy_default_wifi(esp_netif_t* ap_wifi);
esp_err_t esp_wifi_init(const wifi_init_config_t* config);
void esp_wifi_deinit();
esp_err_t esp_wifi_set_mode(wifi_mode_t mode);
esp_err_t esp_wifi_set_config(wifi_interface_t interface, wifi_config_t* conf);
esp_err_t esp_wifi_start();
void esp_wifi_stop();
