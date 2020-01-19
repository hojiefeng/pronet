/* https://www.alictronix.com/archives/860
* Sketch uses 155594 bytes (14%) of program storage space. Maximum is 1044464 bytes.
* Global variables use 11612 bytes (3%) of dynamic memory, leaving 283300 bytes for local variables. Maximum is 294912 bytes.
*
*
* This is the code Jiefeng used in his setup to print the RSSI, SNR, and message of the LoRa signal on a OLED screen.
*
*/
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
//#include <SSD1306.h>
#include <Arduino.h>
//OLED pins to ESP32 GPIOs via this connecthin:
//OLED_SDA — GPIO4
//OLED_SCL — GPIO15
//OLED_RST — GPIO16
//SSD1306 display(0x3c, 4, 15);
// WIFI_LoRa_32 ports
// GPIO5 — SX1278’s SCK
// GPIO19 — SX1278’s MISO
// GPIO27 — SX1278’s MOSI
// GPIO18 — SX1278’s CS
// GPIO14 — SX1278’s RESET
// GPIO26 — SX1278’s IRQ(Interrupt Request)
#define SS 18
#define RST 34
#define DI0 26
#define BAND 433E6 //915E6
int counter = 0;

Adafruit_ST7735 tft = Adafruit_ST7735(13, 25, 14, 12, 33);
void setup() {
 pinMode(32, OUTPUT); //Send success, LED will bright 1 second
 pinMode(16, OUTPUT);
 digitalWrite(32, LOW); // set GPIO16 low to reset OLED
 delay(50);
 digitalWrite(32, HIGH);
 Serial.begin(115200);
 while (!Serial); //If just the the basic function, must connect to a computer
 // Initialising the UI will init the display too.
 /*display.init();
 display.flipScreenVertically();
 display.setFont(ArialMT_Plain_10);
 display.setTextAlignment(TEXT_ALIGN_LEFT);
 display.drawString(5, 5, "LoRa Sender");
 display.display();*/
 
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
 //delay(1000);
//LoRa.onReceive(onReceive);
}

int lastpacket = 0;
int lastdisplay = 0;
void onReceive(int packetSize) {
 //int packetSize = LoRa.parsePacket();
 Serial.println("RECV");
 if (packetSize) {
  lastpacket = millis();
  char* msg = (char*)malloc(packetSize+1);
  for(int i = 0;i<packetSize;i++){
    msg[i] = LoRa.read();
  }
  msg[packetSize] = '\0';
   /*while (LoRa.available()) {
       char receivedText = (char)LoRa.read();
       Serial.print(receivedText);
   }*/
   int rssi = LoRa.packetRssi();
   float snr = LoRa.packetSnr();
  long freqErr = LoRa.packetFrequencyError();
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(true);
  tft.setTextSize(3);
  tft.println(msg);
  tft.setTextSize(1);
  tft.printf("RSSI: %d\n",rssi);
  tft.printf("SNR: %f\n",snr);
  tft.printf("Err: %ld\n",freqErr);
   //Serial.println("RECV");
   //delay(1000);
 }

}

void loop() {
 //Serial.print("Sending packet: ");
 //Serial.println(counter); 
 int packetSize = LoRa.parsePacket();
 //Serial.println("RECV");
 if (packetSize) {
  lastpacket = millis();
  char* msg = (char*)malloc(packetSize+1);
  for(int i = 0;i<packetSize;i++){
    msg[i] = LoRa.read();
  }
  msg[packetSize] = '\0';
  Serial.println(msg);
   /*while (LoRa.available()) {
       char receivedText = (char)LoRa.read();
       Serial.print(receivedText);
   }*/
   int rssi = LoRa.packetRssi();
   float snr = LoRa.packetSnr();
  long freqErr = LoRa.packetFrequencyError();
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(true);
  tft.setTextSize(1);
  tft.println(msg);
  tft.printf("RSSI: %d\n",rssi);
  tft.printf("SNR: %f\n",snr);
  tft.printf("Err: %ld\n",freqErr);
   //Serial.println("RECV");
   //delay(1000);
 }
  
  //tft.fillScreen(ST77XX_BLACK);
  if(millis()-lastdisplay>=500) {
    tft.fillRect(75, 100, 50, 10, ST77XX_BLACK);
 tft.setCursor(0, 100);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(true);
 tft.setTextSize(1);
 tft.printf("Last Packet: %dms\n",millis()-lastpacket);
  lastdisplay = millis();
  }
 /*display.clear();
 display.setFont(ArialMT_Plain_16);
 display.drawString(3, 5, "Sending packet ");
 display.drawString(50, 30, String(counter));
 display.display();
 // send packet
 LoRa.beginPacket() ;
 LoRa.print("Hello ") ;
 LoRa.print(counter) ;
 LoRa.endPacket();
 counter++;
 digitalWrite(25, HIGH); // turn the LED on (HIGH is the voltage level)
 delay(1000); // wait for a second
 digitalWrite(25, LOW); // turn the LED off by making the voltage LOW
 delay(1000); // wait for a second
 delay(3000);
}
/* https://robotzero.one/heltec-wifi-lora-32/
* Sketch uses 146842 bytes (14%) of program storage space. Maximum is 1044464 bytes.
* Global variables use 11628 bytes (3%) of dynamic memory, leaving 283284 bytes for local variables. Maximum is 294912 bytes.
*/
} 
