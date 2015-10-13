#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


/* OSC */
// MESSAGE PROTOCOL OBJECT
#include <OscSerial.h>

// tile support libraries
#include <EthernetUdp.h>
#include <SPI.h>  




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



OscSerial oscSerial;
long timer;

void setup() {
  Serial.begin(9600);
  oscSerial.begin(Serial); 
  //pinMode(13, OUTPUT);
  
  pixels.begin(); // This initializes the NeoPixel library.
  
  r[0] = 255;
  g[0] = 0;
  b[0] = 0;
  
  r[1] = 0;
  g[1] = 140;
  b[1] = 40;
  
}

void loop() {
  // send a message every 100 ms
  // avoid using delay() since it just blocks everything  
//  long now = millis();
//  if (now-timer > 100) {
//    
//    OscMessage msg("/helloFromArduino");
//    msg.add(0); // <-- this could be any data 
//    oscSerial.send(msg);       
//    timer = now;
//  }
  
  // important! 
  oscSerial.listen();
  
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(r[i],g[i],b[i]));
  }
  pixels.show();
  
  delay(1);
  
}

void oscEvent(OscMessage &m) { // *note the & before msg
  // receive a message 
  m.plug("/colors", myFunction); 
}

void myFunction(OscMessage &m) {  // *note the & before msg
  // getting to the message data
 //digitalWrite(13, HIGH);
 
 //int intNum = 0;
 
 for (int pix=0; pix<NUMPIXELS; pix++){
 
  r[pix] = m.getInt(0+3*pix); 
  g[pix] = m.getInt(1+3*pix); 
  b[pix] = m.getInt(2+3*pix); 
  
 }
  
//  r[1] = m.getInt(3); 
//  g[1] = m.getInt(4); 
//  b[1] = m.getInt(5); 
//
//  r[2] = m.getInt(6); 
//  g[2] = m.getInt(7); 
//  b[2] = m.getInt(8);   
  
  //digitalWrite(13, LOW);
  
  //int value = m.getInt(0); 
  //if (value == 0) digitalWrite(13, LOW);
  //if (value == 1) digitalWrite(13, HIGH);
}
