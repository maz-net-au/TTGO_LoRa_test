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

void setup() {
  Serial.begin(115200);
  Serial.println("");

  // using the arduino "Wire" library that is known to have issues
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);


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
  display.println("HI ROB");
  display.println("TEST MESSAGE");
  display.display();
  
  Serial.println("LoRa Sender Test");

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
//  LoRa.begin(868E6);
//  LoRa.setSpreadingFactor(12);
//  LoRa.setSignalBandwidth(125E3);
//  LoRa.setCodingRate4(8);
//  LoRa.setTxPower(20);
//  LoRa.enableCrc();
  
  // LoRa.begin(BAND);


  
}

void loop() {
  
}
