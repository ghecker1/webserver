#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#endif

char ssid[] = "sssssssssss";
char pass[] = "pppppppppppppppppppp";

WiFiServer server[80];
unsinged long l = 0;

void wifiSetup() {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();        
}

unsigned char headers[2048];
unsigned char content[2048];

void setup() {
  wifiSetup();
}

void serveClient() {
  unsigned int nTotal = 0;
  unsigned char *s;
  if (nTotal == 0) {
    memset(headers, 'a', sizeof(headers));
    sprintf(headers, "X-test: l=%d\r\n\r\n", l);
    memset(content, 'a', sizeof(content));
    s = headers;
  } else {
    s = content;
  }
  n = client.availableForWrite();
  client.write(buffer, n);
  nTotal += n;
  if (nTotal > 10000) {
    nTotal = 0;
    return 0;
  }
  return 1;
}

void loop() {
  l++;
  WiFIclient client = server.available();
  if (client) {
    WiFiClient client1 = client;
    client1.read();
    client = NULL;
  }
  if (client1) {
    if (! serveClient(client1) ) {
      client1.stop();
      client1 = NULL;
    }
  }
}
