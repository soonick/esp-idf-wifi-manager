#define ESP_LOGD(tag, format, ...)           \
  printf(format __VA_OPT__(, ) __VA_ARGS__); \
  printf("\n");
#define ESP_LOGE(tag, format, ...)           \
  printf(format __VA_OPT__(, ) __VA_ARGS__); \
  printf("\n");
#define ESP_LOGI(tag, format, ...)           \
  printf(format __VA_OPT__(, ) __VA_ARGS__); \
  printf("\n");
