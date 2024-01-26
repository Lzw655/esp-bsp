file(GLOB_RECURSE SRCS src/*.c)

idf_component_register(
    SRCS ${SRCS}
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "priv_include"
    REQUIRES driver esp_lcd esp_adc
    PRIV_REQUIRES esp_timer spiffs esp_psram
)

if (CONFIG_LV_USE_OS EQUAL 255)
    idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
    message(WARNING ${lvgl_lib})
    target_compile_definitions(${lvgl_lib} PUBLIC "-DLV_OS_CUSTOM_INCLUDE=\"lv_esp_freertos.h\"")
    target_link_libraries(${lvgl_lib} PUBLIC ${COMPONENT_LIB})
endif()
