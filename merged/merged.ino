#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>

#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

#define SS 18
#define RST 34
#define DI0 26
#define BAND 433E6 //915E6

Adafruit_ST7735 tft = Adafruit_ST7735(13, 25, 14, 12, 33);
using namespace websockets;
/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

String readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return "gg";
  }

  Serial.println("- read from file:");
  auto str = file.readString();
  file.close();
  return str;
}

const char* ssid = "........";
const char* password = "........";

WebServer server(80);
WebsocketsServer wsserver;

const int led = 13;

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", readFile(SPIFFS, "/index.html"));
  digitalWrite(led, 0);
}

void handleCSS() {
  digitalWrite(led, 1);
  server.send(200, "text/css", readFile(SPIFFS, "/style.css"));
  digitalWrite(led, 0);
}

void handleJS() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", readFile(SPIFFS, "/skript.js"));
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.println("");

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/style.css", handleCSS);

  server.on("/skript.js", handleJS);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  wsserver.listen(81);

  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
  /*display.drawString(5, 20, "LoRa Initializing OK!");
    display.display();*/
  tft.fillScreen(ST77XX_BLACK);


  tft.setRotation(3);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(250E3);
  LoRa.setCodingRate4(8);
}

int lastpacket;
int lastdisplay;
std::vector<WebsocketsClient> clients;
void onMessage(WebsocketsClient& client, WebsocketsMessage message) {

  if (message.length() > 0) {
    for (auto& c : clients) {
      c.send(String(WiFi.macAddress()) + " (myself): " + message.data());
    }
    LoRa.beginPacket() ;
    LoRa.print(String(WiFi.macAddress()) + ": " + message.data());
    LoRa.endPacket();
  }
  Serial.print("Message: ");
  Serial.println(message.data());
}
void handleallclients() {
  for (auto& client : clients) {
    /*while (client.available()) {
      WebsocketsMessage msg = client.readBlocking();

      // TODO: read incoming messages from LoRa and transfer the data to the phone
      // LoRa.readData()
      // client.send(whatever_data_is_read_from_lora)
      }*/
    client.poll();
  }
}
void loop(void) {
  server.handleClient();
  if (wsserver.poll()) {
    WebsocketsClient client = wsserver.accept();
    client.onMessage(onMessage);
    clients.push_back(client);
  }

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    lastpacket = millis();
    char* msg = (char*)malloc(packetSize + 1);
    for (int i = 0; i < packetSize; i++) {
      msg[i] = LoRa.read();
    }
    msg[packetSize] = '\0';
    for (auto& client : clients) {
      client.send(msg);
    }
    Serial.println(msg);
    free(msg);
  }
  handleallclients();
}
