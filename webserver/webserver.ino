// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright 2016-2026 Hristo Gochkov, Mathieu Carbou, Emil Muratov, Will Miles

//
// Query and send headers
//

#include <Arduino.h>
#if defined(ESP32) || defined(LIBRETINY)
#include <AsyncTCP.h>
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
#include <RPAsyncTCP.h>
#include <WiFi.h>
#endif

#include <ESPAsyncWebServer.h>

static AsyncWebServer server(80);

char ssid[] = "sssssssssss";
char pass[] = "pppppppppppppppppppp";

static const char *htmlContent PROGMEM = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Sample HTML</title>
</head>
<body>
    <h1>Hello, World!</h1>
    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin euismod, purus a euismod
    rhoncus, urna ipsum cursus massa, eu dictum tellus justo ac justo. Quisque ullamcorper
    arcu nec tortor ullamcorper, vel fermentum justo fermentum. Vivamus sed velit ut elit
    accumsan congue ut ut enim. Ut eu justo eu lacus varius gravida ut a tellus. Nulla facilisi.
    Integer auctor consectetur ultricies. Fusce feugiat, mi sit amet bibendum viverra, orci leo
    dapibus elit, id varius sem dui id lacus.</p>
    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin euismod, purus a euismod
    rhoncus, urna ipsum cursus massa, eu dictum tellus justo ac justo. Quisque ullamcorper
    arcu nec tortor ullamcorper, vel fermentum justo fermentum. Vivamus sed velit ut elit
    accumsan congue ut ut enim. Ut eu justo eu lacus varius gravida ut a tellus. Nulla facilisi.
    Integer auctor consectetur ultricies. Fusce feugiat, mi sit amet bibendum viverra, orci leo
    dapibus elit, id varius sem dui id lacus.</p>
    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin euismod, purus a euismod
    rhoncus, urna ipsum cursus massa, eu dictum tellus justo ac justo. Quisque ullamcorper
    arcu nec tortor ullamcorper, vel fermentum justo fermentum. Vivamus sed velit ut elit
    accumsan congue ut ut enim. Ut eu justo eu lacus varius gravida ut a tellus. Nulla facilisi.
    Integer auctor consectetur ultricies. Fusce feugiat, mi sit amet bibendum viverra, orci leo
    dapibus elit, id varius sem dui id lacus.</p>
    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin euismod, purus a euismod
    rhoncus, urna ipsum cursus massa, eu dictum tellus justo ac justo. Quisque ullamcorper
    arcu nec tortor ullamcorper, vel fermentum justo fermentum. Vivamus sed velit ut elit
    accumsan congue ut ut enim. Ut eu justo eu lacus varius gravida ut a tellus. Nulla facilisi.
    Integer auctor consectetur ultricies. Fusce feugiat, mi sit amet bibendum viverra, orci leo
    dapibus elit, id varius sem dui id lacus.</p>
    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin euismod, purus a euismod
    rhoncus, urna ipsum cursus massa, eu dictum tellus justo ac justo. Quisque ullamcorper
    arcu nec tortor ullamcorper, vel fermentum justo fermentum. Vivamus sed velit ut elit
    accumsan congue ut ut enim. Ut eu justo eu lacus varius gravida ut a tellus. Nulla facilisi.
    Integer auctor consectetur ultricies. Fusce feugiat, mi sit amet bibendum viverra, orci leo
    dapibus elit, id varius sem dui id lacus.</p>
    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin euismod, purus a euismod
    rhoncus, urna ipsum cursus massa, eu dictum tellus justo ac justo. Quisque ullamcorper
    arcu nec tortor ullamcorper, vel fermentum justo fermentum. Vivamus sed velit ut elit
    accumsan congue ut ut enim. Ut eu justo eu lacus varius gravida ut a tellus. Nulla facilisi.
    Integer auctor consectetur ultricies. Fusce feugiat, mi sit amet bibendum viverra, orci leo
    dapibus elit, id varius sem dui id lacus.</p>
    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin euismod, purus a euismod
    rhoncus, urna ipsum cursus massa, eu dictum tellus justo ac justo. Quisque ullamcorper
    arcu nec tortor ullamcorper, vel fermentum justo fermentum. Vivamus sed velit ut elit
    accumsan congue ut ut enim. Ut eu justo eu lacus varius gravida ut a tellus. Nulla facilisi.
    Integer auctor consectetur ultricies. Fusce feugiat, mi sit amet bibendum viverra, orci leo
    dapibus elit, id varius sem dui id lacus.</p>
    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin euismod, purus a euismod
    rhoncus, urna ipsum cursus massa, eu dictum tellus justo ac justo. Quisque ullamcorper
    arcu nec tortor ullamcorper, vel fermentum justo fermentum. Vivamus sed velit ut elit
    accumsan congue ut ut enim. Ut eu justo eu lacus varius gravida ut a tellus. Nulla facilisi.
    Integer auctor consectetur ultricies. Fusce feugiat, mi sit amet bibendum viverra, orci leo
    dapibus elit, id varius sem dui id lacus.</p>
</body>
</html>
)";

static const size_t htmlContentLength = strlen_P(htmlContent);

void dumpFreeMemory(char *msg) {
  Serial.printf("%10d: %20s: ESP.getFreeHeap: %6d", millis(), msg, ESP.getFreeHeap());
  Serial.println();
}

/*

WiFiEventHandler  onStationModeConnected (std::function< void(const WiFiEventStationModeConnected &)>)
WiFiEventHandler  onStationModeDisconnected (std::function< void(const WiFiEventStationModeDisconnected &)>)

WiFiEventHandler  onStationModeAuthModeChanged (std::function< void(const WiFiEventStationModeAuthModeChanged &)>)
WiFiEventHandler  onStationModeGotIP (std::function< void(const WiFiEventStationModeGotIP &)>)

WiFiEventHandler  onStationModeDHCPTimeout (std::function< void(void)>)

WiFiEventHandler  onSoftAPModeStationConnected (std::function< void(const WiFiEventSoftAPModeStationConnected &)>)
WiFiEventHandler  onSoftAPModeStationDisconnected (std::function< void(const WiFiEventSoftAPModeStationDisconnected &)>)

*/

WiFiEventHandler connectedEventHandler;
WiFiEventHandler disconnectedEventHandler;
WiFiEventHandler gotIpEventHandler;
WiFiEventHandler AuthModeChangedEventHandler;
WiFiEventHandler DHCPTimeoutEventHandler;

void onStationModeConnected(const WiFiEventStationModeConnected &event) {
  Serial.println("onStationModeConnected");
}

void onStationModeDisconnected(const WiFiEventStationModeDisconnected &event) {
  Serial.println("onStationModeDisconnected");
}

void onStationModeGotIP(const WiFiEventStationModeGotIP &event) {
  Serial.print("onStationModeGotIP, IP: ");
  Serial.println(WiFi.localIP());
}

void onStationModeAuthModeChanged(const WiFiEventStationModeAuthModeChanged &event) {
  Serial.println("onStationModeAuthModeChanged");
}

void onStationModeDHCPTimeout(void) {
  Serial.println("onStationModeDHCPTimeout");
}

void setup_wifi() {

  // setSleepMode (WiFiSleepType_t type, int listenInterval=0)
  Serial.print("Sleep mode: ");
  Serial.println(getSleepMode());
  setSleepMode(WIFI_MODEM_SLEEP);
  Serial.print("Sleep mode: ");
  Serial.println(getSleepMode());
  Serial.println("Sleep modes: ");
  Serial.println(WIFI_NONE_SLEEP);
  Serial.println(WIFI_LIGHT_SLEEP);
  Serial.println(WIFI_MODEM_SLEEP);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);

  // event handler
  connectedEventHandler = WiFi.onStationModeConnected(onStationModeConnected);
  disconnectedEventHandler = WiFi.onStationModeDisconnected(onStationModeDisconnected);
  gotIpEventHandler = WiFi.onStationModeGotIP(onStationModeGotIP);
  AuthModeChangedEventHandler = WiFi.onStationModeAuthModeChanged(onStationModeAuthModeChanged);
  DHCPTimeoutEventHandler = WiFi.onStationModeDHCPTimeout(onStationModeDHCPTimeout);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

  // Print the IP address
  Serial.println(WiFi.localIP());
}


void setup() {
  Serial.begin(115200);

  setup_wifi();
/*
#if ASYNCWEBSERVER_WIFI_SUPPORTED
  WiFi.mode(WIFI_AP);
  WiFi.softAP("esp-captive");
#endif
*/

  //
  // curl -v http://192.168.4.1
  //
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    dumpFreeMemory("/ initial");
    //List all collected headers
    int headers = request->headers();
    int i;
    for (i = 0; i < headers; i++) {
      const AsyncWebHeader *h = request->getHeader(i);
      Serial.printf("HEADER[%s]: %s", h->name().c_str(), h->value().c_str());
      Serial.println();
    }

    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Hello World!");

    //Add header to the response
    response->addHeader("Server", "ESP Async Web Server");

    //Add multiple headers with the same name
    response->addHeader("Set-Cookie", "sessionId=38afes7a8", false);
    response->addHeader("Set-Cookie", "id=a3fWa; Max-Age=2592000", false);
    response->addHeader("Set-Cookie", "qwerty=219ffwef9w0f; Domain=example.com", false);

    //Remove specific header
    response->removeHeader("Set-Cookie", "sessionId=38afes7a8");

    //Remove all headers with the same name
    response->removeHeader("Set-Cookie");

    dumpFreeMemory("/ before request->send(response)");
    request->send(response);
    dumpFreeMemory("/ after request->send(response)");
  });


  // from ESPAsyncWebServer : examples/FlashResponse/FlashResponse.ino

  server.on("/fromflash", HTTP_GET, [](AsyncWebServerRequest *request) {
    dumpFreeMemory("/fromflash initial");
    // need to cast to uint8_t*
    // if you do not, the const char* will be copied in a temporary String buffer
    request->send(200, "text/html", (uint8_t *)htmlContent, htmlContentLength);
    dumpFreeMemory("/fromflash after send");
  });


  // from ESPAsyncWebServer : examples/ChunkResponse/ChunkResponse.ino with caching removed

  server.on("/chunked", HTTP_GET, [](AsyncWebServerRequest *request) {

    dumpFreeMemory("/chunked initial");

    AsyncWebServerResponse *response = request->beginChunkedResponse("text/html", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
      Serial.printf("%u / %u / %u", index, htmlContentLength, maxLen);
      Serial.println();
      dumpFreeMemory("/chunked callback");

      // finished ?
      if (htmlContentLength <= index) {
        Serial.println("finished");
        return 0;
      }

      // serve a maximum of 256 or maxLen bytes of the remaining content
      // this small number is specifically chosen to demonstrate the chunking
      // DO NOT USE SUCH SMALL NUMBER IN PRODUCTION
      // Reducing the chunk size will increase the response time, thus reducing the server's capacity in processing concurrent requests
      const int chunkSize = min((size_t)64, min(maxLen, htmlContentLength - index));
      Serial.printf("sending: %u", chunkSize);
      Serial.println();


      memcpy(buffer, htmlContent + index, chunkSize);

      return chunkSize;
    });

    dumpFreeMemory("/chunked before request->send(response)");
    request->send(response);
    dumpFreeMemory("/chunked after request->send(response)");
  });

  server.begin();
}

void loop() {
  //Sleep in the loop task to not keep the CPU busy
  delay(1000);
  //dumpFreeMemory("loop()");
}
