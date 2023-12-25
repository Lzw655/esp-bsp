#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
TouchPad_ID:9,1,1
TouchPad_Config_Version:41
FirmwareVersion:1060
*/
#define CT_MAX_TOUCH   5

#define GT911_MAX_WIDTH			800    	//Touchscreen pad max width
#define GT911_MAX_HEIGHT		1280			//Touchscreen pad max height

#define GT911_PRODUCT_ID_REG	    0x8140
#define GT911_CONFIG_REG		    0x8047
#define GT911_FIRMWARE_VERSION_REG  0x8144 
#define GT911_READ_XY_REG 			0x814E

#define GT911_I2C_ADDR  0x5D

void gt911_touch_init(void);
void gt911_scan(void);
int gt911_pos_read(uint16_t *xpos, uint16_t *ypos);

#ifdef __cplusplus
}
#endif
