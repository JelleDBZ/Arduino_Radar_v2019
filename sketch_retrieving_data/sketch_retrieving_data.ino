// Includes the Servo library
#include <Servo.h>. 

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

void setup() 
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  myServo.attach(12); // Defines on which pin is the servo motor attached
}

void loop() 
{
  //Asking for resolution to user
 
  // rotates the servo motor from lowDegree to highDegree
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
