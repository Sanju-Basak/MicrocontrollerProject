// (c) Michael Schoeffler 2017, http://www.mschoeffler.de
#include <Servo.h>
#include <HX711_ADC.h> // https://github.com/olkal/HX711_ADC
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // LiquidCrystal_I2C library

Servo servo1;
Servo servo2;


#define S0 7 //changed from 4-10&& 5-9
#define S1 8
#define S2 9
#define S3 10
#define sensorOut 6

#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

HX711_ADC LoadCell(3, 2); // parameters: dt pin, sck pin<span data-mce-type="bookmark" style="display: inline-block; width: 0px; overflow: hidden; line-height: 0;" class="mce_SELRES_start"></span>
LiquidCrystal_I2C lcd(0x3F, 16, 2); // 0x27 is the i2c address of the LCM1602 IIC v1 module (might differ)

const int calVal_eepromAdress = 0;
const int tareOffsetVal_eepromAdress = 4;
unsigned long t = 0;
int red;
int green;
int blue;
int redf;
int greenf;
int bluef;
int pos1;
int pos2;


void setup() {
  servo1.attach(12);
  servo2.attach(4);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  // Set Sensor output as input
  pinMode(sensorOut, INPUT);

  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  Serial.begin(9600);
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(200.0); // calibration factor for load cell => strongly dependent on your individual setup
  lcd.begin(); // begins connection to the LCD module
  lcd.backlight(); // turns on the backlight
  lcd.print("hello");
}

void loop() {
  for(pos1=0;pos1<90;pos1++){
    LoadCell.update();
    servo1.write(pos1);
    delay(10);
  }
  for(pos2=0;pos2<90;pos2++){
    LoadCell.update();
    servo2.write(pos2);
    delay(10);
  }

  //delay(100);
  
  LoadCell.update(); // retrieves data from the load cell
  float i = LoadCell.getData(); // get output value
  lcd.clear();
  lcd.setCursor(0, 0); // set cursor to first row
 // lcd.print("Weight[g]:"); // print out to LCD
  //lcd.setCursor(0, 1); // set cursor to secon row
  lcd.print(i); // print out the retrieved value to the second row


  for(int i=0;i<3;i++){
    LoadCell.update();
    colorDetect();
    LoadCell.update();
    redf+=red;
    greenf+=green;
    bluef+=blue;
  }
  redf=redf/4;
  greenf=greenf/4;
  bluef=bluef/4;
  LoadCell.update();
  delay(100);
  //lcd.setCursor(0, 1);
  if(redf<greenf && redf<bluef){
    //lcd.clear();
    lcd.clear();
    lcd.setCursor(0,1);
    
    lcd.print("R");
    lcd.print(redf);
    lcd.print("_");
    lcd.print(greenf);
    lcd.print("_");
    lcd.print(bluef);
    
    Serial.println("r");
  }

  else if(greenf<redf && greenf<bluef){
    //lcd.clear();
    lcd.clear();
    lcd.setCursor(0,1);
    
    lcd.print("G");
    lcd.print(redf);
    lcd.print("_");
    lcd.print(greenf);
    lcd.print("_");
    lcd.print(bluef);
    Serial.println("g");
  }
  else if(bluef<redf&&bluef<greenf){
    //lcd.clear();
   // lcd.setCursor(0, 1);
   lcd.clear();
  lcd.setCursor(0,1);
    lcd.print("B");
    lcd.print(redf);
    lcd.print("_");
    lcd.print(greenf);
    lcd.print("_");
    lcd.print(bluef);
    Serial.println("b");
    
  }
  else{
    //lcd.clear();
    //lcd.setCursor(0, 1);
    lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("un");
  lcd.print(redf);
  lcd.print("_");
    lcd.print(greenf);
    lcd.print("_");
    lcd.print(bluef);
    
  }
  LoadCell.update();
  for(;pos1>0;pos1--){
    servo1.write(pos1);
    LoadCell.update();
    delay(10);
  }
  for(;pos2>0;pos2--){
    servo2.write(pos2);
    LoadCell.update();
    delay(10);
  }
  delay(200);
  lcd.clear();
  

  
}

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
