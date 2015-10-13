
import processing.serial.*;

// How many NeoPixels are attached to the Arduino?
final int NUMPIXELS = 7; // Constant, cannot be modified 

boolean FIND_ARDUINO = true;
Serial serial;
int serial_port = 4;

// colors
float[] rA = new float[11];
float[] gA = new float[11];
float[] bA = new float[11];

void setup()
{
  size(512, 200);
  

  
    // Prints out the available serial ports.
   if ( FIND_ARDUINO == true ) { 
    
   //StringList inventory;
    int numberinos = Serial.list().length;
    for (int i=0; i<numberinos; i++) {
      println(i, ":", Serial.list()[i]);
    }
    exit();
  }


  serial = new Serial(this, Serial.list()[serial_port], 9600);
  

}

int i = 0;

void draw() {
  
  
  rA[0] = 0;
  gA[0] = 0;
  bA[0] = 0;
  
  rA[1] = mouseX;
  gA[1] = mouseX;
  bA[1] = 0;
  
  rA[2] = 0;
  gA[2] = 255;
  bA[2] = 0;
  
  serial.write(0xff); //write marker, arduino is looking for this
  for (int Br = 1; Br < NUMPIXELS; Br++){
    serial.write((byte)(rA[Br]));
    serial.write((byte)(gA[Br]));
    serial.write((byte)(bA[Br]));
  }
  
  
  
}