#define CATCH_CONFIG_MAIN
#include <catch.hpp>

// Mocks
#include "nvs_handle.hpp"

// Files under test
#include "constants.hpp"
#include "esp_idf_wifi_manager.hpp"

TEST_CASE("EspIdfWifiManager") {
  SECTION("SSID too long") {
    CHECK_THROWS(EspIdfWifiManager::get_instance(
        "12345678901234567890123456789012", "12345678"));
  }

  SECTION("Password too long") {
    CHECK_THROWS(EspIdfWifiManager::get_instance(
        "12345678", "12345678901234567890123456789012"));
  }

  SECTION("Password too short") {
    CHECK_THROWS(EspIdfWifiManager::get_instance("12345678", "1234567"));
  }
}

TEST_CASE("get_config") {
  SECTION("Gets directly from NVS") {
    esp_err_t err;
    std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(
        EspIdfWifiManagerConstants::NVS_NAMESPACE.c_str(), NVS_READWRITE, &err);
    handle->set_string(EspIdfWifiManagerConstants::SSID_KEY.c_str(),
                       "some_ssid");
    handle->set_string(EspIdfWifiManagerConstants::PASSWORD_KEY.c_str(),
                       "some_password");
    handle->set_string(EspIdfWifiManagerConstants::DEVICE_ID_KEY.c_str(),
                       "some_id");

    EspIdfWifiManager* wm =
        EspIdfWifiManager::get_instance("12345678", "12345678");
    std::optional<wm_config> config_opt =
        wm->get_config([](wm_config c) { (void)c; });
    REQUIRE(config_opt.has_value() == true);
    REQUIRE(config_opt.value().ssid == "some_ssid");
    REQUIRE(config_opt.value().password == "some_password");
    REQUIRE(config_opt.value().id == "some_id");
  }

  SECTION("Returns empty optional if NVS is empty") {
    EspIdfWifiManager* wm =
        EspIdfWifiManager::get_instance("12345678", "12345678");
    wm->clear_config();
    std::optional<wm_config> config_opt =
        wm->get_config([](wm_config c) { (void)c; });
    REQUIRE(config_opt.has_value() == false);
  }
}
