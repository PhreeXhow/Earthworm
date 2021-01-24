#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <SPI.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

const char* host = "Earthworm";

const int pin_ser_dat = D6;
const int pin_ser_clk = D7;
const int pin_reg_latch = D5;
const int pin_mstr_rst = D8;

volatile uint8_t controlStatus = 0;
volatile unsigned long millisfrom = 0;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void sendToShiftRegister(uint8_t value) {  
  Serial.print("Requested to send value ");
  Serial.print(value);
  Serial.println(" to Shift Register.");
  shiftOut(pin_ser_dat,pin_ser_clk, MSBFIRST, value);
  digitalWrite(pin_reg_latch, HIGH);
  delay(20);
  digitalWrite(pin_reg_latch, LOW);
}

void setup(void) {
  pinMode(pin_mstr_rst,OUTPUT);
  digitalWrite(pin_mstr_rst,LOW);
  Serial.begin(115200);
  Serial.println("Booting Sketch...");
  WiFiManager wifiManager;
  Serial.println("Starting WiFi OR Starting WiFi Config Manager...");
  wifiManager.autoConnect("ConfigEarthworm");
  Serial.println("Starting mDNS Service...");
  MDNS.begin(host);
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n\n\n", host);
  sendToShiftRegister(random(255));
  digitalWrite(pin_mstr_rst, HIGH);
  millisfrom = millis();
}

void loop(void) {
  Serial.println("In main loop, doing main loop stuff...");
  httpServer.handleClient();
  MDNS.update();
  controlStatus = random(255);
  if (millisfrom + 750 > millis()) {
    sendToShiftRegister(controlStatus);
  }
}