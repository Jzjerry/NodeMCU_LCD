#ifndef _LVGL_PORT_H
#define _LVGL_PORT_H

#include <lvgl.h>
#include <TFT_eSPI.h>

#define LV_USE_LOG 0
#define LV_TOUCH 0


#if LV_USE_LOG != 0
/* Serial debugging */
void my_print( lv_log_level_t level, const char * file, uint32_t line, const char * fn_name, const char * dsc );
#endif

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p );


#if LV_TOUCH != 0
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data );
#endif

#endif