#include "assets.hpp"

extern const char config_page_end[] asm("_binary_config_page_en_html_end");
extern const char config_page_end_es[] asm("_binary_config_page_es_html_end");
extern const char config_page_start[] asm("_binary_config_page_en_html_start");
extern const char config_page_start_es[] asm(
    "_binary_config_page_es_html_start");
extern const char save_page_end[] asm("_binary_save_success_page_en_html_end");
extern const char save_page_end_es[] asm(
    "_binary_save_success_page_es_html_end");
extern const char save_page_start[] asm(
    "_binary_save_success_page_en_html_start");
extern const char save_page_start_es[] asm(
    "_binary_save_success_page_es_html_start");

const char* get_config_page_start(const std::string& lang) {
  if (lang == "es") {
    return config_page_start_es;
  } else {
    return config_page_start;
  }
}
const char* get_config_page_end(const std::string& lang) {
  if (lang == "es") {
    return config_page_end_es;
  } else {
    return config_page_end;
  }
}
const char* get_save_page_start(const std::string& lang) {
  if (lang == "es") {
    return save_page_start_es;
  } else {
    return save_page_start;
  }
}
const char* get_save_page_end(const std::string& lang) {
  if (lang == "es") {
    return save_page_end_es;
  } else {
    return save_page_end;
  }
}
