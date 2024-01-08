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

void handleMessage(AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  String message = String();
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    message = (char*)data;
  }
  Serial.printf("Client #%u says: %s\n", client->id(), message.c_str());
  if(message == "checked") {
    digitalWrite(LED_BUILTIN,LOW);
    builtin_state = message;
    ws.textAll(builtin_state);
    Serial.println("Built-in LED is on.");
  } else if(message == "unchecked") {
      digitalWrite(LED_BUILTIN,HIGH);
      builtin_state = message;
      ws.textAll(builtin_state);
      Serial.println("Built-in LED is off.");
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      ws.text(client->id(), builtin_state);
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleMessage(client, type, arg, data, len);
      break;
    case WS_EVT_PONG:
      Serial.printf("Pong from client #%u\n", client->id());
      break;
    case WS_EVT_ERROR:
      Serial.printf("Error from client #%u\n", client->id());
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
  server.on("/",HTTP_GET,[] (AsyncWebServerRequest *request){
      Serial.println("Fetch the main page.");
      request->send(LittleFS,"./index.html",String(),false,processor);
    });
  server.begin();
}

void loop() {
  ws.cleanupClients();
}