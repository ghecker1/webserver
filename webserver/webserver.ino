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


// https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html

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
  //Serial.print("Set sleep mode: WIFI_MODEM_SLEEP");

  Serial.print(millis()); Serial.print(": Sleep mode: "); Serial.println(WiFi.getSleepMode());

  WiFi.setSleepMode(WIFI_MODEM_SLEEP);

  Serial.print(millis()); Serial.print(": Sleep mode: "); Serial.println(WiFi.getSleepMode());

  /*
  Serial.println("Sleep modes: ");
  Serial.println(WIFI_NONE_SLEEP);
  Serial.println(WIFI_LIGHT_SLEEP);
  Serial.println(WIFI_MODEM_SLEEP);
  */

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(millis()); Serial.print(F(": Connecting to ")); Serial.println(ssid);

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
  Serial.print(millis()); Serial.println(F(": WiFi connected"));

  // Print the IP address
  Serial.print(millis()); Serial.println(WiFi.localIP());
}

void deepsleep() {
  //Serial.println("WiFi.disconnect()");
  //WiFi.disconnect();
  //WiFi.mode(WIFI_OFF);

  Serial.print(millis()); Serial.println(": ESP.deepSleep");
  delay(500);
  ESP.deepSleep(5e6);
  delay(500);
  Serial.print(millis()); Serial.println(": ESP.deepSleep() failed. This code should not be reached.");
  delay(500);
}

bool _deepsleep = false;

void setup() {
  Serial.begin(115200);
  Serial.println(millis());

  setup_wifi();

  if (_deepsleep) {
    deepsleep();
  }
}

void loop() {
  //Serial.print(millis()); Serial.println(": loop()");
  delay(10);
}
