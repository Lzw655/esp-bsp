SET(SOURCES screens/ui_MainScreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_location_png.c
    images/ui_img_celsius_png.c
    images/ui_img_sunny_png.c
    images/ui_img_sunny_2_png.c
    images/ui_img_sunset_png.c
    images/ui_img_moon_png.c
    images/ui_img_fog_png.c
    images/ui_img_light_rain_png.c
    images/ui_img_thermometer_png.c
    images/ui_img_humidity_png.c
    images/ui_img_cloud_cover_png.c
    images/ui_img_uv_png.c
    images/ui_img_wind_speed_png.c
    fonts/ui_font_FontPuHui96.c
    fonts/ui_font_FontPuhui20.c
    fonts/ui_font_FontPuhui30.c)

add_library(ui ${SOURCES})
