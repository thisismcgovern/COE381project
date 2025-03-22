//SMARTKEMBO DISCOVERY SCIENCE
#include <LiquidCrystal.h>
#include <TimerOne.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int val;
int tempPin = A0;// temperature Sensor Pin
int HBSensor = 8;// Sensor Pin
int HBCount = 0;
int HBCheck = 0;
int TimeinSec = 0;
int HBperMin = 0;
int HBStart = 9;
int HBStartCheck = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(20, 4);
  pinMode(HBSensor, INPUT);
  pinMode(HBStart, INPUT_PULLUP);
  Timer1.initialize(800000); 
  Timer1.attachInterrupt( timerIsr );
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Current HB  : ");
  lcd.setCursor(0,1);
  lcd.print("Time in Sec : ");
  lcd.setCursor(0,2);
  lcd.print("HB per Min  : 0.0");
  lcd.setCursor(0,3);
  lcd.print("Body Temp  : ");
  
}

void loop() {
 
  
  
  if(digitalRead(HBStart) == LOW){
  //lcd.setCursor(0,3);
  //lcd.print("HB Counting ..");
HBStartCheck = 1;}
  if(HBStartCheck == 1)
  {
      if((digitalRead(HBSensor) == HIGH) && (HBCheck == 0))
      {
        HBCount = HBCount + 1;
        HBCheck = 1;
        lcd.setCursor(14,0);
        lcd.print(HBCount);
        lcd.print(" ");
      }
      if((digitalRead(HBSensor) == LOW) && (HBCheck == 1))
      {
        HBCheck = 0;   
      }
      if(TimeinSec == 10)
      {
          HBperMin = HBCount * 6;
          HBStartCheck = 0;
          lcd.setCursor(14,2);
          lcd.print(HBperMin);
          lcd.print(" ");
          //lcd.setCursor(0,3);
          //lcd.print("Press Button again.");
          HBCount = 0;
          TimeinSec = 0;      
      }
  }
  
   val = analogRead(tempPin);
  float mv = (val/1024.0)*5000;
  float cel = mv/10;
  lcd.setCursor(14,3);
          lcd.print(cel);
          lcd.print(" ");
          delay(100);
          
}

void timerIsr()
{
  if(HBStartCheck == 1)
  {
      TimeinSec = TimeinSec + 1;
      lcd.setCursor(14,1);
      lcd.print(TimeinSec);
      lcd.print(" ");
  }
}
