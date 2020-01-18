/* https://www.alictronix.com/archives/860
* Sketch uses 155594 bytes (14%) of program storage space. Maximum is 1044464 bytes.
* Global variables use 11612 bytes (3%) of dynamic memory, leaving 283300 bytes for local variables. Maximum is 294912 bytes.
*/
#include <SPI.h>
#include <LoRa.h>
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
void setup() {
 pinMode(25, OUTPUT); //Send success, LED will bright 1 second
 pinMode(16, OUTPUT);
 digitalWrite(16, LOW); // set GPIO16 low to reset OLED
 delay(50);
 digitalWrite(16, HIGH);
 Serial.begin(115200);
 while (!Serial); //If just the the basic function, must connect to a computer
 // Initialising the UI will init the display too.
 /*display.init();
 display.flipScreenVertically();
 display.setFont(ArialMT_Plain_10);
 display.setTextAlignment(TEXT_ALIGN_LEFT);
 display.drawString(5, 5, "LoRa Sender");
 display.display();*/
 SPI.begin(5, 19, 27, 18);
 LoRa.setPins(SS, RST, DI0);
 Serial.println("LoRa Sender");
 if (!LoRa.begin(BAND)) {
   Serial.println("Starting LoRa failed!");
   while (1);
 }
 Serial.println("LoRa Initial OK!");
 LoRa.setTxPower(20);
 LoRa.setSpreadingFactor(12);
 LoRa.setSignalBandwidth(250E3);
 LoRa.setCodingRate4(8);
 /*display.drawString(5, 20, "LoRa Initializing OK!");
 display.display();*/
 //delay(2000);
}
char str[] = "test message";
void loop() {
 //Serial.print("Sending packet: ");
 //Serial.println(counter);
 char* pch;
  pch = strtok (str,"\n");
  while (pch != NULL)
  {
    Serial.printf ("%s\n",pch);
 LoRa.beginPacket() ;
 LoRa.print(pch) ;
 LoRa.endPacket();
    pch = strtok (NULL, "\n");
    delay(1000);
  }

 counter++;
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
