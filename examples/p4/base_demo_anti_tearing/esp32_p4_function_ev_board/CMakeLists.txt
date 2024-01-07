set(INCLUDE_DIR "")
list(APPEND INCLUDE_DIR "include")
list(APPEND INCLUDE_DIR "priv_include")
list(APPEND INCLUDE_DIR "src/mipi/include")
list(APPEND INCLUDE_DIR "src/mipi_soc/include")
list(APPEND INCLUDE_DIR "src/ppa/include")
list(APPEND INCLUDE_DIR "src/sensor/include")
list(APPEND INCLUDE_DIR "src/isp/include")
list(APPEND INCLUDE_DIR "src/lcd/include")

file(GLOB_RECURSE SRCS src/*.c)

idf_component_register(
    SRCS ${SRCS}
    INCLUDE_DIRS ${INCLUDE_DIR}
    REQUIRES driver
    PRIV_REQUIRES esp_timer spiffs esp_lcd fatfs esp_psram esp_mm
)

target_compile_options(
    ${COMPONENT_LIB} PRIVATE
        -Wno-int-conversion
        -Wno-incompatible-pointer-types
        -Wunused-function
        -Wno-unused-variable
        -Wno-unused-function
        -Wno-overflow
        -Wno-unused-but-set-variable
        -Wno-discarded-qualifiers)
