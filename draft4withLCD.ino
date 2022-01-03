//pins used ArdMega: 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 14, 15, A0

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int moisturePin = A0; 
int moistureValue;  
int limit = 300; 
int en=11;
int pumpMotorForward = 14, pumpMotorbackward = 15;  
  //int enA=12;// Enable A   //int enB=13;// Enable B
const int YstepPin = 3, YdirPin = 4; 
const int ZstepPin = 1, ZdirPin = 2; 
  int leftMotorForward = 11;   //IN3
  int rightMotorForward = 7; //IN1  
  int leftMotorBackward = 12;  //IN4 
  int rightMotorBackward = 9; //IN2


int pumpTime, XUnit= (7000/12), YUnit = 1000, ZUnit = 1000; //calibrate

String ArdInput, s, prevLocation, newLocation;
double newX, newY, newZ, prevX, prevY, prevZ, moveXby, moveYby, moveZby; //all coordinates are units of grid lengths
char mode, func; 




void setup() {
  lcd.init();// initialize the lcd 
  
  lcd.backlight();
  Serial.begin(9600);
  //pinMode(13,OUTPUT);
  // Sets the two pins as Outputs
  pinMode(YstepPin,OUTPUT); 
  pinMode(YdirPin,OUTPUT);  
  //pinMode(enA, OUTPUT);
  //pinMode(enB, OUTPUT);
  pinMode(leftMotorForward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT); 
  pinMode(leftMotorBackward, OUTPUT);  
  pinMode(rightMotorBackward, OUTPUT);
  
  pinMode(pumpMotorForward, OUTPUT);  
  pinMode(pumpMotorbackward, OUTPUT); 
 
}//end of setup()

void loop() {

  String ArdInput = "ID00"; //ideal state initialisation
  String prevLocation="X00Y00Z00Q00"; //initially hardcoded to Home
    

  
  if(Serial.available()==1)
  {
    
    //ArdInput = Serial.readStringUntil('\n');
    //ArdInput = ""
    ArdInput = "MV00X12Y00Z00Q00"; //command to (M)ove 
    //ArdInput = "WP05";//command to pump water
    //ArdInput = "PH02"//command to pump water
    
    lcd.setCursor(0,0);
    lcd.print("Message Recieved:");
    lcd.setCursor(0,1);
    lcd.print(ArdInput);
    //lcd.setCursor(0,1);
    //lcd.print("");
    
    Serial.println("Message Recieved: ");
    Serial.println(ArdInput);    
    char Mode = ArdInput.charAt(0);
    Serial.println(Mode); //use if to direct to functions M- move first, then P - Pump
    //Serial.println(Operation);
    
    if(Mode == 'M')//&& prevLocation != newLocation
    {          
      Serial.println("Move Command:");
      
      Serial.println("old location:");
      Serial.println(prevLocation);
      Serial.println("new location:");
      newLocation = ArdInput.substring(4,16); // formatX00Y00Z00Q00
      Serial.println(newLocation);
      
      String newXS = newLocation.substring(1,3);      newX =newXS.toDouble();
      String prevXS = prevLocation.substring(1,3);      prevX =prevXS.toDouble();
      Serial.println("moving X by:"); 
      lcd.setCursor(-4,2);    
      lcd.print("moving X by:");
      Serial.println(newX-prevX);
      lcd.setCursor(-4,3);
      lcd.print(newX-prevX);
      moveXby = newX - prevX;
      xaxis(moveXby);


      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Message Recieved");
      lcd.setCursor(0,1);
      lcd.print(ArdInput);
      
      String newYS = newLocation.substring(4,6);      newY =newYS.toDouble();
      String prevYS = prevLocation.substring(4,6);      prevY =prevYS.toDouble();
      Serial.println("moving Y by:");     
      Serial.println(newY-prevY);
      lcd.setCursor(-4,2);
  lcd.print("Moving Y by");
  lcd.setCursor(-4,3);
  lcd.print(moveYby);
      moveYby = newY - prevY;
      yaxis(moveYby);

lcd.clear();

lcd.setCursor(0,0);
  lcd.print("Message Recieved");
  lcd.setCursor(0,1);
  lcd.print(ArdInput);
      String newZS = newLocation.substring(7,9);      newZ =newZS.toDouble();
      String prevZS = prevLocation.substring(7,9);      prevZ =prevZS.toDouble();
      Serial.println("moving Z by:");     
      Serial.println(newZ-prevZ);
      lcd.setCursor(-4,2);
      lcd.print("Moving Z by");
      lcd.setCursor(-4,3);
      lcd.print(moveZby);
      moveZby = newZ - prevZ;
      zaxis(moveZby);
      
      //move from prevLocation to newLocation using functions
      //prevLocation = newLocation;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ArdDone confirmation"); 
      Serial.println("arddone"); // notifies the RPi that arduino command is done
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
      Serial.println("arddone"); // notifies the RPi that arduino command is done
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
  delay(200);//200ms buffer 
  if(XT>0) // moving forward
  { 
  Serial.println("Moving Forward for time:");
  Serial.println(XT*XUnit/1000);
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
      digitalWrite(YdirPin,LOW); //Clockwise Direction
      Serial.println("Moving Right");
      for(int x = 0; x < YTN ; x++)
        { 
        digitalWrite(YstepPin,HIGH);
        delayMicroseconds(500);
        digitalWrite(YstepPin,LOW);
        delayMicroseconds(500);
        }
   }
   else if (YT<0)
   {
      int YTN = abs(YT*YUnit);
      digitalWrite(YdirPin,HIGH); //Anti-Clockwise Direction
      Serial.println("Moving Left");
       for(int x = 0; x <YT; x++) 
       {
         digitalWrite(YstepPin,HIGH); 
         delayMicroseconds(500); 
         digitalWrite(YstepPin,LOW); 
         delayMicroseconds(500); 
        }
    }
}//end of y-axis




void zaxis(int ZT)
{
   if (ZT>0)
   {
      int ZTN = ZT*ZUnit;
      Serial.println("Moving down ");
      //Serial.println(ZT/1000);
      digitalWrite(ZdirPin,LOW); //Clockwise Direction
      
      for(int z = 0; z < ZTN ; z++)
        { 
        digitalWrite(ZstepPin,HIGH);
        delayMicroseconds(500);
        digitalWrite(ZstepPin,LOW);
        delayMicroseconds(500);
        }
   }
   else if (ZT<0)
   {
      int ZTN = abs(ZT*ZUnit);
      Serial.println("Moving up ");
      //Serial.println(ZT/1000);
      digitalWrite(ZdirPin,HIGH); //Anti-Clockwise Direction
      
       for(int z = 0; z <ZTN; z++) 
       {
         digitalWrite(ZstepPin,HIGH); 
         delayMicroseconds(500); 
         digitalWrite(ZstepPin,LOW); 
         delayMicroseconds(500); 
        }
    }
}//end of z-axis



void pump(int pumpTime)
{
  Serial.println("Pumping water for (s):");
  Serial.println(pumpTime);
  digitalWrite(en,HIGH);
  digitalWrite(pumpMotorForward,HIGH);
  digitalWrite(pumpMotorbackward,LOW);
  delay(pumpTime*1000);  
  
  digitalWrite(pumpMotorForward,LOW);//stop pumping
  digitalWrite(pumpMotorbackward,LOW);
  delay(1000);
  //digitalWrite(en,LOW);
}//end of pump() function

void photo(int photoDuration)
{
  Serial.println("Photo being taken"); 
  delay(photoDuration*1000);
}

void moisture()
{
      moistureValue = analogRead(moisturePin); 
      Serial.println("Moisture Value : ");
      Serial.println(moistureValue);
      if (moistureValue<limit)
      {
      }
      else
      {
      }
}
