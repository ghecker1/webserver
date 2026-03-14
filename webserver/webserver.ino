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

char ssid[] = "sssssssssss";
char pass[] = "pppppppppppppppppppp";

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
  Serial.print("Set sleep mode: WIFI_MODEM_SLEEP");
  //Serial.println(getSleepMode());
  setSleepMode(WIFI_MODEM_SLEEP);
  //Serial.print("Sleep mode: ");
  //Serial.println(getSleepMode());
  /*
  Serial.println("Sleep modes: ");
  Serial.println(WIFI_NONE_SLEEP);
  Serial.println(WIFI_LIGHT_SLEEP);
  Serial.println(WIFI_MODEM_SLEEP);
  */

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
  Serial.println(millis());

  setup_wifi();

  Serial.println(millis());
  delay(3000);
  Serial.println(millis());
  Serial.println("ESP.deepSleep");
  delay(500);
  ESP.deepSleep(5e6);
  delay(500);
  Serial.println("ESP.deepSleep() failed. This code should not be reached.");
  delay(500);
}

void loop() {
  Serial.println("ESP.deepSleep() failed. This code should not be reached.");
  delay(1000);
}
