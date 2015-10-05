/* 
  This sketch reads the acceleration from the Bean's on-board accelerometer. 
  
  The acceleration readings are sent over serial and can be accessed in Arduino's Serial Monitor.
  
  To use the Serial Monitor, set Arduino's serial port to "/tmp/tty.LightBlue-Bean"
  and the Bean as "Virtual Serial" in the OS X Bean Loader.
    
  This example code is in the public domain.
*/

#include<stdlib.h>


// Accelerometer units to gravity
const float g = 3.91e-3;

void setup() {
  // Bean Serial is at a fixed baud rate. Changing the value in Serial.begin() has no effect.
  Serial.begin();   
  // Optional: Use Bean.setAccelerationRange() to set the sensitivity to something other than the default of ±2g.
}

void loop() {
  // Get the current acceleration with range of ±2g, and a conversion of 3.91×10-3 g/unit or 0.03834(m/s^2)/units. 
  AccelerationReading acceleration = Bean.getAcceleration();
  
  float ax,ay,az;
  ax = acceleration.xAxis*g;
  ay = acceleration.yAxis*g;
  az = acceleration.zAxis*g;
  
  // http://physics.rutgers.edu/~aatish/teach/srr/workshop3.pdf
  float roll = -atan(ax/sqrt(pow(ay,2) + pow(az,2)));
  float pitch = atan(ay/sqrt(pow(ax,2) + pow(az,2)));
  //convert radians into degrees
  pitch = pitch * (180.0/PI);
  roll = roll * (180.0/PI) ;

  uint16_t batteryReading =  Bean.getBatteryVoltage(); 
  int joystickY = analogRead(A0); // 0-255
  int joystickX = analogRead(A1); // 0-255
  int joyClick = !digitalRead(0); // 0-255
  
  int joystickY_0_254 = map(joystickY, 0, 1024, 0, 254);
  int joystickX_0_254 = map(joystickX, 0, 1024, 0, 254);
  int pitch_0_254 = map(pitch, -90.0, 90.0, 0, 254);
  
//  Serial.print(joystickY_0_254);
//    Serial.print("\t");
//  Serial.print(joystickX_0_254);
//    Serial.print("\t");  
//  Serial.print(pitch_0_254);
//    Serial.print("\n");  
      
    
  // Requires data from 0 - 254 (255 reserved for packet header)
  serialSend4(joystickY_0_254,joystickX_0_254,pitch_0_254,joyClick);
  
  // Print Pitch and Roll Angles
//  String stringToPrint = String();
//  char pitchBuffer [8];
//  char rollBuffer [8];
//  dtostrf(pitch,5,2,pitchBuffer);
//  dtostrf(roll,5,2,rollBuffer);
//  stringToPrint = stringToPrint + "Pitch: " + pitchBuffer + "\tRoll: " + rollBuffer;
//  Serial.println(stringToPrint);
//  
//  stringToPrint = String();
//  char battBuffer [8];
//  char joyYBuffer [8];
//  char joyXBuffer [8];
//  dtostrf(batteryReading,5,2,battBuffer);
//  dtostrf(joystickY,5,2,joyYBuffer);
//  dtostrf(joystickX,5,2,joyXBuffer);
//  stringToPrint = stringToPrint + "joyY: " + joyYBuffer + "\tjoyX: " + joyXBuffer + "\tClx: " + joyClick + "\nBatt: " + battBuffer;
//  Serial.println(stringToPrint);
//  Serial.println("");
  
  Bean.sleep(5);

}






void serialSend4(int one, int two, int three, int four) {
  
  Serial.write(255); // start byte
  Serial.write(one);
  Serial.write(two);
  Serial.write(three);
  Serial.write(four);
  
}



