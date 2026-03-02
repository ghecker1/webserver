#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#endif

char ssid[] = "sssssssssss";
char pass[] = "pppppppppppppppppppp";

WiFiServer server(80);
WiFiClient *client;

unsigned long connect_time = 0;
unsigned long l = 0;
int headers_size = 0;
char headers[2048];
char content[2048];

void printScanResult(int networksFound)
{
  Serial.printf("%d network(s) found\n", networksFound);
  for (int i = 0; i < networksFound; i++)
  {
    Serial.printf("%d: %s, Ch:%d (%ddBm) %s", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
    Serial.println();
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  printScanResult(WiFi.scanNetworks());
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

  // Start the server
  server.begin();
  Serial.println(F("Server started"));

  // Print the IP address
  Serial.println(WiFi.localIP());
  delay(500);

  headers_size = sprintf(headers, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nContent-length: 102400\r\n\r\n");
}

int serveClient() {
  static unsigned int totalWritten = 0;
  static unsigned int total = 100 * 1024;
  static uint8_t status = 0;

  //Serial.println(F("Entering serveClient()"));
  int n = client->availableForWrite();
  if (status == 0) {
    if (n > headers_size) {
      Serial.println("Send headers");
      client->write(headers, headers_size);
      memset(content, 'a', sizeof(content));
      status = 1;
    } else {
      Serial.println("Waiting");
    }
  } else {
    if (totalWritten + n > total) {
      n = total - totalWritten;
    }
    client->write(content, n);
    totalWritten += n;
  }
  if (totalWritten >= total) {
    Serial.print(millis()-connect_time);
    Serial.println(F(" ms"));
    Serial.print(totalWritten);
    Serial.println(F(" bytes"));
    Serial.println(F("Complete"));
    totalWritten = 0;
    status = 0;
    return 0;
  }
  //Serial.println(F("Incomplete"));
  return 1;
}

void loop() {
  l++;
  if (!client && server.hasClient()) {
    client = new WiFiClient(server.accept());
    Serial.println(F("============ New connection ============"));
    connect_time = millis();
    //String req = client->readStringUntil('\r');
    while (client->available()) {
      // byte by byte is not very efficient
      client->read();
    }
    //client->print(F("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n"));
  }
  if (client) {
    if (! serveClient() ) {
      Serial.println(F("============ client->stop() ============"));
      client->stop();
      delete(client);
      Serial.println(F("============ client = NULL  ============"));
      client = NULL;
    }
  } else {
    delay(20);
  }
}
