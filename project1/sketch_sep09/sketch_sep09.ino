int bluePin = 3;
int greenPin = 5;
int yellowPin = 6;
int redPin = 9;

int pumpPin = 10;
int valvePin = 2;

int tempVal = 0;
int blueVal = 0;
int greenVal = 0;
int yellowVal = 0;
int redVal = 0;

int micMin = 0;
int micMax = 500; //1023 actual max (Lower number maxes max volume lower
int ledMin = 0;
int ledMax = 255;
int numLeds = 4;

int micPin = 0;
int micVal = 0;
int micValLast = 0;
int micRaw = 0;

int mode = 0;
int colorPin = 0;

const unsigned long windowSize = 300;
unsigned long data[windowSize];

unsigned long total = 0;
unsigned long average = 0;
int index = 0;


void setup() {
  // put your setup code here, to run once:

  pinMode( micPin, INPUT );
  pinMode( bluePin, OUTPUT );
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
  // put your main code here, to run repeatedly:

  // Needs transistor. Pump draws about 400mA
  analogWrite( pumpPin, 200 );

  digitalWrite( valvePin, HIGH );
  

  micRaw = analogRead(micPin);
  //micVal = micValLast + .1 * (micRaw - micValLast);
  //micValLast = micVal;


  // Moving Average Smoothing
  total = total - data[index];
  data[index] = micRaw;
  total = total + data[index];
  index=index+1;

  if (index >= windowSize)
    index = 0;

  average = total / windowSize;
  delay(1);

  // Testing without whistling...
  // average++;
  // if (average >= 1023) average = 0;




  


  // Which Mode are we in (0-3)
  mode = 0;
  colorPin = 0;

  //for (int pin = 0; pin++; pin <= 12)
  //  digitalWrite(pin, 0);
  
  blueVal = 0;
  greenVal = 0;
  yellowVal = 0;
  redVal = 0;

  if ( ( average > micMin ) && (average < (1*micMax/numLeds)) )
  {
    //Serial.println("mode 1");
    mode = 1;
    tempVal = map(average, micMin, (1*micMax/numLeds), ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    blueVal = tempVal;
  }
  
  if ( (average > (micMin+(1*micMax/numLeds)) ) && (average < (2*micMax/numLeds)) )
  {
    //Serial.println("mode 2");
    mode = 2;
    tempVal = map(average, (micMin+(1*micMax/numLeds)), (2*micMax/numLeds), ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    greenVal = tempVal;
    
  }
  
  if ( (average > (micMin+(2*micMax/numLeds)) ) && (average < (3*micMax/numLeds)) )
  {
    //Serial.println("mode 2");
    mode = 3;
    tempVal = map(average, (micMin+(2*micMax/numLeds)), (3*micMax/numLeds), ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    yellowVal = tempVal;
  }

  if ( (average > (micMin+(3*micMax/numLeds)) ) && (average < (4*micMax/numLeds)) )
  {
    //Serial.println("mode 2");
    mode = 4;
    tempVal = map(average, (micMin+(3*micMax/numLeds)), (4*micMax/numLeds), ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    redVal = tempVal;
  }

  //Serial.println(filteredValue);
  //Serial.println(average);
  //Serial.println(mode);
  //analogWrite( colorPin, map(average, 0, 1023, 0, 255) );
  //Serial.println(micVal);  

  analogWrite( bluePin, blueVal );
  analogWrite( greenPin, greenVal );
  analogWrite( yellowPin, yellowVal );
  analogWrite( redPin, redVal );

//  Serial.print(average);
//  Serial.print("\t");
//  Serial.print(blueVal);
//  Serial.print("\t");
//  Serial.print(greenVal);
//  Serial.print("\t");
//  Serial.print(yellowVal);
//  Serial.print("\t");
//  Serial.print(redVal);
//  Serial.print("\n");

}
