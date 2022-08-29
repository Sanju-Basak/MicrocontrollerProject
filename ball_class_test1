


//color_ix=1//red
//color_ix=2//green
//color_ix=3//blue
class ball{
  int color_ix;
  float weight;
  public:
  ball(int c,float w){
    color_ix=c;
    weight=w; 
  }
  int getcolor(){
    return color_ix;
  }
  float getweight(){
    return weight;
  }
  
};





ball balls[]={ball(1,2.5),ball(3,1.58),ball(2,1.45),ball(1,6.5)};

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  int b;
  Serial.println("input");        //Prompt User for input
  while (Serial.available()==0){}             // wait for user input
  b= Serial.parseInt();
  if(b!=0){
     Serial.print(balls[b-1].getcolor());
  Serial.print(" ");
  Serial.println(balls[b-1].getweight());
  }
  
  // put your main code here, to run repeatedly:

}
