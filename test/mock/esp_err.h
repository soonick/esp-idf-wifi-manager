#pragma once

#define ESP_OK 0
#define ESP_ERR 1

typedef int esp_err_t;

// We throw in the mock, so we can catch in the tests
#define ESP_ERROR_CHECK(x) \
  if (x != ESP_OK) {       \
    throw 1;               \
  }
