#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <esp_wifi_location.h>

esp_wifi_location location;
#define LOCATION_GOOGLE_KEY "INSERT YOUR GOOGLE API KEY HERE"

void setup()
{
        Serial.begin(115200);

        WiFi.begin("INSERT_YOUR_SSID_HERE", "INSERT_YOUR_PASS_HERE");
        while (WiFi.isConnected() == false) {
                delay(500);
                Serial.print(".");
        }
        Serial.println("Connected, getting location...");
        double lat, lng, acc;
        location.setGoogleKey(LOCATION_GOOGLE_KEY);
        // location.setClient(client); You can change the client if you wish (should be a secure client)
        if (location.getLocation(lat, lng, acc) == true)
                Serial.printf("Location success: %.6f, %.6f (%.2fm)\n", lat, lng, acc);
        else
                Serial.println("Location failed");
}

void loop()
{

}
