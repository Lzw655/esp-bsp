idf_component_register(
    SRCS "esp-watch.c" "src/bsp_lcd_panel_st7789.c"
    INCLUDE_DIRS "include"
    REQUIRES driver
    PRIV_REQUIRES esp_timer esp_lcd esp_lcd_touch_cst816s
)
