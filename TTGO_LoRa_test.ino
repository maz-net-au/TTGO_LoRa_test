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
#define RST 23
#define DIO0 26

#define BAND 433E6
//
#define OLED_SDA 21
#define OLED_SCL 22 
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
  //initialize Serial Monitor
  Serial.begin(115200);
  
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA RECEIVER ");
  display.display();

  Serial.println("LoRa Duplex Test");
  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSyncWord(136383);
  LoRa.setTxPower(20);
  LoRa.enableCrc();  
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.println("Waiting for messages");
  display.display(); 
}

void loop() {
  if (nextSend == 0)
  {
    // Schedule a send in the future, either after we have just sent a message, or
    // when we first loop.
    nextSend = millis() + random(25000) + 5000; // 5 - 30 seconds from now.
  }
  else if (millis() > nextSend)
  {
    // Send a message once after the random time has elapsed.
    nextSend = 0;
    unsigned long now = millis();
    String outgoing = "The time is now " + String(now) + " since boot";
    LoRa.beginPacket();
    // LoRa.write(outgoing.length());
    LoRa.print(outgoing);
    LoRa.endPacket();
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Message sent @");
    display.println(now);
    display.invertDisplay(true);
    display.display();
  }
  else
  {
    // Idle and wait for a message most of the time. The screen will continue to say
    // "message sent" until we actually receive a packet.
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      Serial.print("Received packet ");
      String LoRaData = "";
      while (LoRa.available()) {
        LoRaData += LoRa.readString();
      }
      Serial.print(LoRaData);
  
      int rssi = LoRa.packetRssi();
      Serial.print(" with RSSI ");    
      Serial.println(rssi);

      display.clearDisplay();
      display.setCursor(0,0);
      display.println(LoRaData);
      display.println("");
      display.println("RSSI: " + String(LoRa.packetRssi()));
      display.println("Snr: " + String(LoRa.packetSnr()));
      display.invertDisplay(false);
      display.display();
    }
  }
}
