#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "wifi-credentials.h"

bool ledOn = false;
int ledPin = 2;

ESP8266WebServer server(80);

void setup(void){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(ledPin, OUTPUT);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [](){
    ledOn = !ledOn;
    digitalWrite(ledPin, ledOn ? LOW : HIGH);
    Serial.println(ledOn ? "ON" : "OFF");
    server.send(200, "text/plain", ledOn ? "ON" : "OFF");
  });

  server.onNotFound([](){
    server.send(404, "text/plain", "404 not found");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
