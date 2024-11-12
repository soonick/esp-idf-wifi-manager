#include "esp_timer.h"

esp_err_t esp_timer_create(const esp_timer_create_args_t* create_args,
                           esp_timer_handle_t* out_handle) {
  (void)create_args;
  (void)out_handle;

  return ESP_OK;
}

esp_err_t esp_timer_start_once(esp_timer_handle_t timer, uint64_t timeout_us) {
  (void)timer;
  (void)timeout_us;

  return ESP_OK;
}
