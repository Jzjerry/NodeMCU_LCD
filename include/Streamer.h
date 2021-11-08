#ifndef _STREAMER_H_
#define _STREAMER_H_

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

/* TODO: 
  * Build a struct or class for UID and related info
  * Store UID, room_id, live_status, follower, p_follower in it
  * So that we can store different streamers and switch between them
  * Or maybe we can get your following list from the API
*/
const String LiverInfoUrl = "http://api.live.bilibili.com/live_user/v1/Master/info?uid=";
const String RoomInfoUrl = "http://api.live.bilibili.com/room/v1/Room/room_init?id=";

enum LiveStatus_t {
  LIVE_OFFSTREAM,
  LIVE_ONSTREAM,
  LIVE_ROUNDING,
  LIVE_UNKNOWN
};

enum UpdateErr_t{
    STREAMER_UPDATE_ERR_SUCCESS,
    STREAMER_UPDATE_ERR_JSON_FAIL,
    STREAMER_UPDATE_ERR_HTTP_FAIL,
    STREAMER_UPDATE_ERR_UNKNOWN
};

class Streamer
{
    public:
        Streamer(){};
        Streamer(String UID)
        {
            this->UID = UID;
            this->room_id = "";
            this->live_status = LIVE_UNKNOWN;
            this->follower = 0;
            this->p_follower = 0;
        };
        ~Streamer(){};
        UpdateErr_t UpdateLiveStatus(WiFiClient &Connection, HTTPClient &http, DynamicJsonDocument &jsonBuffer);
        UpdateErr_t UpdateStreamerInfo(WiFiClient &Connection, HTTPClient &http, DynamicJsonDocument &jsonBuffer);
        UpdateErr_t UpdateAll(WiFiClient &Connection, HTTPClient &http, DynamicJsonDocument &jsonBuffer);

        String getUID() { return this->UID; };
        String getRoomID() { return this->room_id; };
        LiveStatus_t getLiveStatus() { return this->live_status; };
        long getFollower() { return this->follower; };

    private:
        String UID;
        String room_id;
        LiveStatus_t live_status;
        long follower;
        long p_follower; // should be removed
};
#endif