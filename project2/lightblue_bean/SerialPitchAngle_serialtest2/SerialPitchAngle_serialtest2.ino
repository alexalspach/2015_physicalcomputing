
/* 
  This sketch reads the acceleration from the Bean's on-board accelerometer. 
  
  The acceleration readings are sent over serial and can be accessed in Arduino's Serial Monitor.
  
  To use the Serial Monitor, set Arduino's serial port to "/tmp/tty.LightBlue-Bean"
  and the Bean as "Virtual Serial" in the OS X Bean Loader.
    
  This example code is in the public domain.
*/

#include<stdlib.h>

 #include <BufferedSerial.h>
 #include <ByteBuffer.h>
 
BufferedSerial serial = BufferedSerial(256, 256);
 ByteBuffer send_buffer;
 
 

// Accelerometer units to gravity
const float g = 3.91e-3;

void setup() {
  // Bean Serial is at a fixed baud rate. Changing the value in Serial.begin() has no effect.
  // initialize the serial communication:
   serial.init(0, 9600);
   serial.setPacketHandler(handlePacket);
  
   // Initialize the send buffer that we will use to send data
   send_buffer.init(30); 
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
  
  
  serialDataPrint(pitch, roll);
  
  // Print Pitch and Roll Angles
  String stringToPrint = String();
  char pitchBuffer [8];
  char rollBuffer [8];
  dtostrf(pitch,5,2,pitchBuffer);
  dtostrf(roll,5,2,rollBuffer);
  stringToPrint = stringToPrint + "Pitch: " + pitchBuffer + "\tRoll: " + rollBuffer;
  Serial.println(stringToPrint);

  
  Bean.sleep(500);
}


void serialDataPrint(float pitch, float roll) {
  //http://www.varesano.net/blog/fabio/sending-float-variables-over-serial-without-loss-precision-arduino-and-processing
  
  byte * b = (byte *) &pitch;
  Serial.print("f:");
  Serial.write(b[0]);
  Serial.write(b[1]);
  Serial.write(b[2]);
  Serial.write(b[3]);
  

//  byte * c = (byte *) &roll;
//  Serial.write(c[0]);
//  Serial.write(c[1]);
//  Serial.write(c[2]);
//  Serial.write(c[3]);
//  
  /* DEBUG */
//  Serial.println();
//  Serial.print(b[0],BIN);
//  Serial.print(b[1], BIN);
//  Serial.print(b[2], BIN);
//  Serial.println(b[3], BIN);
//  
//    Serial.println();
//  Serial.print(c[0],BIN);
//  Serial.print(c[1], BIN);
//  Serial.print(c[2], BIN);
//  Serial.println(c[3], BIN);
  //*/
}

