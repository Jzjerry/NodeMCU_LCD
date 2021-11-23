#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Ticker.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <LittleFS.h>

#include <stdio.h>


// User Includes
#include "wifi_config.h"
#include "lvgl_port.h"
#include "lv_port_fs.h"
#include "display.h"
#include "Streamer.h"
#include "Simp.h"

#define FRAME_INTERVAL 20
// Milliseconds
// High FPS leads to some glitching in the frame

#define HTTP_INTERVAL 2000
// Milliseconds

const char *UserAgent = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.128 Safari/537.36 OPR/75.0.3969.218";

long p_follower = 0;
bool first_update = false;
bool is_lv_init = false;


String UID = "";

void lv_ui_init(void);
void lv_ui_update(Streamer *Streamer_ptr);
void fs_test();

String int2str(int num);

WiFiClient Connection = WiFiClient();
TFT_eSPI tft = TFT_eSPI();
Ticker TickerUpdate;
Streamer Bilibili_Vup;
Simp Bilibili_Simp;

static const uint32_t screenWidth  = 240;
static const uint32_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

static void lv_update()
{
  lv_timer_handler();
}

void setup() 
{
  Serial.begin(115200);
  LittleFS.begin();

  lv_init();
  lv_port_fs_init();

  fs_test();

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);

  tft.setSwapBytes(true);

  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init( &disp_drv );
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register( &disp_drv );

  TickerUpdate.attach_ms( FRAME_INTERVAL, lv_update);
  // tft.setCursor(0, 20);
  // tft.setTextFont(4);
  // tft.setTextColor(TFT_WHITE, TFT_BLACK);
  // tft.println("Connecting");
  lv_obj_t * loading_bar = lv_bar_create(lv_scr_act());
  lv_obj_set_size(loading_bar, 200, 30);
  lv_obj_center(loading_bar);
  lv_bar_set_value(loading_bar, 0, LV_ANIM_OFF);
  
  lv_obj_t * loading_label = lv_label_create(lv_scr_act());
  lv_label_set_text(loading_label, "Connecting");
  lv_obj_align_to(loading_label, loading_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

  // Connect to WiFi
  WiFi.begin(ssid, passwd);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    i = (i<100)? i + 5 : 95;
    lv_bar_set_value(loading_bar, i, LV_ANIM_OFF);
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  lv_bar_set_value(loading_bar, 100, LV_ANIM_ON);

  // Print the IP address
  Serial.println(WiFi.localIP());
  lv_label_set_text_fmt(loading_label, "Connected\n%s",
                        WiFi.localIP().toString().c_str());
  // tft.println(WiFi.localIP());
  delay(500);
  display_init();

  // Get UID from Serial Port
  /*
  Serial.println("Please input your UID:");
  while (Serial.available() == 0) {
    delay(100);
  }
  UID = Serial.readStringUntil('\n');
  UID.trim();
  Serial.println(UID);
  */

  UID = "742139"; 

  Bilibili_Simp = Simp(UID);

  // 742139 Follow Jzjerry_Official PLZ!
  // 392505232 Andou Inari
  // 672328094 Diana, my Diana, heh heh, my Diana, take me, take me on, Diana!

  // Attach ticker to frame
  // TickerUpdate.attach(2, Ticker_loop);
}

void loop()
{  
  HTTPClient http;
  DynamicJsonDocument jsonBuffer(4096); // ArduinoJson V6
  if(Bilibili_Simp.UpdateMembership(Connection, http, jsonBuffer) == STREAMER_UPDATE_ERR_SUCCESS)
  {
    #ifdef SERIAL_DEBUG
    Serial.println("Update Membership Success");
    #endif
    Bilibili_Vup = Bilibili_Simp.getStreamer();
  }
  else
  {
    display_error(DISPLAY_SIMP_FAIL);
  }

  if(Bilibili_Vup.UpdateAll(Connection, http, jsonBuffer) == STREAMER_UPDATE_ERR_SUCCESS)
  {
      #ifdef SERIAL_DEBUG
      Serial.println("Update All Success");
      #endif
      display_update(&Bilibili_Vup);
  }
  else
  {
    display_error(DISPLAY_STREAMER_FAIL);
  }

  delay(HTTP_INTERVAL);
}

void fs_test()
{
  lv_fs_file_t f;
  lv_fs_res_t res;

  res = lv_fs_open(&f, "P:/fstest.txt", LV_FS_MODE_RD);

  if(res == LV_FS_RES_OK) Serial.println("File open failed");
  else Serial.println("File open success");


  uint32_t read_num;
  uint8_t buf[8];
  res = lv_fs_read(&f, buf, 8, &read_num);
  Serial.println("expected read bytes: 8");
  Serial.printf("actual read bytes %d\n", read_num);

  lv_fs_close(&f);
}