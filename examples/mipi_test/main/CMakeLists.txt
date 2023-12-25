set(srcs "main.c")

list(APPEND srcs "gui/gui.c")

list(APPEND includes "gui")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS ${includes}
                    ) #cannot totally hide soc headers, since there are a lot arguments in the driver are chip-dependent

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")