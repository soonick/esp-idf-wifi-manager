#include "nvs_handle.hpp"

#include <cstring>
#include <memory>
#include <string>
#include <unordered_set>
#include "esp_err.h"
#include "nvs_flash.h"

std::unordered_map<std::string, std::string> NVS_STORAGE =
    std::unordered_map<std::string, std::string>();

namespace nvs {

esp_err_t NVSHandle::set_string(const char* key, const char* value) {
  NVS_STORAGE[std::string(key)] = std::string(value);
  return ESP_OK;
}

esp_err_t NVSHandle::commit() {
  return ESP_OK;
}

esp_err_t NVSHandle::get_string(const char* key, char* out_str, size_t len) {
  const std::string val = NVS_STORAGE[std::string(key)];

  if (val.size() > (len - 1)) {
    return ESP_ERR;
  }

  strcpy(out_str, val.c_str());
  return ESP_OK;
}

esp_err_t NVSHandle::erase_all() {
  NVS_STORAGE = std::unordered_map<std::string, std::string>();
  return ESP_OK;
}

std::unique_ptr<NVSHandle> open_nvs_handle(const char* ns_name,
                                           nvs_open_mode_t open_mode,
                                           esp_err_t* err) {
  (void)ns_name;
  (void)open_mode;

  *err = ESP_OK;
  NVSHandle handle = NVSHandle();
  return std::make_unique<NVSHandle>(handle);
}

}  // namespace nvs
