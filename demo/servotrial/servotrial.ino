/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos;    // variable to store the servo position
int in1=5;
int in2=6;

void setup() {
  myservo.attach(13);// attaches the servo on pin 9 to the servo object
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
}

void loop() {
  digitalWrite(in1,HIGH);
digitalWrite(in2,LOW);

digitalWrite(in1,LOW);
digitalWrite(in2,LOW);
  //myservo.write(0);
   
  myservo.write(90); 
  //delay(1000);
  
  
  
  
  for (pos = 90; pos >0; pos -= 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(10);                       // waits 15ms for the servo to reach the position
      }
      delay(1000);
  //delay(1500); 
  for(; pos<90;pos++){
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10); 
  }

  /*delay(1500);
  for(; pos<90;pos++){
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10); 
  }
  delay(1500);
  for (; pos > 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }*/


delay (3000);

delay(1000);
}
