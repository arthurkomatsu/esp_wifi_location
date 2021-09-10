#include <Arduino.h>
#include <esp_wifi_location.h>

#define LOCATION_GOOGLE_KEY "INSERT YOUR GOOGLE API KEY HERE"

void setup()
{
	//Starting serial for debugging purposes
	Serial.begin(115200);

	//Connecting to WiFi
	WiFi.begin("INSERT_YOUR_SSID_HERE", "INSERT_YOUR_PASS_HERE");
	while (WiFi.isConnected() == false) {
		delay(500);
		Serial.print(".");
	}

	//Getting location...
	Serial.println("Connected, getting location...");
	double lat, lng, acc;
	esp_wifi_location location(LOCATION_GOOGLE_KEY);
	// location.setGoogleKey(LOCATION_GOOGLE_KEY); //You can also change your google key here
	// location.setClient(client); You can change the client if you wish (should be a secure client)
	if (location.getLocation(lat, lng, acc) == true)
		Serial.printf("Location success: %.6f, %.6f (%.2fm)\n", lat, lng, acc);
	else
		Serial.println("Location failed");
}

void loop()
{

}
