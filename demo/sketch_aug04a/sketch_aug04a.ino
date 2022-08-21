  /*Color Sensor Calibration
  color-sensor-calib.ino
  Calibrate RGB Color Sensor output Pulse Widths
  Uses values obtained for RGB Sensor Demo sketch 
 
  DroneBot Workshop 2020
  https://dronebotworkshop.com
*/

// Define color sensor pins

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
#include <Servo.h>
Servo myservo;
// Variables for Color Pulse Width Measurements
 
int redPW = 0;
int greenPW = 0;
int bluePW = 0;
int rc=0;
int gc=0;
int bc=0; 
int pos;
void setup() {
  myservo.attach(13); 
  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Set Sensor output as input
  pinMode(sensorOut, INPUT);
  
  // Set Pulse Width scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  // Setup Serial Monitor
  Serial.begin(9600);
}
 
void loop() {
  int i=0;
  int r=0;
  int g=0;
  int b=0;

  myservo.write(60);
  while(i<10){
  // Read Red Pulse Width
  redPW = getRedPW();
  // Delay to stabilize sensor
  delay(200);
  
  // Read Green Pulse Width
  greenPW = getGreenPW();
  // Delay to stabilize sensor
  delay(200);
  
  // Read Blue Pulse Width
  bluePW = getBluePW();
  // Delay to stabilize sensor
  delay(200);

  r=r+redPW;
  g=g+greenPW;
  b=b+bluePW;
  i++;
  }

  r=r/10;
  g=g/10;
  b=b/10;
  // Print output to Serial Monitor
  Serial.print("Red PW = ");
  Serial.print(r);
  Serial.print(" - Green PW = ");
  Serial.print(g);
  Serial.print(" - Blue PW = ");
  Serial.println(b);

  if((r>=90 && r<=165)&&(g>170&& g<=300)&&(b>=110&&b<=195)){
    if(rc==1){
      for (pos = 60; pos < 120; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(10);                       // waits 15ms for the servo to reach the position
      }
      delay(1000);

      for (; pos > 60; pos -= 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(10);                       // waits 15ms for the servo to reach the position
      }
      rc=-1;
      delay(1500); 
    }
    Serial.println("RED");
    rc++;
    gc=0;
  }///red
  else if((r>=120&&r<=170)&&(g>=40&&g<=115)&&(b>=35&&b<=100)){
    Serial.println("BLUE");
  }///blue
  else if((r>=90&&r<=145)&&(g>=50&&g<=150)&&(b>=70&&b<=140)){

     if(gc==1){
      for (pos = 60; pos >0; pos -= 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(10);                       // waits 15ms for the servo to reach the position
      }
      delay(1000);

      for (; pos < 60; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(10);                       // waits 15ms for the servo to reach the position
      }
      gc=-1;
      delay(1500); 
    }
    Serial.println("GREEN");
    gc++;
    rc=0;
  }///green
  
  
}
 
 
// Function to read Red Pulse Widths
int getRedPW() {
 
  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}
 
// Function to read Green Pulse Widths
int getGreenPW() {
 
  // Set sensor to read Green only
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}
 
// Function to read Blue Pulse Widths
int getBluePW() {
 
  // Set sensor to read Blue only
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
 
}
