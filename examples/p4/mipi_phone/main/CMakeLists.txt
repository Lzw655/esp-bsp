idf_component_register(
    SRCS main.cpp
    INCLUDE_DIRS .)

target_compile_options(
    ${COMPONENT_LIB}
    PRIVATE
        -Wno-format
        -Wno-incompatible-pointer-types
        -Wno-int-conversion
        -DLV_LVGL_H_INCLUDE_SIMPLE
        -DLV_USE_DEMO_MUSIC)

if (NOT CONFIG_EXAMPLE_USE_SD_CARD)
    spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)
endif()
