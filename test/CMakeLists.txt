cmake_minimum_required(VERSION 3.27.4)
project(EspIdfWifiManagerTest
  VERSION 0.1.0
  LANGUAGES CXX)

include_directories(../include)
include_directories(external/catch2)
include_directories(mock)

FILE(GLOB MOCK_SRCS mock/*.cpp
                    mock/lwip/*.cpp
                    mock/freertos/*.cpp)

set(TESTING_SRCS
  ../src/esp_idf_wifi_manager.cpp
)

set(TEST_TARGET_SRCS
  src/esp_idf_wifi_manager_test.cpp
)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -std=c++20)

add_executable(
  test
  ${MOCK_SRCS}
  ${TESTING_SRCS}
  ${TEST_TARGET_SRCS}
)
