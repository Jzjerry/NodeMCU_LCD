#include "Simp.h"

StaticJsonDocument<200> filter;

StreamUpdateErr_t Simp::UpdateMembership(WiFiClient &Connection, HTTPClient &http, DynamicJsonDocument &jsonBuffer)
{
    int httpCode;
    http.begin(Connection, MemberInfoUrl + this->UID);
    httpCode = http.GET();
    filter["data"]["fans_medal"] = true;
    #ifdef SERIAL_DEBUG
        // Serial.print("HTTP Code: ");
        // Serial.println(httpCode);
    #endif
    if (httpCode == 200)
    {
        // String resBuff = http.getString();

        // ---------- ArduinoJson V6 ----------
        // JsonObject root = jsonBuffer.parseObject(resBuff);
        auto error = deserializeJson(jsonBuffer, http.getString(), DeserializationOption::Filter(filter));
        if (error)
        {
            #ifdef SERIAL_DEBUG
                Serial.print("deserializeJson() failed with code ");
                Serial.println(error.c_str());
            #endif
            
            return STREAMER_UPDATE_ERR_JSON_FAIL;
        }
        this->_isMembership = jsonBuffer["data"]["fans_medal"]["wear"];
        if(this->_isMembership == true)
        {
            this->membership_id = int2str(jsonBuffer["data"]["fans_medal"]["medal"]["target_id"]);
            this->Simp_Streamer = Streamer(this->membership_id);
        }
    }
    else
    {
        return STREAMER_UPDATE_ERR_HTTP_FAIL;
    }
    http.end();
    return STREAMER_UPDATE_ERR_SUCCESS;
}