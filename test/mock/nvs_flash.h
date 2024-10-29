#pragma once

#include "esp_err.h"

#define ESP_ERR_NVS_BASE 0x1100
#define ESP_ERR_NVS_NO_FREE_PAGES (ESP_ERR_NVS_BASE + 0x0d)
#define ESP_ERR_NVS_NEW_VERSION_FOUND (ESP_ERR_NVS_BASE + 0x10)

typedef enum { NVS_READONLY, NVS_READWRITE } nvs_open_mode_t;

esp_err_t nvs_flash_init();
esp_err_t nvs_flash_erase();
