#pragma once

#include <stdint.h>

#define configSTACK_DEPTH_TYPE uint32_t

typedef void* TaskHandle_t;
typedef long BaseType_t;
typedef unsigned int UBaseType_t;
typedef void (*TaskFunction_t)(void*);

BaseType_t xTaskCreate(TaskFunction_t pxTaskCode,
                       const char* const pcName,
                       const configSTACK_DEPTH_TYPE uxStackDepth,
                       void* const pvParameters,
                       UBaseType_t uxPriority,
                       TaskHandle_t* const pxCreatedTask);
