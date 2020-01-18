#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#define DEBUG
#define PERIOD_TO_PROBE_LORA 500 // Msg is read from LoRa every 500ms

const char* ssid = "solomonbstoner"; //Enter SSID
const char* password = "password2"; //Enter Password
using namespace websockets;


WebsocketsServer server;

void setup() {
  Serial.begin(115200);
  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for(int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
      Serial.print(".");
      delay(1000);
  }

#ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP
#endif
  server.listen(80);
#ifdef DEBUG
  Serial.print("Is server live? ");
  Serial.println(server.available());
#endif
}

void loop() {
  WebsocketsClient client = server.accept();
  // Phone is connected to the ESP32
  while (client.available()) {
    WebsocketsMessage msg = client.readBlocking();
    if (msg.length() > 0) {
       // TODO: Forward msg.data() to LoRa
    }
#ifdef DEBUG
    Serial.print("Message: ");
    Serial.println(msg.rawData());
#endif

  // TODO: read incoming messages from LoRa and transfer the data to the phone
  // LoRa.readData()
  // client.send(whatever_data_is_read_from_lora)
  }

}
