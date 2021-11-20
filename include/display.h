#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ESP8266WiFi.h>

#include "Streamer.h"

void display_init();
void display_update(Streamer *Streamer_ptr);

#endif