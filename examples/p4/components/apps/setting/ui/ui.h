// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: UI_Setting

#ifndef _UI_SETTING_UI_H
#define _UI_SETTING_UI_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
// SCREEN: ui_ScreenSettingMain
void ui_ScreenSettingMain_screen_init(void);
extern lv_obj_t *ui_ScreenSettingMain;
extern lv_obj_t *ui_PanelSettingMainContainer;
void ui_event_PanelSettingMainContainerItem1( lv_event_t * e);
extern lv_obj_t *ui_PanelSettingMainContainerItem1;
extern lv_obj_t *ui_ImagePanelSettingMainContainer1WiFi;
extern lv_obj_t *ui_ImagePanelSettingMainContainer1Arrow;
extern lv_obj_t *ui_LabelPanelSettingMainContainer1WiFi;
void ui_event_PanelSettingMainContainerItem2( lv_event_t * e);
extern lv_obj_t *ui_PanelSettingMainContainerItem2;
extern lv_obj_t *ui_ImagePanelSettingMainContainer2Blue;
extern lv_obj_t *ui_ImagePanelSettingMainContainer2Arrow;
extern lv_obj_t *ui_LabelPanelSettingMainContainer2Blue;
void ui_event_PanelSettingMainContainerItem3( lv_event_t * e);
extern lv_obj_t *ui_PanelSettingMainContainerItem3;
extern lv_obj_t *ui_ImagePanelSettingMainContainer3Volume;
extern lv_obj_t *ui_ImagePanelSettingMainContainer3Arrow;
extern lv_obj_t *ui_LabelPanelSettingMainContainer3Volume;
void ui_event_PanelSettingMainContainerItem4( lv_event_t * e);
extern lv_obj_t *ui_PanelSettingMainContainerItem4;
extern lv_obj_t *ui_ImagePanelSettingMainContainer4Light;
extern lv_obj_t *ui_ImagePanelSettingMainContainer4Arrow;
extern lv_obj_t *ui_LabelPanelSettingMainContainer4Light;
void ui_event_PanelSettingMainContainerItem5( lv_event_t * e);
extern lv_obj_t *ui_PanelSettingMainContainerItem5;
extern lv_obj_t *ui_ImagePanelSettingMainContainer5About;
extern lv_obj_t *ui_ImagePanelSettingMainContainer5Arrow;
extern lv_obj_t *ui_LabelPanelSettingMainContainer5About;
// SCREEN: ui_ScreenSettingWiFi
void ui_ScreenSettingWiFi_screen_init(void);
extern lv_obj_t *ui_ScreenSettingWiFi;
extern lv_obj_t *ui_PanelScreenSettingWiFiSwitch;
extern lv_obj_t *ui_ImagePanelScreenSettingWiFiSwitch;
extern lv_obj_t *ui_LabelPanelScreenSettingWiFiSwitch;
extern lv_obj_t *ui_SwitchPanelScreenSettingWiFiSwitch;
extern lv_obj_t *ui_PanelScreenSettingWiFiList;
extern lv_obj_t *ui_SpinnerScreenSettingWiFi;
void ui_event_ButtonScreenSettingWiFiReturn( lv_event_t * e);
extern lv_obj_t *ui_ButtonScreenSettingWiFiReturn;
extern lv_obj_t *ui_ImageScreenSettingWiFiReturn;
// SCREEN: ui_ScreenSettingBLE
void ui_ScreenSettingBLE_screen_init(void);
extern lv_obj_t *ui_ScreenSettingBLE;
extern lv_obj_t *ui_PanelScreenSettingBLESwitch;
extern lv_obj_t *ui_ImagePanelScreenSettingBLESwitch;
extern lv_obj_t *ui_LabelPanelScreenSettingBLESwitch;
extern lv_obj_t *ui_SwitchPanelScreenSettingBLESwitch;
extern lv_obj_t *ui_PanelScreenSettingBLEList;
extern lv_obj_t *ui_SpinnerScreenSettingBLE;
void ui_event_ButtonScreenSettingBLEReturn( lv_event_t * e);
extern lv_obj_t *ui_ButtonScreenSettingBLEReturn;
extern lv_obj_t *ui_ImageScreenSettingBLEReturn;
// SCREEN: ui_ScreenSettingVolume
void ui_ScreenSettingVolume_screen_init(void);
extern lv_obj_t *ui_ScreenSettingVolume;
extern lv_obj_t *ui_PanelScreenSettingVolumeSwitch;
extern lv_obj_t *ui_ImagePanelScreenSettingVolumeSwitch;
extern lv_obj_t *ui_LabelPanelScreenSettingVolumeSwitch;
extern lv_obj_t *ui_SliderPanelScreenSettingVolumeSwitch;
extern lv_obj_t *ui_PanelScreenSettingVolumeList;
extern lv_obj_t *ui_SpinnerScreenSettingVolume;
void ui_event_ButtonScreenSettingVolumeReturn( lv_event_t * e);
extern lv_obj_t *ui_ButtonScreenSettingVolumeReturn;
extern lv_obj_t *ui_ImageScreenSettingVolumeReturn;
// SCREEN: ui_ScreenSettingLight
void ui_ScreenSettingLight_screen_init(void);
extern lv_obj_t *ui_ScreenSettingLight;
extern lv_obj_t *ui_PanelScreenSettingLightSwitch;
extern lv_obj_t *ui_ImagePanelScreenSettingLightSwitch;
extern lv_obj_t *ui_LabelPanelScreenSettingLightSwitch;
extern lv_obj_t *ui_SliderPanelScreenSettingLightSwitch1;
extern lv_obj_t *ui_PanelScreenSettingLightList;
extern lv_obj_t *ui_SpinnerScreenSettingLight;
void ui_event_ButtonScreenSettingLightReturn( lv_event_t * e);
extern lv_obj_t *ui_ButtonScreenSettingLightReturn;
extern lv_obj_t *ui_ImageScreenSettingLightReturn;
// SCREEN: ui_ScreenSettingAbout
void ui_ScreenSettingAbout_screen_init(void);
extern lv_obj_t *ui_ScreenSettingAbout;
extern lv_obj_t *ui_PanelScreenSettingAboutList;
extern lv_obj_t *ui_SpinnerScreenAboutList;
void ui_event_ButtonScreenSettingAboutReturn( lv_event_t * e);
extern lv_obj_t *ui_ButtonScreenSettingAboutReturn;
extern lv_obj_t *ui_ImageScreenSettingAboutReturn;
extern lv_obj_t *ui_ImageScreenSettingAbout;
extern lv_obj_t *ui_PanelScreenSettingAbout;
extern lv_obj_t *ui_PanelPanelScreenSettingAbout;
extern lv_obj_t *ui_LabelPanelPanelScreenSettingAboutDevice;
extern lv_obj_t *ui_LabelPanelPanelScreenSettingAbout2;
extern lv_obj_t *ui_PanelPanelScreenSettingAbout1;
extern lv_obj_t *ui_LabelPanelPanelScreenSettingAboutManufacturer;
extern lv_obj_t *ui_LabelPanelPanelScreenSettingAbout1;
extern lv_obj_t *ui_PanelPanelScreenSettingAbout2;
extern lv_obj_t *ui_LabelPanelPanelScreenSettingAboutMAC;
extern lv_obj_t *ui_LabelPanelPanelScreenSettingAbout3;
extern lv_obj_t *ui_PanelPanelScreenSettingAbout3;
extern lv_obj_t *ui_LabelPanelPanelScreenSettingAboutUIFramework;
extern lv_obj_t *ui_LabelPanelPanelScreenSettingAbout4;
extern lv_obj_t *ui_PanelPanelScreenSettingAbout4;
extern lv_obj_t *ui_LabelPanelPanelScreenSettingAboutSoftwareVersion;
extern lv_obj_t *ui_LabelPanelPanelScreenSettingAbout5;
extern lv_obj_t *ui____initial_setting_actions0;

LV_IMG_DECLARE( ui_img_wifi_png);   // assets/WiFi.png
LV_IMG_DECLARE( ui_img_arrow_png);   // assets/arrow.png
LV_IMG_DECLARE( ui_img_bluetooth_png);   // assets/BlueTooth.png
LV_IMG_DECLARE( ui_img_sound_png);   // assets/Sound.png
LV_IMG_DECLARE( ui_img_light_png);   // assets/Light.png
LV_IMG_DECLARE( ui_img_about_png);   // assets/About.png
LV_IMG_DECLARE( ui_img_return_png);   // assets/return.png
LV_IMG_DECLARE( ui_img_482951309);   // assets/espressif-logo.png

void ui_setting_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif