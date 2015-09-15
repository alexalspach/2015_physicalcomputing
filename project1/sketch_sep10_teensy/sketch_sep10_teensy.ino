
const boolean DEBUG = false;
const boolean BUZZERS = true;
const boolean SHOW = false;

const boolean PRINT_LEDS = false;

float seconds = 0.0; // time counted in seconds from the beginning of the program

// For debugging over serial
float controlValue = 0.0; // Takes place of the mic raw data when debugging

// Pins
int led1 = 3;
int led2 = 4;
int led3 = 5;
int micPin = 7;
int vibePinOne = 23;
int vibePinTwo = 22;

// Buzzer Parameters
float vibeMin = 50.0; // Lowest PWM value that I can feel
float vibeMax = 255.0; // Max vibration PWM value
float vibeAmp = (vibeMax - vibeMin) / 2.0; // Sine wave amplitube
float vibeCenter = vibeMin + vibeAmp; // Sine wave center

// MIC Parameters
float micMin = 0.0;
float micMax = 1023.0; //1023 actual max (Lower number maxes max volume lower

// LED Parameters
float ledMin = 0.0;
float ledMax = 255.0;


// PWM values for buzzers
float vibeCommandOne = 0.0;
float vibeCommandTwo = 0.0;

// PWM values for LEDs
float tempVal = 0.0;
float led1Val = 0.0;
float led2Val = 0.0;
float led3Val = 0.0;





// Mic value thresholds that determine which mode we are in: Safe, Warning, Danger
float threshOne = 340.0;
float threshTwo = 680.0;
float overlap = 0.0; // Overlap between modes

float micRaw = 0.0;

float mode = 0.0;

const unsigned long windowSize = 4000;
float data[windowSize];

float total = 0.0;
float average = 0.0;
int dataIndex = 0;


void setup() {
  // put your setup code here, to run once:

  //setTime(0);

  pinMode( micPin, INPUT );

  pinMode( led1, OUTPUT );
  pinMode( led2, OUTPUT );
  pinMode( led3, OUTPUT );

  pinMode( vibePinOne, OUTPUT );
  pinMode( vibePinTwo, OUTPUT );

  Serial.begin(9600);

  for (int thisReading = 0; thisReading < windowSize; thisReading++)
    data[thisReading] = 0;
}

void loop() {


  // keep time
  seconds = millis() * 0.001;

  // put your main code here, to run repeatedly:

  // send data only when you receive data:

  if ( DEBUG == true ) {
    if (Serial.available() > 0) {
      // read the incoming integer
      controlValue = Serial.parseInt();
      // say what you got:
      Serial.println(controlValue);
      micRaw = controlValue;
      average = controlValue;
    }
  }
  
  if (SHOW == true) {
    if ((millis() % 50) == 0 ) micRaw+=1.0; //50
    if ( micRaw>micMax ) {
      micRaw = 0.0;
      average = 0.0;
    }
  }
  
  if ( DEBUG == false && SHOW == false) { 
    micRaw = analogRead(micPin); 
  }
  
  //else {
  //  micRaw = analogRead(micPin);
  //}

  //Serial.println(micRaw);
  //Serial.println(millis());




  average = movingAverage();
  // Serial.println(average);
  delay(1);

  //Serial.println(average);






  // Which Mode are we in (1-3: Safe, Warning, Danger)
  mode = 0;

  // Reset LED PWM values to Zero
  led1Val = 0.0;
  led2Val = 0.0;
  led3Val = 0.0;


  // First Range, Safe Range, Mode1
  if ( (average >= micMin) && (average < threshOne) )
  {
    mode = 1;
    tempVal = mapfloat(average, micMin, threshOne, ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    led1Val = tempVal;
  }

  // Second Range, Warning Range, Mode2
  if ( (average >= (threshOne - overlap) ) && (average < threshTwo) )
  {
    tempVal = mapfloat(average, (threshOne - overlap), threshTwo, ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    led2Val = tempVal;
    
    //Serial.println(led2Val);

    if (average >= threshOne) mode = 2;

  }

  // Third Range, Danger Range, Mode3
  if ( (average >= (threshTwo - overlap) ) && (average <= micMax ) )
  {
    tempVal = mapfloat(average, (threshTwo - overlap), micMax, ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    led3Val = tempVal;

    if (average >= threshTwo) mode = 3;

  }

  // First Range, Safe Range, Mode1
  if (mode == 1) {

    vibeCommandOne = 0;
    vibeCommandTwo = 0;
  }

  // Second Range, Warning Range, Mode2
  if (mode == 2) {

    float waveFreq = mapfloat(led2Val, 0.0, 255.0, 3.0, 1.0);

    led1Val = sinWave(waveFreq, PI, led2Val / 2, led2Val / 2);
    led2Val = sinWave(waveFreq, 0.0, led2Val / 2, led2Val / 2);

    vibeCommandOne = 0.0;
    vibeCommandTwo = 0.0;
    //vibeCommandOne = squareWave(1.0, 0.0, vibeMax);
    //vibeCommandTwo = squareWave(1.0, 0.0, vibeMax);
    
    //Serial.println(waveFreq);
    
  }

  // Third Range, Danger Range, Mode3
  if (mode == 3) {

    float waveFreq = mapfloat(led3Val, 0.0, 255.0, 2.0, 1.0);

    led1Val = squareWave(waveFreq / 1.0, PI, led3Val);
    led2Val = squareWave(waveFreq / 1.0, PI / 2.0, led3Val);
    led3Val = squareWave(waveFreq / 1.0, 0.0, led3Val);
    //led1Val = sinWave(1.0, 3*PI/3, 255/2, 255/2);
    //led2Val = sinWave(1.0, 2*PI/3, 255/2, 255/2);
    //led3Val = sinWave(0.5, 1*PI/3, 255/2, 255/2);


    vibeCommandOne = sinWave(waveFreq/2.0, 0.0, vibeCenter, vibeAmp);
    //vibeCommandTwo = sinWave(waveFreq/2, PI, vibeCenter, vibeAmp);
    vibeCommandTwo = sinWave(waveFreq/2.0, PI, vibeCenter, vibeAmp);
    
    //Serial.println(waveFreq);
  }



  // LED PWM Control! // ~~~~~~~~
  analogWrite( led1, led1Val );
  analogWrite( led2, led2Val );
  analogWrite( led3, led3Val );

  // Buzzers! // ~~~~~~~~

  // For debugging you can turn the buzzers off for sanity
  if (BUZZERS == false) {
    vibeCommandOne = 0;
    vibeCommandTwo = 0;
  }

  // Thresholding the PWM input values
  if (vibeCommandOne < 0) vibeCommandOne = 0;
  if (vibeCommandOne > vibeMax) vibeCommandOne = vibeMax;

  // Buzzer PWM Control
  analogWrite(vibePinOne, vibeCommandOne);
  analogWrite(vibePinTwo, vibeCommandTwo);



 Serial.println(average);

  if (PRINT_LEDS == true ) {
    Serial.print(average);
    Serial.print("\t");
    Serial.print(led1Val);
    Serial.print("\t");
    Serial.print(led2Val);
    Serial.print("\t");
    Serial.print(led3Val);
    Serial.print("\n");
  }

}


float sinWave(float period, float phase, float center, float amplitude) {

  //float period = 1.0;
  float frequency = 1 / period;
  //float phase = 0;
  float wave = 0.0;
  //Serial.println(seconds);
  return wave =   (sin(seconds * frequency * 1.0 + phase ) * (amplitude) + center); //magic 6.12

}


float squareWave(float period, float phase, float amplitude) {

  if ( period <= 0.001 ) return 0.0;

  int wave = 0;
  int periodMs = period * 1000; //convert to milliseconds
  int phaseMillis = (phase / (2.0 * PI)) * (periodMs);
  int ms = millis() + phaseMillis;

  if ( (ms % periodMs) >= (periodMs / 2) ) wave = amplitude;
  else wave = 0;

  return wave;

}

float movingAverage() {

  total = total - data[dataIndex];
  data[dataIndex] = micRaw;
  total = total + data[dataIndex];
  dataIndex = dataIndex + 1;

  if (dataIndex >= windowSize)
    dataIndex = 0;

  return ( total / windowSize );

}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  //Modified version of skumlerud's function from http://forum.arduino.cc/index.php?topic=3922.0
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
