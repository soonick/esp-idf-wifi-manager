#pragma once

#include <stdint.h>

#include "esp_err.h"

struct esp_netif_obj;

struct esp_ip4_addr {
  uint32_t addr; /*!< IPv4 address */
};

typedef struct esp_ip4_addr esp_ip4_addr_t;
typedef struct esp_netif_obj esp_netif_t;

typedef struct {
  esp_ip4_addr_t ip;
  esp_ip4_addr_t netmask;
  esp_ip4_addr_t gw;
} esp_netif_ip_info_t;

esp_err_t esp_netif_get_ip_info(esp_netif_t* esp_netif,
                                esp_netif_ip_info_t* ip_info);
esp_err_t esp_netif_init();
esp_netif_t* esp_netif_get_handle_from_ifkey(const char* if_key);
