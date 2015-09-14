
const boolean DEBUG = false;
const boolean BUZZERS = true;
const boolean PRINT_LEDS = false;

float seconds = 0.0; // time counted in seconds from the beginning of the program

// For debugging over serial
int controlValue = 0; // Takes place of the mic raw data when debugging

// Pins
int led1 = 3;
int led2 = 4;
int led3 = 5;
int micPin = 7;
int vibePinOne = 23;
int vibePinTwo = 22;

// Buzzer Parameters
int vibeMin = 50; // Lowest PEWM value that I can feel
int vibeMax = 255; // Max vibration PWM value
int vibeAmp = (vibeMax - vibeMin) / 2; // Sine wave amplitube
int vibeCenter = vibeMin + vibeAmp; // Sine wave center

// MIC Parameters
int micMin = 0;
int micMax = 1023; //1023 actual max (Lower number maxes max volume lower

// LED Parameters
int ledMin = 0;
int ledMax = 255;


// PWM values for buzzers
int vibeCommandOne = 0;
int vibeCommandTwo = 0;

// PWM values for LEDs
int tempVal = 0;
int led1Val = 0;
int led2Val = 0;
int led3Val = 0;





// Mic value thresholds that determine which mode we are in: Safe, Warning, Danger
int threshOne = 340;
int threshTwo = 680;
int overlap = 0; // Overlap between modes

int micRaw = 0;

int mode = 0;

const unsigned long windowSize = 2000;
unsigned long data[windowSize];

unsigned long total = 0;
unsigned long average = 0;
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

  //  // map test
  //  int test = 100;
  //  int mapped1 =  map(0, 0, 254, 0, 255);
  //  int mapped2 =  map(500, 255, 509, 0, 255);
  //  int mapped3 =  map(510, 510, 764, 0, 255);
  //  int mapped4 =  map(1000, 765, 1023, 0, 255);
  //
  //  Serial.println(mapped1);
  //  Serial.println(mapped2);
  //  Serial.println(mapped3);
  //  Serial.println(mapped4);
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
    }
  }
  else {
    micRaw = analogRead(micPin);
  }

  //Serial.println(micRaw);
  //Serial.println(millis());




  average = movingAverage();
  // Serial.println(average);
  delay(1);







  // Which Mode are we in (1-3: Safe, Warning, Danger)
  mode = 0;

  // Reset LED PWM values to Zero
  led1Val = 0;
  led2Val = 0;
  led3Val = 0;


  // First Range, Safe Range, Mode1
  if ( (average >= micMin) && (average < threshOne) )
  {
    mode = 1;
    tempVal = map(average, micMin, threshOne, ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    led1Val = tempVal;
  }

  // Second Range, Warning Range, Mode2
  if ( (average >= (threshOne - overlap) ) && (average < threshTwo) )
  {
    tempVal = map(average, (threshOne - overlap), threshTwo, ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    led2Val = tempVal;

    if (average >= threshOne) mode = 2;

  }

  // Third Range, Danger Range, Mode3
  if ( (average >= (threshTwo - overlap) ) && (average <= micMax ) )
  {
    tempVal = map(average, (threshTwo - overlap), micMax, ledMin, ledMax);
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

    float waveFreq = map(led2Val, 0, 255, 3.0, 0.1);

    led1Val = sinWave(waveFreq, PI, led2Val / 2, led2Val / 2);
    led2Val = sinWave(waveFreq, 0.0, led2Val / 2, led2Val / 2);

    vibeCommandOne = 0;
    vibeCommandTwo = 0;
    //vibeCommandOne = squareWave(1.0, 0.0, vibeMax);
    //vibeCommandTwo = squareWave(1.0, 0.0, vibeMax);
  }

  // Third Range, Danger Range, Mode3
  if (mode == 3) {

    float waveFreq = map(led3Val, 0, 255, 2.0, 0.01);

    led1Val = squareWave(waveFreq / 2, PI, led3Val);
    led2Val = squareWave(waveFreq / 2, PI / 2, led3Val);
    led3Val = squareWave(waveFreq / 4, 0.0, led3Val);
    //led1Val = sinWave(1.0, 3*PI/3, 255/2, 255/2);
    //led2Val = sinWave(1.0, 2*PI/3, 255/2, 255/2);
    //led3Val = sinWave(0.5, 1*PI/3, 255/2, 255/2);


    vibeCommandOne = sinWave(waveFreq / 4, 0.0, vibeCenter, vibeAmp);
    //vibeCommandTwo = sinWave(waveFreq/2, PI, vibeCenter, vibeAmp);
    vibeCommandTwo = sinWave(waveFreq / 4, PI, vibeCenter, vibeAmp);
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


float sinWave(float period, float phase, int center, int amplitude) {

  //float period = 1.0;
  float frequency = 1 / period;
  //float phase = 0;
  float wave = 0.0;
  //Serial.println(seconds);
  return wave =   (sin(seconds * frequency * 6.12 + phase ) * (amplitude) + center);

}


int squareWave(float period, float phase, int amplitude) {

  if ( period <= 0.001 ) return 0.0;

  int wave = 0;
  int periodMs = period * 1000; //convert to milliseconds
  float phaseMillis = (phase / (2 * PI)) * (periodMs);
  int ms = millis() + phaseMillis;

  if ( (ms % periodMs) >= (periodMs / 2) ) wave = amplitude;
  else wave = 0;

  return wave;

}

int movingAverage() {

  total = total - data[dataIndex];
  data[dataIndex] = micRaw;
  total = total + data[dataIndex];
  dataIndex = dataIndex + 1;

  if (dataIndex >= windowSize)
    dataIndex = 0;

  return ( total / windowSize );

}

