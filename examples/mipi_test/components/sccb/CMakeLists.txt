set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_PRIV_INCLUDEDIRS "include")
set(COMPONENT_SRCS "i2c.c" "i3c.c")

set(COMPONENT_REQUIRES mipi_soc)

register_component()