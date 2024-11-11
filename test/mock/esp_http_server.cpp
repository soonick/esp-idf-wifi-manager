#include "esp_http_server.h"

esp_err_t httpd_resp_set_type(httpd_req_t* r, const char* type) {
  (void)r;
  (void)type;

  return ESP_OK;
}

esp_err_t httpd_resp_send(httpd_req_t* r, const char* buf, ssize_t buf_len) {
  (void)r;
  (void)buf;
  (void)buf_len;

  return ESP_OK;
}

size_t httpd_req_get_url_query_len(httpd_req_t* r) {
  (void)r;

  return 0;
}

esp_err_t httpd_req_get_url_query_str(httpd_req_t* r,
                                      char* buf,
                                      size_t buf_len) {
  (void)r;
  (void)buf;
  (void)buf_len;

  return ESP_OK;
}

esp_err_t httpd_resp_set_status(httpd_req_t* r, const char* status) {
  (void)r;
  (void)status;

  return ESP_OK;
}

esp_err_t httpd_resp_set_hdr(httpd_req_t* r,
                             const char* field,
                             const char* value) {
  (void)r;
  (void)field;
  (void)value;

  return ESP_OK;
}

esp_err_t httpd_start(httpd_handle_t* handle, const httpd_config_t* config) {
  (void)handle;
  (void)config;

  return ESP_OK;
}

void httpd_stop(httpd_handle_t handle) {
  (void)handle;
}

esp_err_t httpd_register_uri_handler(httpd_handle_t handle,
                                     const httpd_uri_t* uri_handler) {
  (void)handle;
  (void)uri_handler;

  return ESP_OK;
}

esp_err_t httpd_register_err_handler(httpd_handle_t handle,
                                     httpd_err_code_t error,
                                     httpd_err_handler_func_t handler_fn) {
  (void)handle;
  (void)error;
  (void)handler_fn;

  return ESP_OK;
}
