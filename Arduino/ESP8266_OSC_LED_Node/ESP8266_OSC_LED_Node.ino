/*---------------------------------------------------------------------------------------------
Intermedia Lab NodeMCU LED node
code by Halfdan Hauch Jensen halj@itu.dk


OSC LED node protocol
-------------------------------------------------
Address pattern: "IML_LED_NODE

Arguments:
- 0 (int): Pixel index to start from (included)
- 1 (int): Pixel to stop at (not included)
- 2 (int): red value between 0-255
- 3 (int): green value between 0-255
- 4 (int): blue value between 0-255
--------------------------------------------------------------------------------------------- */

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

#include<FastLED.h>
#define NUM_LEDS 60 // ## set nr of leds to use
#define DATA_PIN D3

char ssid[] = "*****";          // ## your network SSID (name)
char pass[] = "*****";         // ## your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const unsigned int localPort = 8888;        // ## local port to listen for UDP packets (here's where we send the packets)

OSCErrorCode error;

int ledRed = 0;               
int ledGreen = 0;
int ledBlue = 0;

CRGB leds[NUM_LEDS];

void setup() {

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
#ifdef ESP32
  Serial.println(localPort);
#else
  Serial.println(Udp.localPort());
#endif

}


void led(OSCMessage &msg) {

  // iterates and sets the pixels acording to the OSC message
  for (int pixelIterator = msg.getInt(0); pixelIterator < min(msg.getInt(1), NUM_LEDS); pixelIterator++){
    ledRed = msg.getInt(2);
    ledGreen = msg.getInt(3);
    ledBlue = msg.getInt(4);
    leds[pixelIterator] = CRGB(ledRed,ledGreen,ledBlue);
    Serial.println(pixelIterator);
    Serial.println(ledRed);
    Serial.println(ledGreen);
    Serial.println(ledBlue);
    Serial.println();
  }
  FastLED.show();
  
}

void loop() {

  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      msg.dispatch("/IML_LED_NODE", led); // calls the led function above when the address pattern is "/IML_LED_NODE"
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}
