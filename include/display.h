#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ESP8266WiFi.h>

#include "lv_port_fs.h"
#include "Streamer.h"


enum display_error_t
{
    DISPLAY_SUCCESS,
    DISPLAY_SIMP_FAIL,
    DISPLAY_STREAMER_FAIL,
    DISPLAY_DEFAULT
};


void display_init();
void display_update(Streamer *Streamer_ptr);
void display_error(display_error_t err);

#endif