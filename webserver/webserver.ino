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

unsigned long l = 0;
char headers[2048];
char content[2048];

void setup() {
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
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
}

int serveClient() {
  static unsigned int nTotal = 0;
  char *s = NULL;
  if (nTotal == 0) {
    memset(headers, 'a', sizeof(headers));
    sprintf(headers, "X-test: l=%d\r\n\r\n", l);
    memset(content, 'a', sizeof(content));
    s = headers;
  } else {
    s = content;
  }
  int n = client->availableForWrite();
  client->write(s, n);
  nTotal += n;
  if (nTotal > 10000) {
    nTotal = 0;
    return 0;
  }
  return 1;
}

void loop() {
  l++;
  if (server.hasClient()) {
    client = new WiFiClient(server.accept());
    client->read();
  }
  if (client) {
    if (! serveClient() ) {
      client->stop();
      delete(client);
      client = NULL;
    }
  }
}
