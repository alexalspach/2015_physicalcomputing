#include <Filters.h>
float filterFrequency = 5.0;  
float filteredValue = 0.0;
FilterOnePole lowpassFilter( LOWPASS, filterFrequency ); 

int bluePin = 3;
int greenPin = 5;
int yellowPin = 6;
int redPin = 9;

int micPin = 0;
int micVal = 0;
int micValLast = 0;
int micRaw = 0;

const int windowSize = 20;
int data[windowSize];

int index = 0;
int total = 0;
int average = 0;



void setup() {
  // put your setup code here, to run once:

  pinMode( micPin, INPUT );
  pinMode( bluePin, OUTPUT );
  Serial.begin(9600); 

  for (int thisReading = 0; thisReading < windowSize; thisReading++)
    data[thisReading] = 0;

}

void loop() {
  // put your main code here, to run repeatedly:

  //analogWrite( bluePin, ledValue );
  //ledValue+1;
  //delay(500);
  //analogWrite( bluePin, 0 );
  //delay(500);
  
  //  for (int i=0; i <= 255; i++){
  //      analogWrite(bluePin, i);
  //      delay(5);
  //   }
  //  
  //  for (int i=255; i >= 0; i = i-5){
  //      analogWrite(bluePin, i);
  //      delay(5);
  //   }

  //  micVal = analogRead(micPin);
  //  //analogWrite( bluePin, (micVal/10) );
  //  analogWrite( bluePin, map(micVal, 0, 1023, 0, 255) );
  //  //Serial.println(micVal);   


  //filteredValue = lowpassFilter.input( analogRead( micPin ) );
  micRaw = analogRead(micPin);
  filteredValue = micValLast + .1 * (micRaw - micValLast);
  micValLast = filteredValue;




  // Low Pass Filtering
  total = total - data[index];
  data[index] = micRaw;
  total = total + data[index];
  index=index+1;

  if (index >= windowSize)
    index = 0;

  average = total / windowSize;

  //Serial.println(average);
  delay(1);




  
   
  //analogWrite( bluePin, (micVal/10) );

  // Which Mode are we in (0-3)
  int micMin = 0;
  int micMax = 1024;
  int numLeds = 4;

  int mode = 0;
  int colorPin = 0;

  if ( average > micMin )
  {
    //Serial.println("mode 1");
    mode = 1;
    colorPin = bluePin;
  }
  
  if ( (average > (micMin+(1*micMax/numLeds)) ) && (average < (2*micMax/numLeds)) )
  {
    //Serial.println("mode 2");
    mode = 2;
    colorPin = greenPin;
  }
  
  if ( (average > (micMin+(2*micMax/numLeds)) ) && (average < (3*micMax/numLeds)) )
  {
    //Serial.println("mode 2");
    mode = 3;
    colorPin = yellowPin;
  }

  if ( (average > (micMin+(3*micMax/numLeds)) ) && (average < (4*micMax/numLeds)) )
  {
    //Serial.println("mode 2");
    mode = 4;
    colorPin = redPin;
  }

  //Serial.println(filteredValue);
  Serial.println(average);
  //Serial.println(mode);
  analogWrite( bluePin, map(average, 0, 1023, 0, 255) );
  //Serial.println(micVal);  

}
