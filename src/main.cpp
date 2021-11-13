#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Ticker.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

#include <stdio.h>


// User Includes
#include "wifi_config.h"
#include "Streamer.h"
#include "Simp.h"


#define FRAME_INTERVAL 0.1 
// Seconds
// High FPS leads to some glitching in the frame

#define HTTP_INTERVAL 2000
// Milliseconds

const char *UserAgent = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.128 Safari/537.36 OPR/75.0.3969.218";

long p_follower = 0;
// long follower = 0;
// long room_id = 0;
// int live_status = 0;
bool first_update = false;


String UID = "";
// String LiverInfoUrl = "http://api.live.bilibili.com/live_user/v1/Master/info?uid=";
// String RoomInfoUrl = "http://api.live.bilibili.com/room/v1/Room/room_init?id=";

void getFollower(HTTPClient &http, DynamicJsonDocument &jsonBuffer);
void getLiveStatus(HTTPClient &http, DynamicJsonDocument &jsonBuffer);

void FrameInfoUpdate(Streamer *Streamer_ptr);
void FrameAnimationUpdate(Streamer *Streamer_ptr);

String int2str(int num);

WiFiClient Connection = WiFiClient();
TFT_eSPI tft = TFT_eSPI();
Ticker ScreenUpdate;
Streamer Bilibili_Vup;
Simp Bilibili_Simp;


void setup() 
{
  Serial.begin(115200);

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  tft.setCursor(0, 20);
  tft.setTextFont(4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Connecting");

  // Connect to WiFi
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tft.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  tft.setCursor(0, 20);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Connected \n");
  

  // Print the IP address
  Serial.println(WiFi.localIP());
  tft.println(WiFi.localIP());

  
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

  tft.print("YourUID:");
  tft.println(UID);

  // Attach ticker to frame
  ScreenUpdate.attach( FRAME_INTERVAL, FrameAnimationUpdate, &Bilibili_Vup);

}

void loop()
{
  DynamicJsonDocument jsonBuffer(4096); // ArduinoJson V6
  HTTPClient http;
  
  if(Bilibili_Simp.UpdateMembership(Connection, http, jsonBuffer) == STREAMER_UPDATE_ERR_SUCCESS)
  {
    #ifdef SERIAL_DEBUG
      Serial.println("Update Membership Success");
    #endif
    Bilibili_Vup = Bilibili_Simp.getStreamer();
    if(Bilibili_Vup.UpdateAll(Connection, http, jsonBuffer) == STREAMER_UPDATE_ERR_SUCCESS)
    {
      #ifdef SERIAL_DEBUG
        Serial.println("Update All Success");
      #endif
      FrameInfoUpdate(&Bilibili_Vup);
    }
  }
  
  if(!first_update) first_update = true;

  delay(HTTP_INTERVAL);
}

void FrameAnimationUpdate(Streamer *Streamer_ptr)
{
  static long circle_radius = 10;
  static long radius_inc = 2;

  if(first_update)
  {
    if(Streamer_ptr->getLiveStatus() != LIVE_ONSTREAM)
    {
      // tft.fillRect(170, 170, 60, 60, TFT_BLACK);
      // tft.fillCircle(200, 200, 30, TFT_BLUE);
    }
    else
    {
      tft.fillRect(170, 170, 60, 60, TFT_BLACK);
      tft.fillCircle(200, 200, circle_radius, TFT_GREENYELLOW);
      circle_radius += radius_inc;
      if(circle_radius>=25||circle_radius<=10) radius_inc = -radius_inc;
    }

  }
  return;
}

void FrameInfoUpdate(Streamer *Streamer_ptr)
{
  {
    long follower;
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.fillRect(0, 140, 240, 40, TFT_BLACK); // Clear last time print
    tft.setCursor(0, 140);
    tft.print("VupUID:");
    tft.println(Streamer_ptr->getUID());
    // tft.print("RoomID:");
    // tft.println(Bilibili_Vup.getRoomID()); // Too crowded to show
    follower = Streamer_ptr->getFollower();
    tft.print("Fans: ");
    if(p_follower<follower) tft.setTextColor(TFT_GREEN, TFT_BLACK);
    else if(p_follower>follower) tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print(follower);
    tft.print("       "); // Clean last time print
    p_follower = follower;
  }

  {
    tft.setCursor(0, 200);
    if( Streamer_ptr->getLiveStatus() != LIVE_ONSTREAM)
    {
      tft.setTextColor(TFT_BLUE);
      tft.fillRect(0, 200, 240, 40, TFT_BLACK);
      tft.fillCircle(200, 200, 25, TFT_BLUE);
      tft.print("Off Stream");
    }
    else
    {
      tft.setTextColor(TFT_GREENYELLOW);
      tft.fillRect(0, 200, 240, 40, TFT_BLACK);
      tft.print("On  Stream");
    }
  }
}

/*
void getFollower(HTTPClient &http, DynamicJsonDocument &jsonBuffer)
{
  int httpCode;
  http.begin(Connection, LiverInfoUrl + UID);
  httpCode = http.GET();
  Serial.println("Request Send");
  if (httpCode == 200)
  {
    Serial.println("Get OK");
    String resBuff = http.getString();

    // ---------- ArduinoJson V6 ----------
    // JsonObject root = jsonBuffer.parseObject(resBuff);
    auto error = deserializeJson(jsonBuffer, resBuff);
    if (error)
    {
      // Serial.print("deserializeJson failed: ");
      // Serial.println(error.c_str());
      return;
    }
    follower = jsonBuffer["data"]["follower_num"];
    room_id = jsonBuffer["data"]["room_id"];
    {
      tft.setCursor(0, 140);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.print("RoomID:");
      tft.println(room_id);
      tft.print("Fans: ");
      if(p_follower<follower) tft.setTextColor(TFT_GREEN, TFT_BLACK);
      else if(p_follower>follower) tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println(follower);
    }
    p_follower = follower;
  }
  else
  {
    // HTTP GET Fail
    Serial.print("HTTP GET FAIL, CODE:");
    Serial.println(httpCode);
  }
  http.end();
}

void getLiveStatus(HTTPClient &http, DynamicJsonDocument &jsonBuffer)
{
  int httpCode;
  String room_id_str;

  room_id_str = int2str(room_id);
  http.begin(Connection, (RoomInfoUrl + room_id_str));
  httpCode = http.GET();
  Serial.println("Request Send");
  if (httpCode == 200)
  {
    Serial.println("Get OK");
    String resBuff = http.getString();

    // ---------- ArduinoJson V6 ----------
    // JsonObject root = jsonBuffer.parseObject(resBuff);
    auto error = deserializeJson(jsonBuffer, resBuff);
    if (error)
    {
      // Serial.print("deserializeJson failed: ");
      // Serial.println(error.c_str());
      return;
    }
    live_status = jsonBuffer["data"]["live_status"];
    tft.setCursor(0, 200);
    if(live_status == 0||live_status == 2)
    {
      tft.setTextColor(TFT_BLUE);
      tft.fillRect(0, 200, 240, 40, TFT_BLACK);
      tft.fillCircle(200, 200, 30, TFT_BLUE);
      tft.print("Off Stream");
    }
    else if(live_status == 1)
    {
      tft.setTextColor(TFT_GREENYELLOW);
      tft.fillRect(0, 200, 240, 40, TFT_BLACK);
      tft.print("On  Stream");
    }
  }
  else
  {
    // HTTP GET Fail
    Serial.print("HTTP GET FAIL, CODE:");
    Serial.println(httpCode);
  }
  http.end();
}
*/