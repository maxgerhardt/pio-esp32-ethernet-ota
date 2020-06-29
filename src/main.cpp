/*

 This example polls for sketch updates over Ethernet, sketches
 can be updated by selecting a network port from within
 the Arduino IDE: Tools -> Port -> Network Ports ...

 Circuit:
 * W5100, W5200 or W5500 Ethernet shield attached

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 modified 16 January 2017
 by Sandeep Mistry
 Ethernet version August 2018
 by Juraj Andrassy
 */
 
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoOTA.h>

//#define Serial SerialUSB

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

unsigned long last_time = 0;

#ifndef LED_BUILTIN
//red LED on my esp32 dev board is on IO2
#define LED_BUILTIN 2
#endif
bool led_state = false;

void setup() {
  //Initialize serial:
  Serial.begin(115200);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);

  //voltage stablilizer lol
  delay(1000);

  SPI.begin();
  Ethernet.init(5);
  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

  // start the OTEthernet library with internal (flash) based storage
  ArduinoOTA.begin(Ethernet.localIP(), "Arduino", "password", InternalStorage);
}

void loop() {
  // check for updates
  ArduinoOTA.poll();
  //constantly maintain DHCP ip in the long run
  Ethernet.maintain();
  //every second, print link status and toggle LED
  if(millis() - last_time >= 1000) {
	  last_time = millis();
	  auto link = Ethernet.linkStatus();
	  Serial.print("[V2] Link status: ");
	  switch (link) {
		case Unknown:
		  Serial.println("Unknown");
		  break;
		case LinkON:
		  Serial.println("ON");
		  break;
		case LinkOFF:
		  Serial.println("OFF");
		  break;
	  }
	  //toggle
	  led_state = !led_state;
	  digitalWrite(LED_BUILTIN, led_state ? HIGH : LOW);
  }
}

