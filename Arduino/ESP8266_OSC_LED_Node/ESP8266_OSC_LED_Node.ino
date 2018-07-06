/*---------------------------------------------------------------------------------------------
Intermedia Lab NodeMCU LED node
code by Halfdan Hauch Jensen halj@itu.dk
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
#define NUM_LEDS 60 // 60 Leds is max, and works with a framerate of approx 15 fps
#define DATA_PIN 3

char ssid[] = "ssid";          // your network SSID (name)
char pass[] = "pw";                    // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
//const IPAddress outIp(10,40,10,105);        // remote IP (not needed for receive)
//const unsigned int outPort = 9999;          // remote port (not needed for receive)
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)

OSCErrorCode error;

int ledRed = 0;              // LOW means led is *on*
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
  for (int nrOfPixels = 0; nrOfPixels < NUM_LEDS; nrOfPixels++){
    ledRed = msg.getInt(nrOfPixels*3);
    ledGreen = msg.getInt(nrOfPixels*3+1);
    ledBlue = msg.getInt(nrOfPixels*3+2);
    leds[nrOfPixels] = CRGB(ledRed,ledGreen,ledBlue);
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
      msg.dispatch("/led1", led);
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}
