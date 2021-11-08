#include "Streamer.h"

String int2str(int num)
{
  char buf[64] = "";
  sprintf(buf,"%d",num);
  return buf;
}

UpdateErr_t Streamer::UpdateStreamerInfo(WiFiClient &Connection, HTTPClient &http, DynamicJsonDocument &jsonBuffer)
{
    int httpCode;
    http.begin(Connection, LiverInfoUrl + this->UID);
    httpCode = http.GET();
    #ifdef SERIAL_DEBUG
        Serial.print("HTTP Code: ");
        Serial.println(httpCode);
    #endif
    if (httpCode == 200)
    {
        String resBuff = http.getString();

        // ---------- ArduinoJson V6 ----------
        // JsonObject root = jsonBuffer.parseObject(resBuff);
        auto error = deserializeJson(jsonBuffer, resBuff);
        if (error)
        {
        // Serial.print("deserializeJson failed: ");
        // Serial.println(error.c_str());
            return STREAMER_UPDATE_ERR_JSON_FAIL;
        }
        this->follower = jsonBuffer["data"]["follower_num"];
        this->room_id = int2str(jsonBuffer["data"]["room_id"]);
        this->p_follower = this->follower; // Wait? Consider it again, Doesn't seem to be right.
        // {
        //     tft.setCursor(0, 140);
        //     tft.setTextColor(TFT_WHITE, TFT_BLACK);
        //     tft.print("RoomID:");
        //     tft.println(room_id);
        //     tft.print("Fans: ");
        //     if(p_follower<follower) tft.setTextColor(TFT_GREEN, TFT_BLACK);
        //     else if(p_follower>follower) tft.setTextColor(TFT_RED, TFT_BLACK);
        //     tft.println(follower);
        // }
    }
    else
    {
        return STREAMER_UPDATE_ERR_HTTP_FAIL;
    }
    http.end();
    return STREAMER_UPDATE_ERR_SUCCESS;
}

UpdateErr_t Streamer::UpdateLiveStatus(WiFiClient &Connection, HTTPClient &http, DynamicJsonDocument &jsonBuffer)
{
    int httpCode;

    http.begin(Connection, (RoomInfoUrl + this->room_id));
    httpCode = http.GET();
    if (httpCode == 200)
    {
    #ifdef SERIAL_DEBUG
        Serial.print("HTTP Code: ");
        Serial.println(httpCode);
    #endif
        String resBuff = http.getString();
    // ---------- ArduinoJson V6 ----------
    // JsonObject root = jsonBuffer.parseObject(resBuff);
        auto error = deserializeJson(jsonBuffer, resBuff);
        if (error)
        {
            // Serial.print("deserializeJson failed: ");
            // Serial.println(error.c_str());
            return STREAMER_UPDATE_ERR_JSON_FAIL;
        }
        this->live_status = (LiveStatus_t)jsonBuffer["data"]["live_status"];
        /* This part should be moved to frame update function */
        // tft.setCursor(0, 200);
        // if(live_status == 0||live_status == 2)
        // {
        //     tft.setTextColor(TFT_BLUE);
        //     tft.fillRect(0, 200, 240, 40, TFT_BLACK);
        //     tft.fillCircle(200, 200, 30, TFT_BLUE);
        //     tft.print("Off Stream");
        // }
        // else if(live_status == 1)
        // {
        //     tft.setTextColor(TFT_GREENYELLOW);
        //     tft.fillRect(0, 200, 240, 40, TFT_BLACK);
        //     tft.print("On  Stream");
        // }
    }
    else
    {
    // HTTP GET Fail
        // Serial.print("HTTP GET FAIL, CODE:");
        // Serial.println(httpCode);
        return STREAMER_UPDATE_ERR_HTTP_FAIL;
    }
    http.end();
    return STREAMER_UPDATE_ERR_SUCCESS;
}

UpdateErr_t Streamer::UpdateAll(WiFiClient &Connection, HTTPClient &http, DynamicJsonDocument &jsonBuffer)
{
    UpdateErr_t err = STREAMER_UPDATE_ERR_SUCCESS;
    err = UpdateStreamerInfo(Connection, http, jsonBuffer);
    if(err != STREAMER_UPDATE_ERR_SUCCESS) return err;
    err = UpdateLiveStatus(Connection, http, jsonBuffer);
    if(err != STREAMER_UPDATE_ERR_SUCCESS) return err;
    return err;
}