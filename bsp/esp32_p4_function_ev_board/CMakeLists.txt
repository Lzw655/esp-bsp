set(PRIV_INCLUDE_DIR "")

list(APPEND PRIV_INCLUDE_DIR "priv_include")
list(APPEND PRIV_INCLUDE_DIR "src/mipi/include")
list(APPEND PRIV_INCLUDE_DIR "src/mipi_soc/include")
list(APPEND PRIV_INCLUDE_DIR "src/ppa/include")

file(GLOB_RECURSE SRCS src/*.c)

idf_component_register(
    SRCS ${SRCS}
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS ${PRIV_INCLUDE_DIR}
    REQUIRES driver
    PRIV_REQUIRES esp_timer spiffs esp_lcd fatfs
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
