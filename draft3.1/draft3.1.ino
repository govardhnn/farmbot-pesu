//pins used ArdMega: 3, 4, 5, 6, 9, 10, 11, 14, 15


int en=11;
int pumpMotorForward = 14, pumpMotorbackward = 15;  
  //int enA=12;// Enable A   //int enB=13;// Enable B
const int YstepPin = 5, YdirPin = 4; 
const int ZstepPin = 1, ZdirPin = 2; 
  int leftMotorForward = 11;   //IN3
  int rightMotorForward = 7; //IN1  
  int leftMotorBackward = 12;  //IN4 
  int rightMotorBackward = 9; //IN2

int pumpTime, XUnit= 1000, YUnit = 1000, ZUnit = 1000; //calibrate

String ArdInput, s, prevLocation, newLocation;
double newX, newY, newZ, prevX, prevY, prevZ, moveXby, moveYby, moveZby; //all coordinates are units of grid lengths
char mode, func; 




void setup() {
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
    //ArdInput = "MV00X05Y03Z03Q00"; //command to (M)ove 
    ArdInput = "WP05";//command to pump water
    //ArdInput = "PH02"//command to pump water

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
      Serial.println(newX-prevX);
      moveXby = newX - prevX;
      xaxis(moveXby);

      String newYS = newLocation.substring(4,6);      newY =newYS.toDouble();
      String prevYS = prevLocation.substring(4,6);      prevY =prevYS.toDouble();
      Serial.println("moving Y by:");     
      Serial.println(newY-prevY);
      moveYby = newY - prevY;
      yaxis(moveYby);

      String newZS = newLocation.substring(7,9);      newZ =newZS.toDouble();
      String prevZS = prevLocation.substring(7,9);      prevZ =prevZS.toDouble();
      Serial.println("moving Z by:");     
      Serial.println(newZ-prevZ);
      moveZby = newZ - prevZ;
      zaxis(moveZby);
      //move from prevLocation to newLocation using functions
      //prevLocation = newLocation;
        
      Serial.println("arddone"); // notifies the RPi that arduino command is done
    }

    if(Mode == 'W')//watering
    {
      String pumpS = ArdInput.substring(2,4);   // PumpS is directly the no of seconds water pumps    
      int pumpT = pumpS.toDouble();
      pump(pumpT); //pumpT is in unit: seconds
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
      
    }
  }

  
   
  }



void xaxis(int XT)
{
  delay(200);//200ms buffer 
  if(XT>0) // moving forward
  { 
  Serial.println("Moving Right for time:");
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
  Serial.println("Moving Left for time");
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
      Serial.println("Moving Y axis ");
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
      Serial.println("Moving z axis ");
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
      Serial.println("Moving down for time ");
      Serial.println(ZT*ZUnit/1000);
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
  
}
