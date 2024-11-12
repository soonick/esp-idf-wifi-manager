/**
 * Shows how the clear_config function can be used to delete information from
 * NVS and how the AP can be restarted so a new configuration can be saved
 *
 * This example requires a button on pin GPIO 18 that will send go to gnd when
 * pressed. If held for 5 seconds it will clear NVS data and a new AP will be
 * started
 */

// Standard library
#include <sys/types.h>
#include <cassert>
#include <cstdint>
#include <optional>

// esp-idf
#include <driver/gpio.h>
#include <esp_log.h>

#include "esp_idf_wifi_manager.hpp"

#define RESET_BUTTON GPIO_NUM_18

const char* TAG = "clear-config";
const uint64_t long_press_thresshold_ms = 5000;
EspIdfWifiManager* wm = nullptr;
uint64_t pressed_since = 0;
bool ap_up = false;

void config_callback(wm_config in) {
  ESP_LOGI(TAG, "Config was set. SSID: %s, Password: %s, ID: %s",
           in.ssid.c_str(), in.password.c_str(), in.id.c_str());
  ap_up = false;
  wm->shutdown_ap();
}

void reset(void* pvParameters) {
  (void)pvParameters;

  gpio_reset_pin(RESET_BUTTON);
  gpio_set_direction(RESET_BUTTON, GPIO_MODE_INPUT);
  gpio_set_pull_mode(RESET_BUTTON, GPIO_PULLUP_ONLY);

  while (true) {
    bool is_pressed = !gpio_get_level(RESET_BUTTON);
    bool is_long_pressed = false;
    if (is_pressed) {
      uint64_t current_millis = esp_timer_get_time() / 1000;
      if (pressed_since == 0) {
        pressed_since = current_millis;
      }
      is_long_pressed =
          (current_millis - pressed_since) > long_press_thresshold_ms;
    } else {
      pressed_since = 0;
    }

    if (!ap_up && is_long_pressed) {
      ESP_LOGI(TAG, "Is long pressed");
      wm->clear_config();
      ap_up = true;
      std::optional<wm_config> conf = wm->get_config(config_callback);
    }
  }
}

extern "C" void app_main() {
  wm = EspIdfWifiManager::get_instance("my-esp-idf-ap", "abcd1234");

  // Make sure the AP will be started
  wm->clear_config();

  ap_up = true;
  std::optional<wm_config> conf = wm->get_config(config_callback);

  // There shouldn't be any value because we called wm->clear_config();
  assert(conf.has_value() == false);

  xTaskCreate(reset, "reset", 4096, nullptr, tskIDLE_PRIORITY, NULL);
}
