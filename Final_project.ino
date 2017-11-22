/*
 * CHANGE by Jade Lui and Nicole Wu using led, hc-sr04, MCP41100
 */
#include <SPI.h>

#define trigPin 7 //pin for sensor
#define echoPin 8 //pin for sensor
#define led 9 // pin for led

byte address = 0x11; //for use in dpwrite(digital potentiometer write value function)
int CS= 10;
int i=0;

void setup() {
  Serial.begin (9600);
  
  //distance sensor setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  //led setup
  pinMode(led, OUTPUT);
  
  //digipot setup
  pinMode (CS, OUTPUT);
  SPI.begin();
}

void loop() {
  float duration, distance;

  //*****distance sensor part
  
  // Clears the trigPin
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = (duration / 2) * 0.0344;

  //*****light & sound changing part
  
  if (distance >= 30 || distance <= 2 ){ //limit response area
    Serial.print("Distance = ");
    Serial.println("Out of range");
    analogWrite(led, 0); //turn off led
    dpwrite(255); //lowest resistance, playing sound all the time
  }
  else {
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println(" cm");
    analogWrite(led, 255 - distance/30 * 255); //change led brightness, the closer, the brightner
    dpwrite(distance/30 * 255); //change digipot value, the closer, higher resistance, no sound
    delay(10);
  }
  delay(10);
}

int dpwrite(int value)
{
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  SPI.transfer(value); //input value
  digitalWrite(CS, HIGH);
}
