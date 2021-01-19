#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

const char* host = "Earthworm";

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup(void) {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFiManager wifiManager;
  Serial.println("Starting WiFi OR Starting WiFi Config Manager...");
  wifiManager.autoConnect("ConfigEarthworm");
  Serial.println("Starting mDNS Service...");
  MDNS.begin(host);
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\nThis Service allows espota to send firmware over WiFi\n\n\n", host);
}

void loop(void) {
  Serial.println("In main loop, doing main loop stuff...");
  httpServer.handleClient();
  MDNS.update();
}