# API Reference

<div align="center">




| :1234: [CAPABILITIES](#1234-capabilities) | :floppy_disk: [SD CARD AND SPIFFS](#floppy_disk-sd-card-and-spiffs) | :pager: [DISPLAY AND TOUCH](#pager-display-and-touch) | :radio_button: [BUTTONS](#radio_button-buttons) | 
| :-------------------------: | :-------------------------: | :-------------------------: | :-------------------------: | 

</div>



## Overview

This document provides an overview of the ESP-BSP (Board Support Package) API as implemented by this board.

While the ESP-BSP framework defines a unified API shared across multiple boards, this documentation focuses only on the APIs supported by the current board. Any APIs not applicable to this board's hardware are excluded or may not be functional.

The goal of this document is to make it easier for developers to understand the available APIs and how to use them consistently across different boards.

## General

### Pinout

Each BSP defines a set of macros for default pin assignments used by its hardware peripherals.
These macros allow users to configure or reference standard interfaces like I2C, SPI, LCD, audio, or SD cards easily.

- I2C: `BSP_I2C_*`
- Display: `BSP_LCD_*`
- Audio I2S: `BSP_I2S_*`
- USB: `BSP_USB_*`
- SD Card (MMC): `BSP_SD_*`
- SD Card (SPI): `BSP_SD_SPI_*`

> [!NOTE]
> Not all boards support all interfaces. You should always check if the related capability macro (e.g., BSP_CAPS_SDCARD) is defined.

### I2C

Some devices included in BSPs (e.g., sensors, displays, audio codecs) communicate via the I2C interface. In many cases, I2C is initialized automatically as part of the device setup. However, you can manually initialize or deinitialize the I2C peripheral using the following API:

```
/* Initialize the default I2C bus used by the BSP */
bsp_i2c_init();

...

/* Deinitialize the I2C bus */
bsp_i2c_deinit();
```

If you need direct access to the initialized I2C bus (e.g., to communicate with an external peripheral not handled by the BSP), you can retrieve the I2C bus handle:

```
i2c_master_bus_handle_t i2c = bsp_i2c_get_handle();
```

> [!NOTE]
> The BSP ensures that I2C initialization is performed only once, even if called multiple times. This helps avoid conflicts when multiple components rely on the same I2C bus.


### ADC

Some devices included in BSPs (such as buttons, battery monitoring, etc.) use the ADC peripheral. In most cases, the ADC is automatically initialized as part of the specific device setup. However, you can manually initialize the ADC using the following API:

```
/* Initialize the ADC peripheral */
bsp_adc_initialize();
```

If you need direct access to the ADC instance (e.g., for custom measurements), you can retrieve the handle:

```
adc_oneshot_unit_handle_t adc = bsp_adc_get_handle();
```

> [!NOTE]
> The BSP ensures the ADC is initialized only once, even if `bsp_adc_initialize()` is called multiple times.

### Features

Some boards support enabling or disabling specific hardware features (such as LCD, SD card, camera, etc.) to reduce power consumption or manage shared resources. The BSP provides a unified API to control these features:

```
/* Enable the LCD feature */
bsp_feature_enable(BSP_FEATURE_LCD, true);

/* Disable the speaker to reduce power usage */
bsp_feature_enable(BSP_FEATURE_SPEAKER, false);
```

Supported feature flags (may vary depending on the board):
- `BSP_FEATURE_LCD` - Display module
- `BSP_FEATURE_TOUCH` - Touch controller
- `BSP_FEATURE_SD` - SD card interface
- `BSP_FEATURE_SPEAKER`-  Audio speaker
- `BSP_FEATURE_BATTERY` - Battery monitoring
- `BSP_FEATURE_VIBRATION` - Vibration motor

> [!NOTE]
> Not all BSPs support feature toggling, and some features may not be available or controllable via this API. Always check the BSP header or documentation for supported features.

> [!TIP]
> Disabling unused features can help reduce power consumption, especially in battery-powered applications.




## Identification

Each BSP defines an identifier macro in the form of `BSP_BOARD_*`.

### Board Name API Reference



## Macros

| Type | Name |
| ---: | :--- |
| define  | [**BSP\_BOARD\_M5\_ATOM\_S3**](#define-bsp_board_m5_atom_s3)  <br> |









## :1234: Capabilities

Each BSP defines a set of capability macros that indicate which features are supported.
The list may look like this.
You can use these macros to conditionally compile code depending on feature availability.

### Capabilities API Reference



## Macros

| Type | Name |
| ---: | :--- |
| define  | [**BSP\_CAPS\_AUDIO**](#define-bsp_caps_audio)  0<br> |
| define  | [**BSP\_CAPS\_AUDIO\_MIC**](#define-bsp_caps_audio_mic)  0<br> |
| define  | [**BSP\_CAPS\_AUDIO\_SPEAKER**](#define-bsp_caps_audio_speaker)  0<br> |
| define  | [**BSP\_CAPS\_BUTTONS**](#define-bsp_caps_buttons)  1<br> |
| define  | [**BSP\_CAPS\_DISPLAY**](#define-bsp_caps_display)  1<br> |
| define  | [**BSP\_CAPS\_IMU**](#define-bsp_caps_imu)  0<br> |
| define  | [**BSP\_CAPS\_KNOB**](#define-bsp_caps_knob)  0<br> |
| define  | [**BSP\_CAPS\_SDCARD**](#define-bsp_caps_sdcard)  0<br> |
| define  | [**BSP\_CAPS\_TOUCH**](#define-bsp_caps_touch)  0<br> |











### I2C API Reference


## Functions

| Type | Name |
| ---: | :--- |
|  esp\_err\_t | [**bsp\_i2c\_deinit**](#function-bsp_i2c_deinit) (void) <br>_Deinit I2C driver and free its resources._ |
|  i2c\_master\_bus\_handle\_t | [**bsp\_i2c\_get\_handle**](#function-bsp_i2c_get_handle) (void) <br>_Get I2C driver handle._ |
|  esp\_err\_t | [**bsp\_i2c\_init**](#function-bsp_i2c_init) (void) <br>_Init I2C driver._ |

## Macros

| Type | Name |
| ---: | :--- |
| define  | [**BSP\_I2C\_NUM**](#define-bsp_i2c_num)  CONFIG\_BSP\_I2C\_NUM<br> |
| define  | [**BSP\_I2C\_SCL**](#define-bsp_i2c_scl)  (GPIO\_NUM\_39)<br> |
| define  | [**BSP\_I2C\_SDA**](#define-bsp_i2c_sda)  (GPIO\_NUM\_38)<br> |



## Functions Documentation

### function `bsp_i2c_deinit`

_Deinit I2C driver and free its resources._
```c
esp_err_t bsp_i2c_deinit (
    void
) 
```


**Returns:**



* ESP\_OK On success
* ESP\_ERR\_INVALID\_ARG I2C parameter error
### function `bsp_i2c_get_handle`

_Get I2C driver handle._
```c
i2c_master_bus_handle_t bsp_i2c_get_handle (
    void
) 
```


**Returns:**



* I2C handle
### function `bsp_i2c_init`

_Init I2C driver._
```c
esp_err_t bsp_i2c_init (
    void
) 
```


**Returns:**



* ESP\_OK On success
* ESP\_ERR\_INVALID\_ARG I2C parameter error
* ESP\_FAIL I2C driver installation error






## :floppy_disk: SD Card and SPIFFS

### SPIFFS Initialization / Deinitialization

Each BSP provides a simple API for mounting and unmounting the SPI Flash File System (SPIFFS).

```
/* Mount SPIFFS to the virtual file system */
bsp_spiffs_mount();

/* ... perform file operations ... */

/* Unmount SPIFFS from the virtual file system */
bsp_spiffs_unmount();
```

### SD Card Initialization / Deinitialization

The BSP offers a flexible API for working with SD cards. In addition to the default mount and unmount functions, you can also use a configuration structure or access preconfigured `host` and `slot` structures.

Mount with Default Configuration

```
/* Mount microSD card to the virtual file system */
bsp_sdcard_mount();

/* ... perform file operations ... */

/* Unmount microSD card */
bsp_sdcard_unmount();
```

Mount with Custom Configuration

Some BSPs allow selecting between SDMMC and SPI interfaces for the SD card. Use the appropriate API function based on your hardware:
```
bsp_sdcard_cfg_t cfg = {0};
/* Mount SD card using SDMMC interface */
bsp_sdcard_sdmmc_mount(&cfg);
```

or

```
bsp_sdcard_cfg_t cfg = {0};
/* Mount SD card using SPI interface */
bsp_sdcard_sdspi_mount(&cfg)
```

> [!NOTE]
> Not all BSPs support both SDMMC and SPI modes. Check the board documentation to see which interfaces are available.
> If an unsupported interface is used, the API will return `ESP_ERR_NOT_SUPPORTED` error.

### After Mounting

Once the SD card or SPIFFS is mounted, you can use standard file I/O functions (`fopen`, `fread`, `fwrite`, `fclose`, etc.) provided by ESP-IDF's VFS (Virtual File System).

To print basic SD card information (after mounting), you can use:
```
sdmmc_card_t *sdcard = bsp_sdcard_get_handle();
sdmmc_card_print_info(stdout, sdcard);
```

> [!TIP]
> The bsp_sdcard_get_handle() function returns a pointer to the sdmmc_card_t structure, which contains detailed information about the connected SD card.

### SD Card and SPIFFS API Reference


## Functions

| Type | Name |
| ---: | :--- |
|  esp\_err\_t | [**bsp\_spiffs\_mount**](#function-bsp_spiffs_mount) (void) <br>_Mount SPIFFS to virtual file system._ |
|  esp\_err\_t | [**bsp\_spiffs\_unmount**](#function-bsp_spiffs_unmount) (void) <br>_Unmount SPIFFS from virtual file system._ |

## Macros

| Type | Name |
| ---: | :--- |
| define  | [**BSP\_SPIFFS\_MOUNT\_POINT**](#define-bsp_spiffs_mount_point)  CONFIG\_BSP\_SPIFFS\_MOUNT\_POINT<br> |



## Functions Documentation

### function `bsp_spiffs_mount`

_Mount SPIFFS to virtual file system._
```c
esp_err_t bsp_spiffs_mount (
    void
) 
```


**Returns:**



* ESP\_OK on success
* ESP\_ERR\_INVALID\_STATE if esp\_vfs\_spiffs\_register was already called
* ESP\_ERR\_NO\_MEM if memory can not be allocated
* ESP\_FAIL if partition can not be mounted
* other error codes
### function `bsp_spiffs_unmount`

_Unmount SPIFFS from virtual file system._
```c
esp_err_t bsp_spiffs_unmount (
    void
) 
```


**Returns:**



* ESP\_OK on success
* ESP\_ERR\_NOT\_FOUND if the partition table does not contain SPIFFS partition with given label
* ESP\_ERR\_INVALID\_STATE if esp\_vfs\_spiffs\_unregister was already called
* ESP\_ERR\_NO\_MEM if memory can not be allocated
* ESP\_FAIL if partition can not be mounted
* other error codes






## :pager: Display and Touch

### Initialization

ESP-BSP provides two ways to initialize the **display**, **touch** and **LVGL**.

Simple method:

```
/* Initialize display, touch, and LVGL */
lv_display_t display = bsp_display_start();
```

Configurable method:

```
bsp_display_cfg_t cfg = {
    .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),   /* See LVGL Port for more info */
    .buffer_size = BSP_LCD_V_RES * BSP_LCD_H_RES,   /* Screen buffer size in pixels */
    .double_buffer = true,                          /* Allocate two buffers if true */
    .flags = {
        .buff_dma = true,                           /* Use DMA-capable LVGL buffer */
        .buff_spiram = false,                       /* Allocate buffer in PSRAM if true */
    }
};
cfg.lvgl_port_cfg.task_stack = 10000;   /* Example: change LVGL task stack size */
/* Initialize display, touch, and LVGL */
lv_display_t display = bsp_display_start_with_config(&cfg);
```

After initialization, you can use the [LVGL](https://docs.lvgl.io/master/) API or [LVGL Port](../components/esp_lvgl_port/README.md) API.

### Initialization without LVGL - NoGLIB BSP

To initialize the LCD without LVGL, use:

```
esp_lcd_panel_handle_t panel_handle;
esp_lcd_panel_io_handle_t io_handle;
const bsp_display_config_t bsp_disp_cfg = {
    .max_transfer_sz = (BSP_LCD_H_RES * 100) * sizeof(uint16_t),
};
BSP_ERROR_CHECK_RETURN_NULL(bsp_display_new(&bsp_disp_cfg, &panel_handle, &io_handle));
```

To initialize the LCD touch without LVGL, use:

```
esp_lcd_touch_handle_t tp;
bsp_touch_new(NULL, &tp);
```

After initialization, you can use the [ESP-LCD](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/lcd/index.html) API and [ESP-LCD Touch](../components/lcd_touch/README.md) API.

### Set Brightness

```
/* Set display brightness to 100% */
bsp_display_backlight_on();

/* Set display brightness to 0% */
bsp_display_backlight_off();

/* Set display brightness to 50% */
bsp_display_brightness_set(50);
```

> [!NOTE]
> Some boards do not support changing brightness. They return an `ESP_ERR_NOT_SUPPORTED` error.

### LVGL API Usage (only when initialized with LVGL)

All LVGL calls must be protected using lock/unlock:

```
/* Wait until other tasks finish screen operations */
bsp_display_lock(0);
...
lv_obj_t * screen = lv_disp_get_scr_act(disp_handle);
lv_obj_t * obj = lv_label_create(screen);
...
/* Unlock after screen operations are done */
bsp_display_unlock();
```

### Screen rotation (only when initialized with LVGL)

```
bsp_display_lock(0);
/* Rotate display to 90 */
bsp_display_rotate(display, LV_DISPLAY_ROTATION_90);
bsp_display_unlock();
```

> [!NOTE]
> Some LCDs do not support hardware rotation and instead use software rotation, which consumes more memory.

### Available constants

Constants like screen resolution, pin configuration, and other options are defined in the BSP header files (`{bsp_name}.h`, `display.h`, `touch.h`).
Below are some of the most relevant predefined constants:

- `BSP_LCD_H_RES` - Horizontal resolution in pixels
- `BSP_LCD_V_RES` - Vertical resolution in pixels
- `BSP_LCD_SPI_NUM` - SPI bus used by the LCD (if applicable)


### Display and Touch API Reference

## Structures and Types

| Type | Name |
| ---: | :--- |
| struct | [**bsp\_display\_cfg\_t**](#struct-bsp_display_cfg_t) <br>_BSP display configuration structure._ |
| struct | [**bsp\_display\_config\_t**](#struct-bsp_display_config_t) <br>_BSP display configuration structure._ |

## Functions

| Type | Name |
| ---: | :--- |
|  esp\_err\_t | [**bsp\_display\_backlight\_off**](#function-bsp_display_backlight_off) (void) <br>_Turn off display backlight._ |
|  esp\_err\_t | [**bsp\_display\_backlight\_on**](#function-bsp_display_backlight_on) (void) <br>_Turn on display backlight._ |
|  esp\_err\_t | [**bsp\_display\_brightness\_init**](#function-bsp_display_brightness_init) (void) <br>_Initialize display's brightness._ |
|  esp\_err\_t | [**bsp\_display\_brightness\_set**](#function-bsp_display_brightness_set) (int brightness\_percent) <br>_Set display's brightness._ |
|  lv\_indev\_t \* | [**bsp\_display\_get\_input\_dev**](#function-bsp_display_get_input_dev) (void) <br>_Get pointer to input device (touch, buttons, ...)_ |
|  bool | [**bsp\_display\_lock**](#function-bsp_display_lock) (uint32\_t timeout\_ms) <br>_Take LVGL mutex._ |
|  esp\_err\_t | [**bsp\_display\_new**](#function-bsp_display_new) (const [**bsp\_display\_config\_t**](#struct-bsp_display_config_t) \*config, esp\_lcd\_panel\_handle\_t \*ret\_panel, esp\_lcd\_panel\_io\_handle\_t \*ret\_io) <br>_Create new display panel._ |
|  void | [**bsp\_display\_rotate**](#function-bsp_display_rotate) (lv\_display\_t \*disp, lv\_display\_rotation\_t rotation) <br>_Rotate screen._ |
|  lv\_display\_t \* | [**bsp\_display\_start**](#function-bsp_display_start) (void) <br>_Initialize display._ |
|  lv\_display\_t \* | [**bsp\_display\_start\_with\_config**](#function-bsp_display_start_with_config) (const [**bsp\_display\_cfg\_t**](#struct-bsp_display_cfg_t) \*cfg) <br>_Initialize display._ |
|  void | [**bsp\_display\_unlock**](#function-bsp_display_unlock) (void) <br>_Give LVGL mutex._ |

## Macros

| Type | Name |
| ---: | :--- |
| define  | [**BSP\_LCD\_BACKLIGHT**](#define-bsp_lcd_backlight)  (GPIO\_NUM\_16)<br> |
| define  | [**BSP\_LCD\_BIGENDIAN**](#define-bsp_lcd_bigendian)  (1)<br> |
| define  | [**BSP\_LCD\_BITS\_PER\_PIXEL**](#define-bsp_lcd_bits_per_pixel)  (16)<br> |
| define  | [**BSP\_LCD\_COLOR\_FORMAT**](#define-bsp_lcd_color_format)  (ESP\_LCD\_COLOR\_FORMAT\_RGB565)<br> |
| define  | [**BSP\_LCD\_COLOR\_SPACE**](#define-bsp_lcd_color_space)  (ESP\_LCD\_COLOR\_SPACE\_BGR)<br> |
| define  | [**BSP\_LCD\_CS**](#define-bsp_lcd_cs)  (GPIO\_NUM\_15)<br> |
| define  | [**BSP\_LCD\_DC**](#define-bsp_lcd_dc)  (GPIO\_NUM\_33)<br> |
| define  | [**BSP\_LCD\_DRAW\_BUFF\_DOUBLE**](#define-bsp_lcd_draw_buff_double)  (1)<br> |
| define  | [**BSP\_LCD\_DRAW\_BUFF\_SIZE**](#define-bsp_lcd_draw_buff_size)  (BSP\_LCD\_H\_RES \* 50)<br> |
| define  | [**BSP\_LCD\_H\_RES**](#define-bsp_lcd_h_res)  (128)<br> |
| define  | [**BSP\_LCD\_MISO**](#define-bsp_lcd_miso)  (GPIO\_NUM\_NC)<br> |
| define  | [**BSP\_LCD\_MOSI**](#define-bsp_lcd_mosi)  (GPIO\_NUM\_21)<br> |
| define  | [**BSP\_LCD\_PCLK**](#define-bsp_lcd_pclk)  (GPIO\_NUM\_17)<br> |
| define  | [**BSP\_LCD\_PIXEL\_CLOCK\_HZ**](#define-bsp_lcd_pixel_clock_hz)  (40 \* 1000 \* 1000)<br> |
| define  | [**BSP\_LCD\_RST**](#define-bsp_lcd_rst)  (GPIO\_NUM\_34)<br> |
| define  | [**BSP\_LCD\_SPI\_NUM**](#define-bsp_lcd_spi_num)  (SPI3\_HOST)<br> |
| define  | [**BSP\_LCD\_V\_RES**](#define-bsp_lcd_v_res)  (128)<br> |
| define  | [**ESP\_LCD\_COLOR\_FORMAT\_RGB565**](#define-esp_lcd_color_format_rgb565)  (1)<br> |
| define  | [**ESP\_LCD\_COLOR\_FORMAT\_RGB888**](#define-esp_lcd_color_format_rgb888)  (2)<br> |


## Structures and Types Documentation

### struct `bsp_display_cfg_t`

_BSP display configuration structure._

Variables:

-  unsigned int buff_dma  <br>Allocated LVGL buffer will be DMA capable

-  unsigned int buff_spiram  <br>Allocated LVGL buffer will be in PSRAM

-  uint32\_t buffer_size  <br>Size of the buffer for the screen in pixels

-  bool double_buffer  <br>True, if should be allocated two buffers

-  struct [**bsp\_display\_cfg\_t**](#struct-bsp_display_cfg_t) flags  

-  lvgl\_port\_cfg\_t lvgl_port_cfg  <br>LVGL port configuration

### struct `bsp_display_config_t`

_BSP display configuration structure._

Variables:

-  int max_transfer_sz  <br>Maximum transfer size, in bytes.


## Functions Documentation

### function `bsp_display_backlight_off`

_Turn off display backlight._
```c
esp_err_t bsp_display_backlight_off (
    void
) 
```


Brightness is controlled with PWM signal to a pin controlling backlight. Brightness must be already initialized by calling [**bsp\_display\_brightness\_init()**](#function-bsp_display_brightness_init) or[**bsp\_display\_new()**](#function-bsp_display_new)



**Returns:**



* ESP\_OK On success
* ESP\_ERR\_INVALID\_ARG Parameter error
### function `bsp_display_backlight_on`

_Turn on display backlight._
```c
esp_err_t bsp_display_backlight_on (
    void
) 
```


Brightness is controlled with PWM signal to a pin controlling backlight. Brightness must be already initialized by calling [**bsp\_display\_brightness\_init()**](#function-bsp_display_brightness_init) or[**bsp\_display\_new()**](#function-bsp_display_new)



**Returns:**



* ESP\_OK On success
* ESP\_ERR\_INVALID\_ARG Parameter error
### function `bsp_display_brightness_init`

_Initialize display's brightness._
```c
esp_err_t bsp_display_brightness_init (
    void
) 
```


Brightness is controlled with PWM signal to a pin controlling backlight.



**Returns:**



* ESP\_OK On success
* ESP\_ERR\_INVALID\_ARG Parameter error
### function `bsp_display_brightness_set`

_Set display's brightness._
```c
esp_err_t bsp_display_brightness_set (
    int brightness_percent
) 
```


Brightness is controlled with PWM signal to a pin controlling backlight. Brightness must be already initialized by calling [**bsp\_display\_brightness\_init()**](#function-bsp_display_brightness_init) or[**bsp\_display\_new()**](#function-bsp_display_new)



**Parameters:**


* `brightness_percent` Brightness in [%] 


**Returns:**



* ESP\_OK On success
* ESP\_ERR\_INVALID\_ARG Parameter error
### function `bsp_display_get_input_dev`

_Get pointer to input device (touch, buttons, ...)_
```c
lv_indev_t * bsp_display_get_input_dev (
    void
) 
```


**Note:**

The LVGL input device is initialized in [**bsp\_display\_start()**](#function-bsp_display_start) function.



**Returns:**

Pointer to LVGL input device or NULL when not initialized
### function `bsp_display_lock`

_Take LVGL mutex._
```c
bool bsp_display_lock (
    uint32_t timeout_ms
) 
```


**Parameters:**


* `timeout_ms` Timeout in [ms]. 0 will block indefinitely. 


**Returns:**

true Mutex was taken 



**Returns:**

false Mutex was NOT taken
### function `bsp_display_new`

_Create new display panel._
```c
esp_err_t bsp_display_new (
    const bsp_display_config_t *config,
    esp_lcd_panel_handle_t *ret_panel,
    esp_lcd_panel_io_handle_t *ret_io
) 
```


For maximum flexibility, this function performs only reset and initialization of the display. You must turn on the display explicitly by calling esp\_lcd\_panel\_disp\_on\_off(). The display's backlight is not turned on either. You can use bsp\_display\_backlight\_on/off(), [**bsp\_display\_brightness\_set()**](#function-bsp_display_brightness_set) (on supported boards) or implement your own backlight control.

If you want to free resources allocated by this function, you can use esp\_lcd API, ie.:


````cpp
esp_lcd_panel_del(panel);
esp_lcd_panel_io_del(io);
spi_bus_free(spi_num_from_configuration);
````





**Parameters:**


* `config` display configuration 
* `ret_panel` esp\_lcd panel handle 
* `ret_io` esp\_lcd IO handle 


**Returns:**



* ESP\_OK On success
* Else esp\_lcd failure
### function `bsp_display_rotate`

_Rotate screen._
```c
void bsp_display_rotate (
    lv_display_t *disp,
    lv_display_rotation_t rotation
) 
```


Display must be already initialized by calling [**bsp\_display\_start()**](#function-bsp_display_start)



**Parameters:**


* `disp` Pointer to LVGL display 
* `rotation` Angle of the display rotation
### function `bsp_display_start`

_Initialize display._
```c
lv_display_t * bsp_display_start (
    void
) 
```


This function initializes SPI, display controller and starts LVGL handling task. LCD backlight must be enabled separately by calling [**bsp\_display\_brightness\_set()**](#function-bsp_display_brightness_set)



**Returns:**

Pointer to LVGL display or NULL when error occured
### function `bsp_display_start_with_config`

_Initialize display._
```c
lv_display_t * bsp_display_start_with_config (
    const bsp_display_cfg_t *cfg
) 
```


This function initializes SPI, display controller and starts LVGL handling task. LCD backlight must be enabled separately by calling [**bsp\_display\_brightness\_set()**](#function-bsp_display_brightness_set)



**Parameters:**


* `cfg` display configuration


**Returns:**

Pointer to LVGL display or NULL when error occured
### function `bsp_display_unlock`

_Give LVGL mutex._
```c
void bsp_display_unlock (
    void
) 
```







## :radio_button: Buttons

Most boards include one or more user buttons. The BSP provides a simple API to initialize and use them.
Internally, it utilizes the [button](https://components.espressif.com/components/espressif/button/) component for event handling and debouncing.

```
/* Initialize all available buttons */
button_handle_t btns[BSP_BUTTON_NUM] = {NULL};
bsp_iot_button_create(btns, NULL, BSP_BUTTON_NUM);

/* Register a callback for button press */
for (int i = 0; i < BSP_BUTTON_NUM; i++) {
    iot_button_register_cb(btns[i], BUTTON_PRESS_DOWN, NULL, btn_handler, (void *) i);
}

/* Called on button press */
static void btn_handler(void *button_handle, void *usr_data)
{
    int button_index = (int)usr_data;
    ESP_LOGI(TAG, "Button %d pressed", button_index);
}
```

**Notes:**
- `BSP_BUTTON_NUM` defines the number of available buttons on the board

- Instead of relying on a numeric index, you can use the `bsp_button_t` structure defined in the BSP header to reference buttons by name (e.g., BSP_BUTTON_MUTE, BSP_BUTTON_MAIN, BSP_BUTTON_BOOT, etc.)

- The callback can be registered for different button events, such as `BUTTON_PRESS_DOWN`, `BUTTON_PRESS_UP`, or `BUTTON_LONG_PRESS_START`, `BUTTON_DOUBLE_CLICK`, ...

- Button features (e.g. long-press support, active level) are configured automatically by the BSP.

### Buttons API Reference

## Structures and Types

| Type | Name |
| ---: | :--- |
| enum  | [**bsp\_button\_t**](#enum-bsp_button_t)  <br> |

## Functions

| Type | Name |
| ---: | :--- |
|  esp\_err\_t | [**bsp\_iot\_button\_create**](#function-bsp_iot_button_create) (button\_handle\_t btn\_array, int \*btn\_cnt, int btn\_array\_size) <br>_Initialize all buttons._ |

## Macros

| Type | Name |
| ---: | :--- |
| define  | [**BSP\_BUTTON\_PRESS\_IO**](#define-bsp_button_press_io)  (GPIO\_NUM\_41)<br> |


## Structures and Types Documentation

### enum `bsp_button_t`

```c
enum bsp_button_t {
    BSP_BUTTON_PRESS,
    BSP_BUTTON_NUM
};
```


## Functions Documentation

### function `bsp_iot_button_create`

_Initialize all buttons._
```c
esp_err_t bsp_iot_button_create (
    button_handle_t btn_array,
    int *btn_cnt,
    int btn_array_size
) 
```


Returned button handlers must be used with espressif/button component API



**Parameters:**


* `btn_array` Output button array 
* `btn_cnt` Number of button handlers saved to btn\_array, can be NULL 
* `btn_array_size` Size of output button array. Must be at least BSP\_BUTTON\_NUM 


**Returns:**



* ESP\_OK All buttons initialized
* ESP\_ERR\_INVALID\_ARG btn\_array is too small or NULL
* ESP\_FAIL Underlaying iot\_button\_create failed



