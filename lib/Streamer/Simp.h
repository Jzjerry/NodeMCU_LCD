#ifndef _SIMP_H_
#define _SIMP_H_

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include "Streamer.h"
#include "Platform_Select.h"

class Simp{
    public:
        Simp(){};
        Simp(String UID)
        {
            this->UID = UID;
            this->_isMembership = 0;
            this->membership_id = "";
        };
        int isMembership()
        {
            return this->_isMembership;
        };
        String getMembershipID()
        {
            return this->membership_id;
        };
        Streamer getStreamer()
        {
            return this->Simp_Streamer;
        };
        StreamUpdateErr_t UpdateMembership(WiFiClient &Connection, HTTPClient &http, DynamicJsonDocument &jsonBuffer); 
    private:
        String UID;
        int _isMembership;
        String membership_id;
        Streamer Simp_Streamer;
};

#endif