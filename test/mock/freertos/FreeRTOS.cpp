#include "FreeRTOS.h"

BaseType_t xTaskCreate(TaskFunction_t pxTaskCode,
                       const char* const pcName,
                       const configSTACK_DEPTH_TYPE uxStackDepth,
                       void* const pvParameters,
                       UBaseType_t uxPriority,
                       TaskHandle_t* const pxCreatedTask) {
  (void)pxTaskCode;
  (void)pcName;
  (void)uxStackDepth;
  (void)pvParameters;
  (void)uxPriority;
  (void)pxCreatedTask;

  return 1;
}
