#include "esp_wifi_location.h"

// ----------------------------- PUBLIC FUNCTIONS ------------------------------
bool esp_wifi_location::getLocation(double &lat, double &lng, double &acc)
{
        debugLoc("Begin");

        //Check if there is a Google Key
        if (google_key == "") {
                debugLoc("Error! No Google Key");
                return false;
        }

        //Check if there is enough HEAP
        if (ESP.getFreeHeap() < 10000) {
                debugLoc("Aborting! Not enough Heap!");
                return false;
        }

        //Getting location
        DynamicJsonDocument json(1024);
        String payload = scan_wifis(json);
        json.clear();
        bool ok = postToGoogle(json, payload, lat, lng, acc);
        debugLoc("End");
        return ok;
}

void esp_wifi_location::setMaxWifisToScan(uint8_t max_wifis_scan)
{
        this->max_wifis_scan = max_wifis_scan;
}

void esp_wifi_location::setGoogleKey(String google_key)
{
        this->google_key = google_key;
}

void esp_wifi_location::setClient(Client *client)
{
        this->client = client;
}

// ----------------------------- PRIVATE FUNCTIONS -----------------------------
bool esp_wifi_location::postToGoogle(DynamicJsonDocument &json, String payload,
                                          double &lat, double &lng, double &acc)
{
        //Setting client
        Client *use_client = this->client;
        if (use_client == NULL) {
                debugLoc("No client specified, using WiFiSecure");
                use_client = (Client *) &this->wifi_client_secure;
                wifi_client_secure.setFingerprint(google_fingerprint);
                wifi_client_secure.setBufferSizes(512, 512);
        }

        HttpClient http(*use_client, google_server, google_port);

        //Sending request
        debugLoc("Making POST request to Google...");
        debugLoc("Payload (%d) = %s", payload.length(), payload.c_str());
        http.connectionKeepAlive();
        int ret = http.post(google_resource + google_key, "application/json",
                                                                       payload);
        if (ret != HTTP_SUCCESS) {
                acc = 0.0, lat = 0.0, lng = 0.0;
                http.stop();
                debugLoc("POST failed! Check connection!");
                return false;
        }

        //Waiting response
        debugLoc("Waiting response...");
        ret = http.responseStatusCode();
        String resp = http.responseBody();
        debugLoc("Finished! Status code: %d, Reponse: %s", ret, resp.c_str());

        //Checking if response was HTTP_OK
        if (ret != 200) {
                acc = 0.0, lat = 0.0, lng = 0.0;
                debugLoc("Http response failed!");
                http.stop();
                return false;
        }

        //Parsing payload
	auto error = deserializeJson(json, resp);
	if (error) {
		debugLoc("Json parer error: %s", error.c_str());
                http.stop();
		return false;
	}

        acc = json["accuracy"]        | 0.0;
        lat = json["location"]["lat"] | 0.0;
        lng = json["location"]["lng"] | 0.0;

        //Disconnecting from the server
        http.stop();
        debugLoc("(lat, lng) = (%.8f, %.8f), acc = %.2fm", lat, lng, acc);
        return lat != 0.0 && lng != 0.0 && acc != 0.0;
}

String esp_wifi_location::scan_wifis(DynamicJsonDocument &json)
{
        debugLoc("Scan WiFis begin");

        //Mount payload
        json["considerIp"] = "false";

        //Scanning Wifis...
        WiFi.mode(WIFI_STA);
        int n = WiFi.scanNetworks();
        if (n >= (int) max_wifis_scan)
                n = max_wifis_scan;

        //Adding scanned wifis to payload
        for (int i = 0; i < n; i++) {
                debugLoc("----> Scanned %s", WiFi.SSID(i).c_str());

                //Convert bssid to mac string
                uint8_t *m = WiFi.BSSID(i);
                static char macStr[18] = {0};
                sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
                                            m[0], m[1], m[2], m[3], m[4], m[5]);
                json["wifiAccessPoints"][i]["macAddress"] = macStr;
                json["wifiAccessPoints"][i]["signalStrength"] = WiFi.RSSI(i);
                json["wifiAccessPoints"][i]["channel"] = WiFi.channel(i);
        }
        debugLoc("Scan end");
        WiFi.scanDelete();

        //Finish payload
        String payload;
        serializeJson(json, payload);
        return payload;
}
