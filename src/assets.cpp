#include "assets.hpp"

extern const char config_page_start[] asm("_binary_config_page_html_start");
extern const char config_page_end[] asm("_binary_config_page_html_end");
extern const char save_page_start[] asm("_binary_save_success_page_html_start");
extern const char save_page_end[] asm("_binary_save_success_page_html_end");

const char* get_config_page_start() {
  return config_page_start;
}
const char* get_config_page_end() {
  return config_page_end;
}
const char* get_save_page_start() {
  return save_page_start;
}
const char* get_save_page_end() {
  return save_page_end;
}
