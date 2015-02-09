/*
This file is part of B{Domogik} project (U{http://www.domogik.org}).
 
 License
 =======
 
 B{Domogik} is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 B{Domogik} is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Domogik. If not, see U{http://www.gnu.org/licenses}.
 
 Plugin purpose
 ==============
 
 This file is part of 'ar_rgb' hardware plugin
 
 Control RGB led strip with an arduino and xPL protocol
 
 This arduino program may be used without Domogik with any xPL project
 
 @author: Fritz <fritz.smh@gmail.com>, OverLoad <y.poilvert@geekinfo.fr>
 @copyright: (C) 2007-2014 Domogik project
 @license: GPL(v3)
 @organization: Domogik
 */

#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008


/***************** Network configuration ********************/

// Mac address : a mac address must be unique
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte broadCastIp[] = {
  255, 255, 255, 255};
// IP address : the value will depend on your network
EthernetClient client;

// Udp local port used (any value you want between 1024 and 65536)
unsigned int localPort = 3865;  
// xPL protocol port (keep default value unless you know what you are doing)
unsigned int xplPort = 3865;  

/***************** xPL configuration part *******************/

// Source. Format is <vendor id>-<device id>.<instance>

// Vendor id and device id should not be changed
// Instance should be the location of your arduino and RGB led strip : living, bedroom, etc
// Instance should only use letters or numbers! (no underscore, ...)
#define MY_SOURCE "arduino-rgb.salon"

// Name of device in body of xpl message
// This should be the same as previously defined instance
#define MY_DEVICE "ledstrip0"

// Maximal size of xPL messages that could be processed
// This size should not be reduce unless you are sure of what you are doing
#define MAX_XPL_MESSAGE_SIZE 165

// Heartbeat interval : time in minutes between 2 sends of a xpl hbeat message
#define HBEAT_INTERVAL 1

/******************** RGB configuration**********************/

// Define which PWM pins to use
#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 9

/******************* END OF CONFIGURATION *******************/


/********************* time management **********************/

int second = 0;
int lastHbeat = 0;

/*********************** Global vars ************************/

// current color
int isOff = true;
byte currentColor[] = {
  0x00, 0x00, 0x00};
float hue = 0;

// UDP related vars
EthernetUDP Udp;

//int packetSize = 0;      // received packet's size
//byte remoteIp[4];        // received packet's IP
//unsigned int remotePort; // received packet's port
byte packetBuffer[MAX_XPL_MESSAGE_SIZE];   // place to store received packet

// status
int result;

/********************* Set up arduino ***********************/
void setup() {
  // Wait before doing anything
  //delay(5000);

  // Serial debugging
  Serial.begin(115200);
  Serial.println(F("Setup arduino..."));

  // Ethernet initialisation
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    Serial.println(F("Rebooting..."));
    delay(1000);
    asm volatile ("  jmp 0"); 
  }
  // print your local IP address:
  Serial.print(F("My IP address: "));
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(F(".")); 
  }
  Serial.println();
  Ethernet.begin(mac, Ethernet.localIP());
  Udp.begin(localPort);

  // Set pin's mode
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Send a hbeat message on startup
  sendHbeat();
  setColorOff();
  Serial.println(F("Setup arduino finished."));
}





/*********************** Processing *************************/
void loop() {

  /**** pulse management ****/
  pulse();
  // Each second, make a pulse
  if (second == 1) {
    Serial.println(F("---- PULSE ----"));
    Serial.print(currentColor[0]);
    Serial.print(",");
    Serial.print(currentColor[1]);
    Serial.print(",");
    Serial.println(currentColor[2]);
    // Hbeat
    lastHbeat += 1;
    // Each N minute, send a hbeat xpl message
    if (lastHbeat == (60*HBEAT_INTERVAL)) {
      sendHbeat();
      lastHbeat = 0;
    }
  }

  /**** Udp listening ****/
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    // read the packet and get the senders IP address and the port number
    Udp.read(packetBuffer,MAX_XPL_MESSAGE_SIZE);

    // if message is full (not truncated), process it :
    if(abs(packetSize) < MAX_XPL_MESSAGE_SIZE) {
      Serial.println(F("Message of good size"));
      result = parseXpl(packetBuffer,abs(packetSize));
    }
    else {
      Serial.println(F("Message to big to be parsed"));
    }
  }
    analogWrite(RED_PIN, currentColor[0]);
    analogWrite(GREEN_PIN, currentColor[1]);
    analogWrite(BLUE_PIN, currentColor[2]);
}

