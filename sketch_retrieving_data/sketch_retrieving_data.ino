//library for servomotor
#include <Servo.h>. 
//LCD libraries
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

//constant values 
const int led=13;
const int tx=1;
const int rx=0;

const int buttonPin = 2;     // the number of the pushbutton pin
const int delta = 1;

// Defines the Trigger and Echo pins of the Ultrasonic Sensor
const int trigPin = 10;
const int echoPin = 11;

// Constant degrees
const int lowDegree = 10; //here we can choose our degrees
const int highDegree = 170;

//booleans
boolean run = false;
boolean rightToLeft = true;

int currentValue = lowDegree;
int buttonState = 0; 
int bluetoothState;
char inSerial[15];

// Variables for the duration and the distance
long duration;
int distance;

Servo myServo; // Creates a servo object for controlling the servo motor

void setup()
{
  Serial.begin(9600); // start serial communication , 9600 bitrate
  pinMode(buttonPin, INPUT); // initialize the pushbutton pin as an input:
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(led, OUTPUT);
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);
  myServo.attach(12); // Defines on which pin is the servo motor attached

  lcd.begin(16,2); //Defining 16 columns and 2 rows of lcd display
  lcd.backlight(); //To Power ON the back light
}


void loop() 
{
  while(!run) //endless loop whole buttonState is low, when we push the button state->high and goes out the while loop
  {
       buttonState = digitalRead(buttonPin); //reads the button states
       
       if(Serial.available() > 0)
       {     
            bluetoothState = Serial.read();
       }
  
        if (bluetoothState == '1') 
        {
            digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
            run = true;
        }
       
        else if (bluetoothState == '2') 
        {
            digitalWrite(led, LOW);
            
        }    // turn the LED off by making the voltage LOW
            
        if (buttonState == HIGH) 
        {
            bluetoothState = 1;
            run = true;
            delay(50);
        }
  }

  while(run) //if run true, start ascend and descend in degrees
  {
      if (rightToLeft && currentValue < highDegree) // if the sensor goes from right to left and the currentValue is lower than the highest degree (170) then we work in this if
      {
        currentValue += delta;
      } 
      else if (rightToLeft && currentValue == highDegree) 
      {
        rightToLeft = false;
      } 
      else if (!rightToLeft && currentValue > lowDegree) 
      {
        currentValue -= delta;
      } 
      else if (!rightToLeft && currentValue == lowDegree) 
      {
        rightToLeft = true;
        
      }
      else 
      {
       //error 
      }
      
      askStateButton();
      
      myServo.write(currentValue);
      delay(30);
      distance = calculateDistance();
      
      Serial.print(currentValue); // Sends the current degree into the Serial Port
      Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
      Serial.print(distance); // Sends the distance value into the Serial Port
      Serial.print("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  }
}

void askStateButton() 
{
  buttonState = digitalRead(buttonPin);
      if(Serial.available() > 0)
      {     
          bluetoothState = Serial.read();
      }

      if (bluetoothState == '1')
      {
          digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
          run = false;
      }
     
      else if (bluetoothState == '2') 
      {
          digitalWrite(led, LOW);
          
      }    // turn the LED off by making the voltage LOW
          
      if (buttonState == HIGH) 
      {
          bluetoothState = 1;
          run = false;
          delay(50);
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
  
   
    
    if(currentValue == 20 || currentValue == 40 || currentValue == 60 || currentValue == 80  || currentValue == 100 || currentValue == 120 || currentValue == 140 || currentValue == 160)
    {
          lcd.clear();
          lcd.setCursor(0,0); 
          lcd.print("Degree: ");
          lcd.setCursor(8,0); 
          lcd.print(currentValue);
          lcd.setCursor(0,1); 
          lcd.print("Distance: ");
          lcd.setCursor(9,1);
          lcd.print(distance);
          lcd.setCursor(13,1);
          lcd.print("cm");
          delay(30);
    }
  
  
    return distance;
}
