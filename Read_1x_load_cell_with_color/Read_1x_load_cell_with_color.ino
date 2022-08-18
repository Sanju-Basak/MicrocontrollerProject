
#include <Servo.h>

Servo myservo1; 
Servo myservo2; 

#include <Wire.h>
#include <LiquidCrystal_I2C.h> // LiquidCrystal_I2C library


#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

#define S0 10 //changed from 4-10&& 5-9
#define S1 9
#define S2 6
#define S3 7
#define sensorOut 8

//pins:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin

LiquidCrystal_I2C lcd(0x3F, 16, 2);


int pos1;    // variable to store the servo position
int pos2;
int servo1pin=12;
int servo2pin=11;

//color
int red = 0;
int green = 0;
int blue = 0;

bool colorflag=false;

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;


int deg=0;
int offset=0;

void setup() {

  myservo1.attach(13);// attaches the servo on pin 9 to the servo object
  myservo2.attach(12); 
  //color

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  // Set Sensor output as input
  pinMode(sensorOut, INPUT);

  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  //loadcell
  //Serial.begin(57600); delay(10);
  Serial.begin(9600); delay(10);
  
 

  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(200.0); // calibration factor for load cell => strongly dependent on your individual setup

  //loadcell
  lcd.begin(); // begins connection to the LCD module
  lcd.backlight(); // turns on the backlight
}


void loop() {


  

  //Serial.println("no color but white");

  //servo1 and 2 to initial pos
  myservo1.write(0);
  myservo2.write(90);
  


  //servo1 to move on color sensor
  for (pos1 = 0; pos1 < 60; pos1 += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
        delay(20);                       // waits 15ms for the servo to reach the position
      }
  


  //color detect

  
  colorDetect();
  delay(100);
  LoadCell.update(); // retrieves data from the load cell
  float i = LoadCell.getData(); // get output value
  delay(200);
  
  lcd.setCursor(0, 0); // set cursor to first row
  //redPw
  if(red<green && red<blue){
    lcd.print("RED");
    deg=-30;
    Serial.println("r");
  }
  else if(green<red && green<blue){
    
    lcd.print("GREEN");
    Serial.println("g");
  }
  else if(blue<red&&blue<green){
   
    lcd.print("BLUE");
    Serial.println("b");
    deg=30;
  }

  

  lcd.setCursor(0, 1); // set cursor to secon row
  lcd.print("w: ");
  lcd.print(i); // print out the retrieved value to the second row

  if(i>20){
    offset=20;
    
  }
  else{
    offset=-20;
    
  }
  int des=deg+offset+90;

  //servo2 to des
  if(deg>0){
    for (pos2 = 90; pos1 < des; pos1 += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
        delay(20);                       // waits 15ms for the servo to reach the position
      }
    
    
  }
  else{
    for (pos2 = 90; pos2 > des; pos2 -= 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
        delay(20);                       // waits 15ms for the servo to reach the position
      }
    
  }

  //servo1 to release

  for (; pos1 < 100; pos1 += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo1.write(pos2);              // tell servo to go to position in variable 'pos'
        delay(20);                       // waits 15ms for the servo to reach the position
      }

  //
  delay(100);
  //servo1 to reset
  for (; pos1 > 0; pos1 -= 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo1.write(pos2);              // tell servo to go to position in variable 'pos'
        delay(20);                       // waits 15ms for the servo to reach the position
      }

  //servo2 to reset

  if(deg>0){
    for (; pos1 > 90; pos1 -= 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
        delay(20);                       // waits 15ms for the servo to reach the position
      }
    
    
  }
  else{
    for (; pos2 <90; pos2 += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
        delay(20);                       // waits 15ms for the servo to reach the position
      }
    
  }


  

  

}



//color detect func

void colorDetect(){
  // Read Red Pulse Width
  red = getRedPW();
  // Delay to stabilize sensor
  delay(200);
  
  // Read Green Pulse Width
  green = getGreenPW();
  // Delay to stabilize sensor
  delay(200);
  
  // Read Blue Pulse Width
  blue = getBluePW();
  // Delay to stabilize sensor
  delay(200);
  
  // Print output to Serial Monitor
  Serial.print("Red PW = ");
  Serial.print(red);
  Serial.print(" - Green PW = ");
  Serial.print(green);
  Serial.print(" - Blue PW = ");
  Serial.println(blue);

  
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
