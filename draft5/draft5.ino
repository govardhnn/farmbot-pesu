//pins used ArdMega: 1, 2, 3, 4, 5, 6, 7, 8, 14, 15, 20, 21, A0

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27

int moisturePin = A0, moistureValue;  
int en=11;
int pumpMotorForward = 14, pumpMotorBackward = 15;  
  //int enA=12;// Enable A   //int enB=13;// Enable B
  int YPin1 = 5, YPin2 = 6; 
int ZPin1 = 8, ZPin2 = 10; 
  int leftMotorForward = 11;   //IN3
  int rightMotorForward = 7; //IN1  
  int leftMotorBackward = 12;  //IN4 
  int rightMotorBackward = 9; //IN2


int pumpTime, XUnit= (7000/12), YUnit = (7000/2), ZUnit = 1000; //calibrate

String ArdInput, s, newLocation;
double newX, newY, newZ, prevX, prevY, prevZ, moveXby, moveYby, moveZby; //all coordinates are units of grid lengths
char mode, func; 
String prevLocation="X00Y00Z00Q00"; //initially hardcoded to Home



void setup() {
  lcd.init();// initialize the lcd 
  
  lcd.backlight();
  Serial.begin(9600);
  //pinMode(13,OUTPUT);
  pinMode(leftMotorForward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT); 
  pinMode(leftMotorBackward, OUTPUT);  
  pinMode(rightMotorBackward, OUTPUT);
  pinMode(YPin1,OUTPUT); 
  pinMode(YPin2,OUTPUT);  
  pinMode(ZPin1,OUTPUT); 
  pinMode(ZPin2,OUTPUT);  
  //pinMode(enA, OUTPUT);
  //pinMode(enB, OUTPUT);  
  
  pinMode(pumpMotorForward, OUTPUT);  
  pinMode(pumpMotorBackward, OUTPUT); 
 
}//end of setup()

void loop() {

 // String ArdInput = "ID00"; //ideal state initialisation
 
    

  
  if(Serial.available()>0)
  { 
    
    ArdInput = Serial.readStringUntil('\n');
    //ArdInput = ""
    //ArdInput = "MV00X12Y03Z05Q00"; //command to (M)ove 
    //ArdInput = "WP05";//command to pump water
    //ArdInput = "PH02"//command to pump water
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Message Recieved"); delay(1000); //between each command
    lcd.setCursor(0,1);
    lcd.print(ArdInput);    
    Serial.println("Message Recieved ");
    Serial.println(ArdInput);  
      
    char Mode = ArdInput.charAt(0); 
    Serial.println(prevLocation);
    Serial.println(newLocation);
    newLocation = ArdInput.substring(4,16);
    Serial.println(Mode); //use if to direct to functions M- move first, then P - Pump
    Serial.println(prevLocation);
    Serial.println(newLocation);
    if( prevLocation == newLocation)
    {
      Serial.println("Z Done");
    }

    if(Mode == 'M' && prevLocation != newLocation)
    {          
      Serial.println("Move Command");      
      Serial.println("Old location");
      Serial.println(prevLocation);
      Serial.println("New location");
      newLocation = ArdInput.substring(4,16); // formatX00Y00Z00Q00
      Serial.println(newLocation);
      
      String newXS = newLocation.substring(1,3);      newX =newXS.toDouble();
      String prevXS = prevLocation.substring(1,3);      prevX =prevXS.toDouble();
      
      Serial.println("moving X by"); 
      Serial.println(newX-prevX);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Message Recieved");
      lcd.setCursor(0,1);
      lcd.print(ArdInput);    
      
      
      
      moveXby = newX - prevX;
      xaxis(moveXby);
      Serial.println("XDONE");

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Message Recieved");
      lcd.setCursor(0,1);
      lcd.print(ArdInput);
      
      String newYS = newLocation.substring(4,6);      newY =newYS.toDouble();
      String prevYS = prevLocation.substring(4,6);      prevY =prevYS.toDouble();
      Serial.println("moving Y by");     
      Serial.println(newY-prevY);     
      moveYby = newY - prevY;
      yaxis(moveYby);
      Serial.println("YDONE");

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Message Recieved");
      lcd.setCursor(0,1);
      lcd.print(ArdInput);
      
      String newZS = newLocation.substring(7,9);      newZ =newZS.toDouble();
      String prevZS = prevLocation.substring(7,9);      prevZ =prevZS.toDouble();
      
      Serial.println("moving Z by");     
      Serial.println(newZ-prevZ);
      
      
      
      moveZby = newZ - prevZ;
      zaxis(moveZby);
      Serial.println("Z Done");
      Serial.println("ARDDONE");
      lcd.print("ZDONE"); 

      //move from prevLocation to newLocation using functions
      Serial.println("Changing Prev Location = New Location");
      prevLocation = newLocation;
      lcd.clear();
      lcd.setCursor(0,0);
    }

    if(Mode == 'W')//watering
    {

      lcd.setCursor(0,0);
      lcd.print("Message Recieved");
      lcd.setCursor(0,1);
      lcd.print(ArdInput);
      String pumpS = ArdInput.substring(2,4);   // PumpS is directly the no of seconds water pumps    
      int pumpT = pumpS.toDouble();
      pump(pumpT); //pumpT is in unit: seconds
      lcd.setCursor(-4,2);
      lcd.print("Pumping Water for");
      lcd.setCursor(-4,3);
      lcd.print(pumpT);
      lcd.clear();
      lcd.setCursor(-4,3);
      lcd.print("ArdDone Confirmation");
      Serial.println("WDONE"); // notifies the RPi that arduino command is done
    }

    if(Mode == 'P')//photo
    {
      String photoS = ArdInput.substring(2,4);
      int photoT = photoS.toDouble();
      photo(photoT);
      Serial.println("arddone"); // notifies the RPi that arduino command is done
    }

    if(Mode == 'S')//sensor moisture
    {
      moisture();
    }
  }  
}



void xaxis(int XT)
{
  
  if(XT>0) // moving forward
  { 
  Serial.println("Moving Forward for time");
  Serial.println(XT*XUnit/1000);
  lcd.setCursor(-4,2);    
  lcd.print("Moving X Forward for");      
  lcd.setCursor(-4,3);
  lcd.print(XT*XUnit/1000);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward,LOW);
  //analogWrite(enA, 235);
  
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,HIGH); 
  //analogWrite(enB, 225);
  delay(XT*XUnit);
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW); 
  } 

  if(XT<0)
  { 
  Serial.println("Moving Backward for time");
  lcd.setCursor(-4,2);    
  lcd.print("Moving X Backward for");      
  lcd.setCursor(-4,3);
  lcd.print(abs(XT*XUnit/1000));
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,HIGH);
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(leftMotorBackward,LOW);  
  delay(abs(XT)*XUnit);
  
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW);
  }

  
 
}//end of x-axis function()

void yaxis(int YT)
{
   if (YT>0)
   {
      int YTN = YT*YUnit;
      Serial.println("Moving Right");
      digitalWrite(YPin1, HIGH);
      digitalWrite(YPin2, LOW);
      
      lcd.setCursor(-4,2);
      lcd.print("Moving Y Right");
      lcd.setCursor(-4,3);
      lcd.print(moveYby*YUnit/1000);
      
      delay(YTN);
      digitalWrite(YPin1, LOW);
      digitalWrite(YPin2, LOW);
      
   }
   else if (YT<0)
   {
      int YTN = abs(YT*YUnit);
      Serial.println("Moving Left");
      digitalWrite(YPin1, LOW);
      digitalWrite(YPin2, HIGH);

      lcd.setCursor(-4,2);
      lcd.print("Moving Y Left");
      lcd.setCursor(-4,3);
      lcd.print(moveYby);
      
      delay(abs(YT)*YUnit);
      digitalWrite(YPin1, LOW);
      digitalWrite(YPin2, LOW);
    }
}//end of y-axis




void zaxis(int ZT)
{
    if (ZT>0)
   {
      int ZTN = ZT*ZUnit;
      Serial.println("Moving Up");
      digitalWrite(ZPin1, HIGH);
      digitalWrite(ZPin2, LOW);

      lcd.setCursor(-4,2);
      lcd.print("Moving Z up");
      lcd.setCursor(-4,3);
      lcd.print(moveZby);
      
      delay(ZTN);
      digitalWrite(ZPin1, LOW);
      digitalWrite(ZPin2, LOW);
      
   }
   else if (ZT<0)
   {
      int ZTN = abs(ZT*YUnit);
      Serial.println("Moving Down");
      digitalWrite(ZPin1, LOW);
      digitalWrite(ZPin2, HIGH);

      lcd.setCursor(-4,2);
      lcd.print("Moving Z down");
      lcd.setCursor(-4,3);
      lcd.print(moveZby);
      
      delay(abs(ZT)*ZUnit);
      digitalWrite(ZPin1, LOW);
      digitalWrite(ZPin2, LOW);
      
    }
}//end of z-axis



void pump(int pumpTime)
{
  Serial.println("Pumping water for (s)");
  Serial.println(pumpTime);
  digitalWrite(en,HIGH);
  digitalWrite(pumpMotorForward,HIGH);
  digitalWrite(pumpMotorBackward,LOW);
  delay(pumpTime*1000);  
  
  digitalWrite(pumpMotorForward,LOW);//stop pumping
  digitalWrite(pumpMotorBackward,LOW);
  delay(1000);
  //digitalWrite(en,LOW);
}//end of pump() function

void photo(int photoDuration)
{
  Serial.println("Photo being taken"); 
  delay(photoDuration*1000);
  
  lcd.setCursor(-4,2);
  lcd.print("Photo Being Taken for");
  lcd.setCursor(-4,3);
  lcd.print(photoDuration);
}

void moisture()
{
      moistureValue = analogRead(moisturePin); 
      moistureValue = map(moistureValue,550,0,0,100);
      Serial.println("Moisture Value  ");
      Serial.println(moistureValue);
      lcd.setCursor(-4,2);
      lcd.print("Moisture Level is");
      lcd.setCursor(-4,3);
      lcd.print(moistureValue);
      delay(4000);
}
