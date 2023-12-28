#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

const char * ssid = "MikroTik";
const char * pass = "1953iarnagrea";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

String builtin_state;

void initLittleFFS() {
  while (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
    delay(1000);
  }
  Serial.println("SPIFFS mounted successfully");
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      Serial.printf("We've got some data from client #%u", client->id());
      break;
    case WS_EVT_PONG:
      Serial.printf("Pong from client #%u", client->id());
      break;
    case WS_EVT_ERROR:
      Serial.printf("Error from client #%u", client->id());
      break;
  }
}

String processor(const String& var) {
  if(var == "PLACEHOLDER_BUILTIN")
    return builtin_state;
  return String();
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  initLittleFFS();
  Serial.printf("\nConnecting to %s ", ssid);
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\nConnected to " + WiFi.localIP().toString() + ".\n");
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.on("/on",HTTP_GET,[] (AsyncWebServerRequest *request){
    digitalWrite(LED_BUILTIN,LOW);
    builtin_state = "checked";
    Serial.println("Built-in LED is on.");
    request->send(200,"text/plain","Built-in LED is on.");
  });
  server.on("/off",HTTP_GET,[] (AsyncWebServerRequest *request){
      digitalWrite(LED_BUILTIN,HIGH);
      builtin_state = "unchecked";
      Serial.println("Built-in LED is off.");
      request->send(200,"text/plain","Built-in LED is off.");
    });
  server.on("/",HTTP_GET,[] (AsyncWebServerRequest *request){
      Serial.println("Fetch the main page.");
      request->send(LittleFS,"./index.html",String(),false,processor);
    });
  server.begin();
}

void loop() {

}