#include <Servo.h>. 

int ledblue=13;
int tx=1;
int rx=0;
char inSerial[15];

// Defines the Trigger and Echo pins of the Ultrasonic Sensor
const int trigPin = 10;
const int echoPin = 11;
// Constant degrees
const int lowDegree = 10; //here we can choose our degrees
const int highDegree = 170;
// Asking for user input
int width;
int height;

// Variables for the duration and the distance
long duration;
int distance;
Servo myServo; // Creates a servo object for controlling the servo motor


void setup(){
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledblue, OUTPUT);
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);
  myServo.attach(12); // Defines on which pin is the servo motor attached
}

void loop(){
  int i=0;
  int m=0;
  delay(500);
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      inSerial[i]=Serial.read();
      i++;
    }
    inSerial[i]='\0';
    if(!strcmp(inSerial,"2off")){ //Led Off
      digitalWrite(ledblue, LOW);
      for(m=0;m<11;m++){
        inSerial[m]=0;
      }
      i=0;
    }
    if(!strcmp(inSerial,"2on")){ //Led on
      digitalWrite(ledblue, HIGH);
      for(int i=lowDegree;i<=highDegree;i++)
      {  
        myServo.write(i);
        delay(30);
        distance = calculateDistance();// Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
        printSerial(i); 
        delay(30);
      }
      // Repeats the previous lines from highDegree to lowDegree
      for(int i=highDegree;i>lowDegree;i--)
      {  
        myServo.write(i);
        delay(30);
        distance = calculateDistance();
        printSerial(i);
        delay(30);
      }
      for(m=0;m<11;m++){
        inSerial[m]=0;
      }
      i=0;
    }
    else{
      for(m=0;m<11;m++){
        inSerial[m]=0;
      }
      i=0;
    }
  }
}

void printSerial(int i)
{
  
  Serial.print(i); // Sends the current degree into the Serial Port
  Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  Serial.print(distance); // Sends the distance value into the Serial Port
  Serial.print("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
}

// Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance()
{ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance= duration*0.034/2;
  return distance;
}
