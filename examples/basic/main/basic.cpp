#include <cassert>
#include <optional>
#include "esp_idf_wifi_manager.hpp"
#include "esp_log.h"

const char* TAG = "basic";

extern "C" void app_main() {
  EspIdfWifiManager* wm =
      EspIdfWifiManager::get_instance("my-esp-idf-ap", "abcd1234");

  // Make sure the AP will be started
  wm->clear_config();

  std::optional<wm_config> conf = wm->get_config([](wm_config in) {
    ESP_LOGI(TAG, "Config was set. SSID: %s, Password: %s, ID: %s",
             in.ssid.c_str(), in.password.c_str(), in.id.c_str());
  });

  // There shouldn't be any value because we called wm->clear_config();
  assert(conf.has_value() == false);
}
