#pragma once

#include <freertos/FreeRTOS.h>
#include <stdint.h>
#include <string>

struct dns_answer {
  uint16_t ptr_offset;
  uint16_t type;
  uint16_t klass;
  uint32_t ttl;
  uint16_t addr_len;
  uint32_t ip_addr;
} __attribute__((__packed__));

struct wm_config {
  std::string ssid;
  std::string password;
  std::string id;
};
