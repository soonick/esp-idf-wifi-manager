#include "assets.hpp"

extern const char config_page_start[] = "config_page";
extern const char save_page_start[] = "save_page";

const char* get_config_page_start(const std::string& lang) {
  (void)lang;
  return config_page_start;
}
const char* get_config_page_end(const std::string& lang) {
  (void)lang;
  return config_page_start + 11;
}
const char* get_save_page_start(const std::string& lang) {
  (void)lang;
  return save_page_start;
}
const char* get_save_page_end(const std::string& lang) {
  (void)lang;
  return save_page_start + 9;
}
