#include <Servo.h>. 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

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

int width;
int height;

boolean run = false;
boolean rechtsNaarLinks = true;

int huidigeWaarde = lowDegree;
const int delta = 1;
int buttonState = 0; 
const int buttonPin = 2;     // the number of the pushbutton pin


// Variables for the duration and the distance
long duration;
int distance;

Servo myServo; // Creates a servo object for controlling the servo motor

void setup(){
  Serial.begin(9600);
  pinMode(buttonPin, INPUT); // initialize the pushbutton pin as an input:
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledblue, OUTPUT);
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);
  myServo.attach(12); // Defines on which pin is the servo motor attached

  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light
}


void loop() {
  while(!run){
      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH) {
        run = true;
        delay(200);
      }
  }

  while(run) {
      if (rechtsNaarLinks && huidigeWaarde < highDegree) 
      {
        huidigeWaarde += delta;
      } 
      else if (rechtsNaarLinks && huidigeWaarde == highDegree) 
      {
        rechtsNaarLinks = false;
      } 
      else if (!rechtsNaarLinks && huidigeWaarde > lowDegree) 
      {
        huidigeWaarde -= delta;
      } 
      else if (!rechtsNaarLinks && huidigeWaarde == lowDegree) 
      {
        rechtsNaarLinks = true;
        
      }
      else {
        // error
      }
      askStateButton();
      myServo.write(huidigeWaarde);
      delay(30);
      distance = calculateDistance();
      Serial.print(huidigeWaarde); // Sends the current degree into the Serial Port
      Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
      Serial.print(distance); // Sends the distance value into the Serial Port
      Serial.print("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
      delay(30);
      
  }
}

void askStateButton() {
  buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      run = false;
      delay(200);
    }
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

  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Degree: ");
  lcd.setCursor(8,0); 
  lcd.print(huidigeWaarde);
  lcd.setCursor(0,1); 
  lcd.print("Distance: ");
  lcd.setCursor(9,1);
  lcd.print(distance);
  lcd.setCursor(13,1);
  lcd.print("cm");
  delay(100);
  
  
  return distance;
}
