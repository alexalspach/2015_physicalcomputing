
int ledPin = 3;
int micPin = 0;
int micVal = 0;

const int windowSize = 50;
int data[windowSize];

int index = 0;
int total = 0;
int average = 0;



void setup() {
  // put your setup code here, to run once:

  pinMode( micPin, INPUT );
  pinMode( ledPin, OUTPUT );
  Serial.begin(9600); 

  for (int thisReading = 0; thisReading < windowSize; thisReading++)
    data[thisReading] = 0;

}

void loop() {
  // put your main code here, to run repeatedly:

  //analogWrite( ledPin, ledValue );
  //ledValue+1;
  //delay(500);
  //analogWrite( ledPin, 0 );
  //delay(500);
  
  //  for (int i=0; i <= 255; i++){
  //      analogWrite(ledPin, i);
  //      delay(5);
  //   }
  //  
  //  for (int i=255; i >= 0; i = i-5){
  //      analogWrite(ledPin, i);
  //      delay(5);
  //   }

  //  micVal = analogRead(micPin);
  //  //analogWrite( ledPin, (micVal/10) );
  //  analogWrite( ledPin, map(micVal, 0, 1023, 0, 255) );
  //  //Serial.println(micVal);   


  total = total - data[index];
  data[index] = analogRead(micPin);
  total = total + data[index];
  index++;

  if (index >= windowSize)
    index = 0;

  average = total / windowSize;

  Serial.println(average);
  delay(1);
   
  //analogWrite( ledPin, (micVal/10) );
  analogWrite( ledPin, map(average, 0, 1023, 0, 255) );
  //Serial.println(micVal);  

}
