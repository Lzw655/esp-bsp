set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_PRIV_INCLUDEDIRS "include")
set(COMPONENT_SRCS "csi.c" "dsi.c" "mipi.c" "isp.c")

set(COMPONENT_REQUIRES mipi_soc sccb sensor utils)

register_component()

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")