//pins used ArdMega: 3, 4, 5, 6, 9, 10, 11, 14, 15


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

  String ArdInput = "II00X00Y00Z00Q00"; //ideal state in format <state><function>00X00Y00Z00
  //Idle / Move - Hydrate Weed Seed
  String prevLocation="X00Y00Z00Q00"; //hardcoded to Home
  if(Serial.available()==1)
  {
    //ArdInput = Serial.readString();
    ArdInput = "MH00X23Y12Z213Q23"; //harcoded to (M)ove and (H)ydrate water with a random location
    Serial.print("Message Recieved: ");
    Serial.println(ArdInput);    
    char Mode = ArdInput.charAt(0);
    char Operation = ArdInput.charAt(1);
    Serial.println(Mode); //use if to direct to functions M- move first, then P - Pump
    Serial.println(Operation);
    
    if(Mode == 'M')//&& prevLocation != newLocation
    {
      Serial.println("old location:");
      Serial.println(prevLocation);
      Serial.println("mode selected, new location:");
      newLocation = ArdInput.substring(4,16);
      Serial.println(newLocation);

       String newXS = newLocation.substring(1,3);      newX =newXS.toDouble();
      String prevXS = prevLocation.substring(1,3);      prevX =prevXS.toDouble();
      Serial.println("moving X by:");     
      Serial.println(newX-prevX);
      moveXby = newX - prevX;
      xaxis(moveXby);
      //move from prevLocation to newLocation using functions
      //prevLocation = newLocation;
     
    }
    
  }
  // put your main code here, to run repeatedly:
   
  }



void xaxis(int a)
{
  delay(5000);
  Serial.println("Moving Forward");
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward,LOW);
  //analogWrite(enA, 235);
  
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,HIGH); 
  //analogWrite(enB, 225);

  delay(4000);

  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW);

  delay(1000);

  Serial.println("MOving Backward");
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,HIGH);
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(leftMotorBackward,LOW);

  delay(2000);

  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW);

 // delay(50000);
  //yaxis(); 
  delay(2000);
  //pump();
  
  
  Serial.println("Moving Forward");
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,HIGH); 

  delay(2000);

  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW);

  delay(1000);

  Serial.println("MOving Backward");
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,HIGH);
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(leftMotorBackward,LOW);

  delay(4000);

  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW);

}//end of x-axis function()

void pump()
{
  digitalWrite(en,HIGH);
  digitalWrite(pumpMotorForward,HIGH);
  digitalWrite(pumpMotorbackward,LOW);
  delay(10000);
  digitalWrite(pumpMotorForward,LOW);
  digitalWrite(pumpMotorbackward,LOW);
  delay(2000);
  digitalWrite(en,LOW);
}//end of pump() function

/*void yaxis()
{
   digitalWrite(dirPin,LOW); //Clockwise Direction
  for(int x = 0; x <10000; x++) //Rotating for 20 seconds
  { 
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  delay(2000);

  digitalWrite(dirPin,HIGH); //Anti-Clockwise Direction
  for(int x = 0; x <10000; x++) 
  {
    digitalWrite(YstepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(YstepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(2000);
}//end of y-axis*/
