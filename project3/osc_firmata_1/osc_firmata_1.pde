/*
arduino_pwm

Demonstrates the control of analog output (PWM) pins of an Arduino board
running the StandardFirmata firmware.  Moving the mouse horizontally across
the sketch changes the output on pins 9 (value is proportional to the mouse
X coordinate) and 11 (inversely porportional to mouse X).

To use:
* Using the Arduino software, upload the StandardFirmata example (located
  in Examples > Firmata > StandardFirmata) to your Arduino board.
* Run this sketch and look at the list of serial ports printed in the
  message area below. Note the index of the port corresponding to your
  Arduino board (the numbering starts at 0).  (Unless your Arduino board
  happens to be at index 0 in the list, the sketch probably won't work.
  Stop it and proceed with the instructions.)
* Modify the "arduino = new Arduino(...)" line below, changing the number
  in Arduino.list()[0] to the number corresponding to the serial port of
  your Arduino board.  Alternatively, you can replace Arduino.list()[0]
  with the name of the serial port, in double quotes, e.g. "COM5" on Windows
  or "/dev/tty.usbmodem621" on Mac.
* Connect LEDs or other outputs to pins 9 and 11.
* Run this sketch and move your mouse horizontally across the screen.
  
For more information, see: http://playground.arduino.cc/Interfacing/Processing
*/

boolean FIND_ARDUINO = false;

import processing.serial.*;
import cc.arduino.*;

import oscP5.*;
import netP5.*;

// OSC
OscP5 oscP5;
NetAddress iPhone;
int portOut = 9000;
int portIn = 8000;

// FIRMATA
Arduino arduino;

int my_arduino = 4;

//OSC Data
float intensityBlue = 0;
float intensityYellow = 0;
float intensityRed = 0;
float intensityVibe = 0;

float pulseBlue = 0.0;
float pulseYellow = 0.0;
float pulseRed = 0.0;
float pulseVibe = 0.0;

// Earduino Pins
int bluePin = 3; 
int yellowPin = 4;
int redPin = 5;
int vibePinOne = 9; // 22
int vibePinTwo = 10; // 23

float blueCtrl = 0.0;
float yellowCtrl = 0.0;
float redCtrl = 0.0;
float vibeCtrl1 = 0.0;
float vibeCtrl2 = 0.0;




float seconds = 0.0; // time counted in seconds from the beginning of the program




void setup() {
  

  // Prints out the available serial ports.
   if ( FIND_ARDUINO == true ) { 
    
   //StringList inventory;
    int numberinos = Arduino.list().length;
    for (int i=0; i<numberinos; i++) {
      println(i, ":", Arduino.list()[i]);
    }
    exit();
  }
  
  
  /* OSC */
  // start oscP5, telling it to listen for incoming messages at port 5001 */
  oscP5 = new OscP5(this,portIn);

  // set the remote location to be the localhost on port 5001
  iPhone = new NetAddress("128.237.130.237",portIn);
  
  
  /* FIRMATA */
  // Modify this line, by changing the "0" to the index of the serial
  // port corresponding to your Arduino board (as it appears in the list
  // printed by the line above).
  arduino = new Arduino(this, Arduino.list()[my_arduino], 57600);
  
  // Alternatively, use the name of the serial port corresponding to your
  // Arduino (in double-quotes), as in the following line.
  //arduino = new Arduino(this, "/dev/tty.usbmodem621", 57600);
  
  size(512, 200);
}

void draw() {
  
  // keep time
  seconds = millis() * 0.001;
  
  
  background(constrain(mouseX / 2, 0, 255));
  
  // float sinWave(float period, float phase, float center, float amplitude) {
  float period_max = 1.5;
  float period_min = 0.02;
  float intensity_max = 255.0;
  float intensity_min = 0.0;
  
  float period_max_vibe = .5;
  float period_min_vibe = 0.02;
  float intensity_max_vibe = 255.0;
  float intensity_min_vibe = 100.0;
  
  
  float temp_period = map(pulseBlue,0.0,100.0,period_max,period_min);
  float temp_intensity = map(intensityBlue,0.0,100.0,intensity_min,intensity_max);
  blueCtrl = sinWave(temp_period, PI, temp_intensity/2, temp_intensity/2);

  temp_period = map(pulseYellow,0.0,100.0,period_max,period_min);
  temp_intensity = map(intensityYellow,0.0,100.0,intensity_min,intensity_max);  
  yellowCtrl = sinWave(temp_period, PI, temp_intensity/2, temp_intensity/2);
  
  temp_period = map(pulseRed,0.0,100.0,period_max,period_min);
  temp_intensity = map(intensityRed,0.0,100.0,intensity_min,intensity_max);  
  redCtrl = sinWave(temp_period, PI, temp_intensity/2, temp_intensity/2);
  
  temp_period = map(pulseVibe,0.0,100.0,period_max_vibe,period_min_vibe);
  temp_intensity = map(intensityVibe,0.0,100.0,intensity_min_vibe,intensity_max_vibe);  
  vibeCtrl1 = sinWave(temp_period, PI, temp_intensity/2, temp_intensity/2);  
  
  //println(led1Val);
   
   
  
  // Output analog values (PWM waves) to digital pins 9 and 11.
  // Note that only certain Arduino pins support analog output (PWM).
  // See the documentation for your board for details.
  //arduino.analogWrite(redPin, map);
  
  arduino.analogWrite(bluePin, int(blueCtrl));
  arduino.analogWrite(yellowPin, int(yellowCtrl));
  arduino.analogWrite(redPin, int(redCtrl));
  
  //arduino.analogWrite(23, 200);
  //arduino.analogWrite(22, 200);
  
  arduino.analogWrite(vibePinOne, int(vibeCtrl1));
  arduino.analogWrite(vibePinTwo, int(vibeCtrl1));
  
  //arduino.analogWrite(18, 200);
  //arduino.analogWrite(19, 200);
  //arduino.analogWrite(17, 200);
  //arduino.analogWrite(16, 200);
  //arduino.analogWrite(20, 200);
  //arduino.analogWrite(21, 200);
  //arduino.analogWrite(24, 200);
  
  //arduino.analogWrite(25, 200);
  
  
}



float sinWave(float period, float phase, float center, float amplitude) {

  //float period = 1.0;
  float frequency = 1 / period;
  //float phase = 0;
  float wave = 0.0;
  //Serial.println(seconds);
  return wave =   (sin(seconds * frequency * 1.0 + phase ) * (amplitude) + center); //magic 6.12

}
 
 
void oscEvent(OscMessage theOscMessage) {   //  This runs whenever there is a new OSC message

    // All float values coming in between 0.0 - 100.0
    String addr = theOscMessage.addrPattern();  //  Creates a string out of the OSC message
    //println(addr);
    
    switch(addr) {
      case "/faderBlue": 
        pulseBlue = theOscMessage.get(0).floatValue(); 
        break;
      case "/rotaryBlue": 
        intensityBlue = theOscMessage.get(0).floatValue(); 
        break;
      case "/faderYellow": 
        pulseYellow = theOscMessage.get(0).floatValue(); 
        break;
      case "/rotaryYellow": 
        intensityYellow = theOscMessage.get(0).floatValue(); 
        break;
      case "/faderRojo": 
        pulseRed = theOscMessage.get(0).floatValue(); 
        break;
      case "/rotaryRed": 
        intensityRed = theOscMessage.get(0).floatValue(); 
        break;
      case "/pad": 
        intensityVibe = theOscMessage.get(0).floatValue(); 
        pulseVibe = theOscMessage.get(1).floatValue(); 
        //println(pulseVibe, intensityVibe);
        break; 
      
        
    }
    

    
    
}