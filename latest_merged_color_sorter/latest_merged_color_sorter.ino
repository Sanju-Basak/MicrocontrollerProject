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

//const int calVal_eepromAdress = 0;
//const int tareOffsetVal_eepromAdress = 4;
unsigned long t = 0;
/*int red;
int green;
int blue;
int redf;
int greenf;
int bluef;*/
int pos1;
int pos2;

int deg= 0;
int offset= 0;

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;
  

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
  delay(20);
  lcd.clear();
  pos1=0;
  pos2=0;

  
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

int r=0;
int g=0; 
int b=0;
  //servo1 and 2 to initial pos
 for(;pos1<90;pos1++){
    LoadCell.update();
    servo1.write(pos1);
    delay(10);
  }
  delay(1000);
  for(;pos2<90;pos2++){
    LoadCell.update();
    servo2.write(pos2);
    delay(10);
  }
  
  
  for(;pos1>40;pos1--){
    LoadCell.update();
    servo1.write(pos1);
    delay(10);
  }
  delay(800);
  /*for(int i=0;i<1;i++){
    colorDetect();
   delay(100);
   LoadCell.update();
   }*/
   delay(1000);
   for(int i=0;i<10;i++){
    colorDetect();
     delay(100);
     LoadCell.update();
     r=r+redColor;
     g=g+greenColor;
     b=b+blueColor;
   }
   redColor=r/10;
   greenColor=g/10;
   blueColor=b/10;
   

  //delay(100);
  deg=0;
  offset=0;
  


  //color detect
   
   
    // retrieves data from the load cell
  float i = LoadCell.getData(); // get output value
  delay(200);
  
  lcd.setCursor(0, 0);
  if(redColor <-117 && redColor> -232){
      Serial.println(" - RED detected!");
      lcd.print("RED");
      deg= -60;
  }
  else if(redColor <28 && redColor> -39){
    Serial.println(" - GREEN detected!");
    lcd.print("GREEN");
    deg= 0;
  }
  else {
    Serial.println(" - BLUE detected!");
    lcd.print("BLUE");
    deg= 60;
  }

  lcd.setCursor(0, 1); // set cursor to secon row
  lcd.print("w: ");
  lcd.print(i); // print out the retrieved value to the second row

  if(i>5){
    offset=20;
    
  }
  else{
    offset=-20;
    
  }
  int dest=deg+offset+90;

   //servo2 to des
  if(dest>90){
    for(;pos2<dest;pos2++){
    servo2.write(pos2);
    LoadCell.update();
    delay(10);
    }
    
  }
  else{
    for(;pos2>dest;pos2--){
    servo2.write(pos2);
    LoadCell.update();
    delay(10);
    }
    
  }
  
  
  LoadCell.update();
  for(;pos1>0;pos1--){
    servo1.write(pos1);
    LoadCell.update();
    delay(10);
  }
  
  delay(1000);
  
  if(dest>90){
    for(;pos2>90;pos2--){
    servo2.write(pos2);
    LoadCell.update();
    delay(10);
    }
    
  }
  else{
    for(;pos2<90;pos2++){
    servo2.write(pos2);
    LoadCell.update();
    delay(10);
    }
    
  }
  delay(200);
  lcd.clear();
  

}

void colorDetect(){
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // redColor = map(redFrequency, 70, 120, 255,0);
  redColor = map(redFrequency, 140, 200, 255,0);
  
  // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redColor);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // greenColor = map(greenFrequency, 100, 199, 255, 0);
  greenColor = map(greenFrequency, 206, 234, 255, 0);
  
  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greenColor);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // blueColor = map(blueFrequency, 38, 84, 255, 0);
  blueColor = map(blueFrequency, 75, 235, 255, 0);
  
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.print(blueColor);
  delay(100);

  // Checks the current detected color and prints
  // a message in the serial monitor
  /*if(redColor > greenColor && redColor > blueColor){
      Serial.println(" - RED detected!");
  }
  if(greenColor > redColor && greenColor > blueColor){
    Serial.println(" - GREEN detected!");
  }
  if(blueColor > redColor && blueColor > greenColor){
    Serial.println(" - BLUE detected!");
  }*/
    if(redColor <-117 && redColor> -232){
      Serial.println(" - Green detected!");
  }
  else if(redColor <28 && redColor> -39){
     Serial.println(" - Red detected!");
  }else{
     Serial.println(" - Blue detected!");
  }
}
