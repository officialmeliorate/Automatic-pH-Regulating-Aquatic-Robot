#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
float calibration_value = 21.34 - 0.928;
int phval = 0;
const int Pump1 = 3;
const int Pump2 = 4;
unsigned long int avgval; 
int buffer_arr[10],temp;


const int  lft_mtr1=5, lft_mtr2=6, rht_mtr1=7, rht_mtr2=8;
int forward_flag = 0, backward_flag = 0, right_flag = 0, left_flag = 0, stop_flag = 0;
int cnt=0;
String Vehicle_status= "STOP",Data;

void setup()
{
  Serial.begin(9600); 
  lcd.init(); 
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("PleCobot");
  lcd.setCursor(0, 1);
  lcd.print("Group 2");
  delay(2000);
  lcd.clear();
  
  pinMode(lft_mtr1,OUTPUT);
  pinMode(lft_mtr2,OUTPUT);   
  pinMode(rht_mtr1,OUTPUT);
  pinMode(rht_mtr2,OUTPUT);

   
  pinMode(Pump1, OUTPUT);
  pinMode(Pump2, OUTPUT);
}

void loop() 
{  
 if(Serial.available())
  {
    delay(100);
    char c=Serial.read();

    digitalWrite(lft_mtr1,LOW);
    digitalWrite(lft_mtr2,LOW);
      
    digitalWrite(rht_mtr1,LOW);
    digitalWrite(rht_mtr2,LOW);

    if(c=='F')
    {
        digitalWrite(lft_mtr1,HIGH);
        digitalWrite(lft_mtr2,LOW);
      
        digitalWrite(rht_mtr1,HIGH);
        digitalWrite(rht_mtr2,LOW);

        Vehicle_status= "FORWARD";
    }
    
    if(c=='B')
    {
        digitalWrite(lft_mtr1,LOW);
        digitalWrite(lft_mtr2,HIGH);
      
        digitalWrite(rht_mtr1,LOW);
        digitalWrite(rht_mtr2,HIGH);

        Vehicle_status= "BACKWARD";
    }

    if(c=='R')
    {
        digitalWrite(lft_mtr1,HIGH);
        digitalWrite(lft_mtr2,LOW);
      
        digitalWrite(rht_mtr1,LOW);
        digitalWrite(rht_mtr2,HIGH);
        
        Vehicle_status= "RIGHT";
    }

     if(c=='L')
    {
        digitalWrite(lft_mtr1,LOW);
        digitalWrite(lft_mtr2,HIGH);
      
        digitalWrite(rht_mtr1,HIGH);
        digitalWrite(rht_mtr2,LOW);

        Vehicle_status= "LEFT";
    }

     if(c=='S')
    {
        digitalWrite(lft_mtr1,LOW);
        digitalWrite(lft_mtr2,LOW);
      
        digitalWrite(rht_mtr1,LOW);
        digitalWrite(rht_mtr2,LOW);

        Vehicle_status= "STOP";
    }
 

 Data = "*V" + Vehicle_status +  "+*cnt" + cnt ; 
 Serial.print(Data);
 Serial.println("");
 delay(1000);      
  }

  for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6;
 float ph_act = -5.70 * volt + calibration_value;
 lcd.setCursor(0, 0);
 lcd.print("pH Val:");
 lcd.setCursor(8, 0);
 lcd.print(ph_act);
 delay(1000);

 if (ph_act <= 6.5){
  WorkPump1();
 }
 else if(ph_act >=9){
  WorkPump2();
 }
 else{
  WorkPump3();
 }
  }

 
void WorkPump1(){
  digitalWrite(Pump1, HIGH);
  digitalWrite(Pump2, LOW);
}

void WorkPump2(){
  digitalWrite(Pump1, LOW);
  digitalWrite(Pump2, HIGH);
}

void WorkPump3(){
  digitalWrite(Pump1, LOW);
  digitalWrite(Pump2, LOW);
}

 
