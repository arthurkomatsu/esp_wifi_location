# ESP WiFi Location
An Arduino ESP8266 library to get geolocation using Google's API from nearby WiFi's stations.

## How does it work?
Google offers a [Geolocation API](https://developers.google.com/maps/documentation/geolocation/overview) service which returns an estimated location (latitude, longitude and accuracy) based on nearby Wifi stations. Each Wifi station has an unique MAC address associated with it. Since Wifi stations usually doesn't move much, Google can *almost* always get the estimated location right. If you wish to use the service, you must get an [Google API key](https://developers.google.com/maps/documentation/geolocation/get-api-key). Take a look at request limitations, you may have to pay to get more requests.

The geolocation service is a simple HTTPS POST request. This library uses the Wifi scan feature, which mounts the json payload and makes the POST to Google's API.

## Client
This library has the ability to change the Client (with the setClient function), so you can make the request using whatever communication module you want (i.e. WiFi, GSM, Ethernet). Since it's a HTTPS request, the client must be secure.

## Limitations
HTTPS POST uses significant heap memory and processing power from the ESP. For this library, you will need at least 10k of free heap memory to make the request, or else it will fail. Since the payload is also built within the heap memory, the more Wifis are scanned, the more memory it will consume. To workaround this problem, the library will scan a max of 10 Wifis (which is more than enough). This number can be changed by calling the setMaxWifisToScan() function.

## Dependencies
**esp_wifi_location** uses the following dependencies:
* [ArduinoHttpClient](https://github.com/arduino-libraries/ArduinoHttpClient)
* [ArduinJson](https://github.com/bblanchon/ArduinoJson)

## Usage
Looking at the [example](examples/location.cpp), it should be pretty straightforward.
