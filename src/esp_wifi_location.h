#ifndef ESP_WIFI_LOCATION_H
#define ESP_WIFI_LOCATION_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#ifdef DEBUG_ESP_WIFI_LOCATION
        #define debugLoc(fmt, args ...) Serial.printf_P(PSTR("LOCATION (%d): " \
                                          fmt "\n"), ESP.getFreeHeap(), ## args)
#else
        #define debugLoc(...) /*Don't do anything*/
#endif

static const char google_server[] = "www.googleapis.com";
static const char google_resource[] = "/geolocation/v1/geolocate?key=";
static const int  google_port = 443;
static const char google_fingerprint[] = "EF 23 EB 97 6E 70 8E CA 5C B7 6F A6 "
                                         "6A 00 6F B0 F4 97 10 D3";

class esp_wifi_location
{
public:
        bool getLocation(double &lat, double &lng, double &acc);
        void setMaxWifisToScan(uint8_t max_wifis_scan);
        void setGoogleKey(String google_key);
        void setClient(Client *client);
private:
        bool postToGoogle(DynamicJsonDocument &json, String payload,
                                         double &lat, double &lng, double &acc);
        String create_payload();
        String scan_wifis(DynamicJsonDocument &json);

        WiFiClientSecure wifi_client_secure;
        String google_key = "";
        Client *client = NULL;
        uint8_t max_wifis_scan = 10;
};

#endif
