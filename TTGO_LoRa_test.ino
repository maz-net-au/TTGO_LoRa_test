/*
Module https://www.aliexpress.com/item/4000396836096.html
RNT using the old lilygo board https://randomnerdtutorials.com/ttgo-lora32-sx1276-arduino-ide/
RNT LoRa basics rfm95 tranceiever and an esp32 https://randomnerdtutorials.com/esp32-lora-rfm95-transceiver-arduino-ide/
Discussion about max range parameters https://github.com/sandeepmistry/arduino-LoRa/issues/90
LoRa lib https://github.com/sandeepmistry/arduino-LoRa

 */

#include <SPI.h>
#include <LoRa.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// not sure about these
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define BAND 433E6
//
//#define OLED_SDA 4
//#define OLED_SCL 15 
//#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64

#define REG_PA_CONFIG 0x09
#define REG_PA_DAC 0x4D
#define PA_DAC_HIGH 0x87
#define REG_LNA 0x0c
#define REG_MODEM_CONFIG_1 0x1d
#define REG_MODEM_CONFIG_2 0x1e
#define REG_MODEM_CONFIG_3 0x26
#define REG_OP_MODE 0x01
#define MODE_LONG_RANGE_MODE 0x80
#define MODE_SLEEP 0x00
#define MODE_STDBY 0x01

unsigned long nextSend = 0;

  // using the arduino "Wire" library that is known to have issues
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup() {
  Serial.begin(115200);
  Serial.println("");

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0); 

//  LoRa.writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
//  delay(10);
//  LoRa.writeRegister(REG_PA_CONFIG, 0b11111111); // That's for the transceiver
//  LoRa.writeRegister(REG_PA_DAC, PA_DAC_HIGH); // That's for the transceiver
//  LoRa.writeRegister(REG_LNA, 00); // TURN OFF LNA FOR TRANSMIT
//  uint8_t BW = 0x48, SF = 0xc4, AGC = 0x0c; // SF 12 BW 125KHz AGC
//  Serial.println("SF 0x" + String(SF, HEX) + "; BW: 0x" + String(BW, HEX) + "; AGC: 0x" + String(AGC, HEX));
//  LoRa.writeRegister(REG_MODEM_CONFIG_1, BW);
//  LoRa.writeRegister(REG_MODEM_CONFIG_2, SF);
//  LoRa.writeRegister(REG_MODEM_CONFIG_3, AGC);
//  delay(10);
//  LoRa.writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
//  delay(10);
//  
//
  LoRa.setPins(SS, RST, DIO0);
  int res = LoRa.begin(BAND);
  display.println("start radio " + String(res));
  display.display();
  LoRa.setSyncWord(136383);
  LoRa.setTxPower(20);
  LoRa.enableCrc();  
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  display.println("Waiting for messages");
  display.display();
}

void loop() {
  // if we don't know when we're sending, pick a time in the future
  if(nextSend == 0)
  {
    nextSend = millis() + random(55000) + 5000; // sometime between 5 and 60 seconds from now
  }
  // check if there's a packet waiting
  // read and display it
  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
  if(incoming != "")
  {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(incoming);
    display.println("");
    display.println("RSSI: " + String(LoRa.packetRssi()));
    display.println("Snr: " + String(LoRa.packetSnr()));
    display.invertDisplay(false);
    display.display();
  }

  // else, if the time is up, make and send a packet
  if(millis() > nextSend)
  {
    nextSend = 0;
    unsigned long now = millis();
    String outgoing = "The time is now " + String(now) + " since boot";
    LoRa.beginPacket();
    LoRa.write(outgoing.length());
    LoRa.print(outgoing);
    LoRa.endPacket();
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Message sent @");
    display.println(now);
    display.invertDisplay(true);
    display.display();
  }
}
