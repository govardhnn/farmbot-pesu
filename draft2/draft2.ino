//pins used ArdMega: 3, 4, 5, 6, 9, 10, 11, 14, 15
// defines GPIO pins numbers
const int YstepPin = 3, YdirPin = 4; 
const int ZstepPin = 1, ZdirPin = 2; 
int en=11;
int pumpMotorForward = 14;     
int pumpMotorbackward = 15;  
  //int enA=12;// Enable A
  //int enB=13;// Enable B
  int leftMotorForward = 6;   //IN3
  int rightMotorForward = 10; //IN1  
  int leftMotorBackward = 5;  //IN4 
  int rightMotorBackward = 9; //IN2
  
int XUnit= 1000; //assuming one second time taken to move by 1 grid length
int YUnit = 1000;
int ZUnit = 1000;
int pumpTime;
String ArdInput;
String s, prevLocation, newLocation;
double newX, newY, newZ, prevX, prevY, prevZ, moveXby, moveYby; //all coordinates are units of grid lengths
char mode;
char func;

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

  String ArdInput = "ID00X00Y00Z00Q00"; //ideal state in format <state><function>00X00Y00Z00
  //Idle / Move - Hydrate Weed Seed
  String prevLocation="X00Y00Z00Q00"; //hardcoded to Home
    

  
  if(Serial.available()==1)
  {
    
    //ArdInput = Serial.readStringUntil('\n');
    //ArdInput = ""
    ArdInput = "MV00X05Y04Z04Q23"; //command to (M)ove 
    //ArdInput = "WP05"//command to pump water
    Serial.print("Message Recieved: ");
    Serial.println(ArdInput);    
    char Mode = ArdInput.charAt(0);
    //char Operation = ArdInput.charAt(1);
    Serial.println(Mode); //use if to direct to functions M- move first, then P - Pump
    //Serial.println(Operation);
    
    if(Mode == 'M')//&& prevLocation != newLocation
    {    
      Serial.println("old location:");
      Serial.println(prevLocation);
      Serial.println("mode selected, new location:");
      newLocation = ArdInput.substring(4,16); // formatX00Y00Z00Q00
      Serial.println(newLocation);
      
      String newXS = newLocation.substring(1,3);      newX =newXS.toDouble();
      String prevXS = prevLocation.substring(1,3);      prevX =prevXS.toDouble();
      Serial.println("moved X by:");     
      Serial.println(newX-prevX);
      moveXby = newX - prevX;
      xaxis(moveXby);

      String newYS = newLocation.substring(4,6);      newY =newYS.toDouble();
      String prevYS = prevLocation.substring(4,6);      prevY =prevYS.toDouble();
      Serial.println("moved Y by:");     
      Serial.println(newY-prevY);
      moveYby = newY - prevY;
      yaxis(moveYby);
      //move from prevLocation to newLocation using functions
      //prevLocation = newLocation;

      Serial.println("arddone"); // notifies the RPi that arduino command is done
    }

    if(Mode == 'W')
    {
      String pumpS = ArdInput.substring(2,4);      
      int pumpT = pumpS.toDouble();
      pump(pumpT); 
    }
  }
  // put your main code here, to run repeatedly:
   
  }



void xaxis(int XT)
{
  delay(200);//200ms buffer remove later
  if(XT>0)
  { 
  //delay();
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
  Serial.println("Moving Backward");
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
 // delay(50000);
  //yaxis(); 
  //delay(2000);
 // pump();
  
 
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
      digitalWrite(ZdirPin,LOW); //Clockwise Direction
      Serial.println("Moving Y axis ");
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
      digitalWrite(ZdirPin,HIGH); //Anti-Clockwise Direction
       for(int z = 0; z <ZT; z++) 
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
