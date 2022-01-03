//y axis only


int Ypin1 = 5;
int Ypin2 = 6;

//int motor2pin1 = 4;
//int motor2pin2 = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(Ypin1, OUTPUT);
  pinMode(Ypin2, OUTPUT);
 // pinMode(motor2pin1, OUTPUT);
 // pinMode(motor2pin2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:   
  digitalWrite(Ypin1, HIGH);
  digitalWrite(Ypin2, LOW);

  //digitalWrite(motor2pin1, HIGH);
  //digitalWrite(motor2pin2, LOW);
  //delay(5000);

  //digitalWrite(Ypin1, LOW);
  //digitalWrite(Ypin2, HIGH);

  //digitalWrite(motor2pin1, LOW);
  //digitalWrite(motor2pin2, HIGH);
  //delay(5000);
}
