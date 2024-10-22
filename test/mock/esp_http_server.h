#pragma once

#include <stdint.h>
#include <unistd.h>

#include "esp_err.h"
#include "http_parser.h"

#define HTTPD_RESP_USE_STRLEN -1

#define HTTPD_DEFAULT_CONFIG() \
  {}

typedef enum {
  HTTPD_500_INTERNAL_SERVER_ERROR = 0,
  HTTPD_404_NOT_FOUND,
} httpd_err_code_t;

typedef struct httpd_req {
} httpd_req_t;

typedef struct httpd_uri {
  const char* uri;                      /*!< URI of the handler */
  httpd_method_t method;                /*!< Method supported by the handler */
  esp_err_t (*handler)(httpd_req_t* r); /*!< The handler function */
  void* user_ctx;                       /*!< User context pointer */
} httpd_uri_t;

typedef struct httpd_config {
  uint16_t server_port;
} httpd_config_t;

typedef void* httpd_handle_t;
typedef esp_err_t (*httpd_err_handler_func_t)(httpd_req_t* req,
                                              httpd_err_code_t error);

esp_err_t httpd_req_get_url_query_str(httpd_req_t* r,
                                      char* buf,
                                      size_t buf_len);
esp_err_t httpd_resp_send(httpd_req_t* r, const char* buf, ssize_t buf_len);
esp_err_t httpd_resp_set_hdr(httpd_req_t* r,
                             const char* field,
                             const char* value);
esp_err_t httpd_resp_set_status(httpd_req_t* r, const char* status);
esp_err_t httpd_resp_set_type(httpd_req_t* r, const char* type);
size_t httpd_req_get_url_query_len(httpd_req_t* r);
esp_err_t httpd_register_uri_handler(httpd_handle_t handle,
                                     const httpd_uri_t* uri_handler);
esp_err_t httpd_register_err_handler(httpd_handle_t handle,
                                     httpd_err_code_t error,
                                     httpd_err_handler_func_t handler_fn);
esp_err_t httpd_start(httpd_handle_t* handle, const httpd_config_t* config);
