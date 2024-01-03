set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/apps)
file(GLOB_RECURSE C_SOURCES ${SOURCE_DIR}/*.c)
file(GLOB_RECURSE CPP_SOURCES ${SOURCE_DIR}/*.cpp)

idf_component_register(
    SRCS main.cpp ${C_SOURCES} ${CPP_SOURCES}
    INCLUDE_DIRS . ${SOURCE_DIR})

target_compile_options(
    ${COMPONENT_LIB}
    PRIVATE
        -Wno-format
        -Wno-incompatible-pointer-types
        -Wno-int-conversion
        -DLV_LVGL_H_INCLUDE_SIMPLE
        -DLV_USE_DEMO_MUSIC)

# if (NOT CONFIG_EXAMPLE_USE_SD_CARD)
#     spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)
# endif()
