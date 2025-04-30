idf_component_register(SRCS "src/assets.cpp"
                            "src/esp_idf_wifi_manager.cpp"
                            "src/my_abort.cpp"
                       INCLUDE_DIRS "include"
                       EMBED_FILES assets/en/config_page_en.html
                                   assets/en/save_success_page_en.html
                                   assets/es/config_page_es.html
                                   assets/es/save_success_page_es.html
                       REQUIRES "esp_http_server"
                                "esp_netif"
                                "esp_timer"
                                "http_parser"
                       PRIV_REQUIRES "esp_event"
                                     "esp_wifi"
                                     "log"
                                     "lwip"
                                     "nvs_flash")

set(MY_FLAGS
    "-Wall"
    "-Wextra"
    "-Werror"
    "-Werror=unused-function"
    "-Werror=unused-variable"
    "-Werror=unused-but-set-variable"
    "-Werror=deprecated-declarations"
)

target_compile_options(${COMPONENT_LIB} PRIVATE ${MY_FLAGS})
