#pragma once

#include <memory>
#include <unordered_map>

#include "esp_err.h"
#include "nvs_flash.h"

namespace nvs {

class NVSHandle {
 public:
  esp_err_t set_string(const char* key, const char* value);
  esp_err_t commit();
  esp_err_t get_string(const char* key, char* out_str, size_t len);
  esp_err_t erase_all();
};

std::unique_ptr<NVSHandle> open_nvs_handle(const char* ns_name,
                                           nvs_open_mode_t open_mode,
                                           esp_err_t* err = nullptr);

}  // namespace nvs
