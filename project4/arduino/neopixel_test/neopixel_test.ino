// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      7

int r[NUMPIXELS]; // array for each color value
int g[NUMPIXELS];
int b[NUMPIXELS];


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
int colorIterator = 0;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  colorIterator += 1;
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
  pixels.show();
//    for(int i=0;i<NUMPIXELS;i++){
//    pixels.setPixelColor(i, pixels.Color(0,0,0));
//  }
//  pixels.show();
  
  
}

void getSerial() {
  
  // Serial Comm code adapted from:
  // http://www.instructables.com/id/Ambient-Computer-Lights-using-an-Arduino-with-NeoP/
  
  
  if (Serial.available()>=19) {
    if (Serial.read() == 0xff){
      
      for (int x = 1; x < NUMPIXELS; x++){
        r[x] = Serial.read(); // read in the values from processing
        g[x] = Serial.read(); // in the same order we sent them
        b[x] = Serial.read();

        r[x] = constrain(r[x], 0, 255); // just incase any values slip away
        g[x] = constrain(g[x], 0, 255);
        b[x] = constrain(b[x], 0, 255);

      }
    }
  }
}
