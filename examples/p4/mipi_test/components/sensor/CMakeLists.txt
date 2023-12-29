set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_PRIV_INCLUDEDIRS "include")
set(COMPONENT_SRCS "dw9714.c" "imx219.c" "ov5640.c" "ov5647.c")

set(COMPONENT_REQUIRES mipi_soc mipi sccb)

register_component()