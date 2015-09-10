int led1 = 3;
int led2 = 4;
int led3 = 5;
int led4 = 6;

int micPin = 7;

int vibePinOne = 23;
int vibePinTwo = 22;
boolean buzzer1Val = LOW;
boolean buzzer2Val = LOW;

int tempVal = 0;

int led1Val = 0;
int led2Val = 0;
int led3Val = 0;
int led4Val = 0;



int micMin = 0;
int micMax = 1023; //1023 actual max (Lower number maxes max volume lower
int ledMin = 0;
int ledMax = 255;

int numLeds = 3;


int micVal = 0;
int micValLast = 0;
int micRaw = 0;

int mode = 0;
int colorPin = 0;

const unsigned long windowSize = 300;
unsigned long data[windowSize];

unsigned long total = 0;
unsigned long average = 0;
int dataIndex = 0;


void setup() {
  // put your setup code here, to run once:

  pinMode( micPin, INPUT );
  
  pinMode( led1, OUTPUT );
  pinMode( led2, OUTPUT );
  pinMode( led3, OUTPUT );
  pinMode( led4, OUTPUT );
  
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
  // put your main code here, to run repeatedly:


  //analogWrite( vibePinOne, 255 );
  

  micRaw = analogRead(micPin);
  //micVal = micValLast + .1 * (micRaw - micValLast);
  //micValLast = micVal;

  // Moving Average Smoothing
  total = total - data[dataIndex];
  data[dataIndex] = micRaw;
  total = total + data[dataIndex];
  dataIndex=dataIndex+1;

  if (dataIndex >= windowSize)
    dataIndex = 0;

  average = total / windowSize;
  delay(1);

  // Testing without whistling...
//   average++;
//   if (average >= micMax) average = 0;
//   delay(3);



  


  // Which Mode are we in (0-3)
  mode = 0;
  colorPin = 0;

  //for (int pin = 0; pin++; pin <= 12)
  //  digitalWrite(pin, 0);
  
  led1Val = 0;
  led2Val = 0;
  led3Val = 0;
  led4Val = 0;

  if ( ( average > micMin ) && (average < (1*micMax/numLeds)) )
  {
    //Serial.println("mode 1");
    mode = 1;
    tempVal = map(average, micMin, (1*micMax/numLeds), ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    led1Val = tempVal;
    buzzer1Val = LOW;
    buzzer2Val = LOW;
  }
  
  if ( (average > (micMin+(1*micMax/numLeds)) ) && (average < (2*micMax/numLeds)) )
  {
    //Serial.println("mode 2");
    mode = 2;
    tempVal = map(average, (micMin+(1*micMax/numLeds)), (2*micMax/numLeds), ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    led2Val = tempVal;
    buzzer1Val = LOW;
    buzzer2Val = LOW;
    
  }
  
  if ( (average > (micMin+(2*micMax/numLeds)) ) && (average < (3*micMax/numLeds)) )
  {
    //Serial.println("mode 2");
    mode = 3;
    tempVal = map(average, (micMin+(2*micMax/numLeds)), (3*micMax/numLeds), ledMin, ledMax);
    if (tempVal < ledMin) tempVal = ledMin;
    if (tempVal > ledMax) tempVal = ledMax;
    led3Val = tempVal;
    buzzer1Val = HIGH;
    buzzer2Val = HIGH;
  }

//  if ( (average > (micMin+(3*micMax/numLeds)) ) && (average < (4*micMax/numLeds)) )
//  {
//    //Serial.println("mode 2");
//    mode = 4;
//    tempVal = map(average, (micMin+(3*micMax/numLeds)), (4*micMax/numLeds), ledMin, ledMax);
//    if (tempVal < ledMin) tempVal = ledMin;
//    if (tempVal > ledMax) tempVal = ledMax;
//    led4Val = tempVal;
//  }

  //Serial.println(micRaw);
  Serial.println(average);
  //Serial.println(mode);
  //analogWrite( colorPin, map(average, 0, 1023, 0, 255) );
  //Serial.println(micVal);  

  analogWrite( led1, led1Val );
  analogWrite( led2, led2Val );
  analogWrite( led3, led3Val );
  analogWrite( led4, led4Val );
  
  digitalWrite(vibePinOne, buzzer1Val); 
  digitalWrite(vibePinTwo, buzzer2Val); 
  
    // Needs transistor. Pump draws about 400mA
  

//  Serial.print(average);
//  Serial.print("\t");
//  Serial.print(led1Val);
//  Serial.print("\t");
//  Serial.print(led2Val);
//  Serial.print("\t");
//  Serial.print(led3Valled4Val);
//  Serial.print("\t");
//  Serial.print(led4Val);
//  Serial.print("\n");

}
