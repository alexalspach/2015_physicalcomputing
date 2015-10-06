/*
  This sketch reads the acceleration from the Bean's on-board accelerometer.

  The acceleration readings are sent over serial and can be accessed in Arduino's Serial Monitor.

  To use the Serial Monitor, set Arduino's serial port to "/tmp/tty.LightBlue-Bean"
  and the Bean as "Virtual Serial" in the OS X Bean Loader.

  This example code is in the public domain.
*/

#include<stdlib.h>

int joyY_pin = A0;
int joyX_pin = A1;

// Accelerometer units to gravity
const float g = 3.91e-3;

// Global accel, pitch and roll updated in calcPitchRoll()
AccelerationReading acceleration;
float roll, pitch;

// Three low-pass filters
const unsigned long pitch_windowSize = 3;
float pitch_data[pitch_windowSize];
float pitch_total = 0.0;
int pitch_dataIndex = 0;
float pitch_filtered = 0.0;

const unsigned long joyX_windowSize = 3;
float joyX_data[joyX_windowSize];
float joyX_total = 0.0;
int joyX_dataIndex = 0;
float joyX_filtered = 0.0;

const unsigned long joyY_windowSize = 20;
float joyY_data[joyY_windowSize];
float joyY_total = 0.0;
int joyY_dataIndex = 0;
float joyY_filtered = 0.0;

int joyX_offset = 0;
int joyY_offset = 0;
float pitch_offset = 0.0;

void setup() {
  // Bean Serial is at a fixed baud rate. Changing the value in Serial.begin() has no effect.
  Serial.begin();
  // Optional: Use Bean.setAccelerationRange() to set the sensitivity to something other than the default of ±2g.

  for (int thisReading = 0; thisReading < pitch_windowSize; thisReading++)
    pitch_data[thisReading] = 0;
  for (int thisReading = 0; thisReading < joyX_windowSize; thisReading++)
    joyX_data[thisReading] = 0;
  for (int thisReading = 0; thisReading < joyY_windowSize; thisReading++)
    joyY_data[thisReading] = 0;

  int count = 0;
  float offset = 0.0;
  for (int i = 0; i < 100; i++) {
    offset += analogRead(joyY_pin);
    count++;
    //delay(1);
  }
  joyY_offset = offset / count;
  joyY_offset = (1024 / 2) - joyY_offset;
  Serial.println(joyY_offset);

  count = 0;
  offset = 0.0;
  for (int i = 0; i < 100; i++) {
    offset += analogRead(joyX_pin);
    count++;
    //delay(1);
  }
  joyX_offset = offset / count;
  Serial.println("here");
  Serial.println(joyX_offset);
  joyX_offset = (1024 / 2) - joyX_offset;
  Serial.println(joyX_offset);

  count = 0;
  offset = 0.0;
  for (int i = 0; i < 100; i++) {
    acceleration = Bean.getAcceleration();
    calcPitchRoll(acceleration);
    offset += pitch;
    count++;
    //delay(1);
  }
  pitch_offset = offset / count;
  //Serial.println("here");
  //Serial.println(pitch_offset);
  pitch_offset = 0.0 - pitch_offset;



}

void loop() {
  // Get the current acceleration with range of ±2g, and a conversion of 3.91×10-3 g/unit or 0.03834(m/s^2)/units.
  //AccelerationReading acceleration = Bean.getAcceleration();
  acceleration = Bean.getAcceleration();
  calcPitchRoll(acceleration);
  pitch += pitch_offset;
  //Serial.println(pitch);

  //uint16_t batteryReading =  Bean.getBatteryVoltage();
  int joystickY = analogRead(joyY_pin) + joyY_offset; // 0-1023
  if (joystickY > 1023) joystickY = 1023;
  if (joystickY < 0) joystickY = 0;
  if ( abs(joystickY - 512 ) < 5 ) joystickY = 512; // dead zone at center
  //Serial.println(joystickY);

  int joystickX = analogRead(A1) + joyX_offset; // 0-1023
  if (joystickX > 1023) joystickX = 1023;
  if (joystickX < 0) joystickX = 0;
  if ( abs(joystickX - 512 ) < 5 ) joystickX = 512; // dead zone at center



  //joystickX = 512;
  //Serial.println(joystickX);

  int joyClick = !digitalRead(0); // 0 or 1


  // Filtering
  pitch_filtered = filterPitch(pitch);
  //joyX_filtered = filterJoyX(joystickX);
  //joyY_filtered = filterJoyY(joystickY);

  //Serial.println(pitch_filtered);
  //Serial.println(joyX_filtered);
  //Serial.println(joyY_filtered);


  // mapped to 0-254 because 255 is defined as start of the packet. see serialSend4()
  int joystickY_0_254 = map(joystickY, 0, 1023, 0, 254);
  int joystickX_0_254 = map(joystickX, 0, 1023, 0, 254);
  int pitch_0_254 = map(pitch_filtered, -90.0, 90.0, 0, 254);

  //Serial.println(joystickY_0_254);
  serialSend4(joystickY_0_254, joystickX_0_254, pitch_0_254, joyClick);

  //  Serial.print(joystickY_0_254);
  //  Serial.print("\t");
  //  Serial.print(joystickX_0_254);
  //  Serial.print("\t");
  //  Serial.print(pitch_0_254);
  //  Serial.print("\n");

  Bean.sleep(5);

}







/*
const unsigned long pitch_windowSize = 100;
float pitch_data[pitch_windowSize];
float pitch_total = 0.0;
float pitch_average = 0.0;
int pitch_dataIndex = 0;

const unsigned long joyX_windowSize = 100;
float joyX_data[joyX_windowSize];
float joyX_total = 0.0;
float joyX_average = 0.0;
int joyX_dataIndex = 0;

const unsigned long joyY_windowSize = 100;
float joyY_data[joyY_windowSize];
float joyY_total = 0.0;
float joyY_average = 0.0;
int joyY_dataIndex = 0;
*/

float filterPitch(float raw) {

  pitch_total = pitch_total - pitch_data[pitch_dataIndex];
  pitch_data[pitch_dataIndex] = raw;
  pitch_total = pitch_total + pitch_data[pitch_dataIndex];
  pitch_dataIndex = pitch_dataIndex + 1;

  if (pitch_dataIndex >= pitch_windowSize)
    pitch_dataIndex = 0;

  return ( pitch_total / pitch_windowSize );

}

float filterJoyX(float raw) {

  joyX_total = joyX_total - joyX_data[joyX_dataIndex];
  joyX_data[joyX_dataIndex] = raw;
  joyX_total = joyX_total + joyX_data[joyX_dataIndex];
  joyX_dataIndex = joyX_dataIndex + 1;

  if (joyX_dataIndex >= joyX_windowSize)
    joyX_dataIndex = 0;

  return ( joyX_total / joyX_windowSize );

}

float filterJoyY(float raw) {

  joyY_total = joyY_total - joyY_data[joyY_dataIndex];
  joyY_data[joyY_dataIndex] = raw;
  joyY_total = joyY_total + joyY_data[joyY_dataIndex];
  joyY_dataIndex = joyY_dataIndex + 1;

  if (joyY_dataIndex >= joyY_windowSize)
    joyY_dataIndex = 0;

  return ( joyY_total / joyY_windowSize );

}

/*
float movingAverage() {

  total = total - data[dataIndex];
  data[dataIndex] = micRaw;
  total = total + data[dataIndex];
  dataIndex = dataIndex + 1;

  if (dataIndex >= windowSize)
    dataIndex = 0;

  return ( total / windowSize );

}
*/

void serialSend4(int one, int two, int three, int four) {

  Serial.write(255); // start byte
  Serial.write(one);
  Serial.write(two);
  Serial.write(three);
  Serial.write(four);

}


void calcPitchRoll(AccelerationReading acceleration) {

  //float pitch_roll[] = {0,0};
  //float pitch, roll;

  float ax, ay, az;
  ax = acceleration.xAxis * g;
  ay = acceleration.yAxis * g;
  az = acceleration.zAxis * g;

  //Serial.println(acceleration.xAxis);

  // http://physics.rutgers.edu/~aatish/teach/srr/workshop3.pdf
  roll = -atan(ax / sqrt(pow(ay, 2) + pow(az, 2)));
  pitch = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)));
  //convert radians into degrees
  pitch = pitch * (180.0 / PI);
  roll = roll * (180.0 / PI);

  //pitch_roll[0] = pitch;
  //pitch_roll[1] = roll;

  //return pitch_roll;

}

