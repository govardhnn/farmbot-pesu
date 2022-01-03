int moisturePin = A0;

int moistureValue ;

void setup() {

   Serial.begin(9600);

   Serial.println("Reading From the Sensor ...");

   delay(2000);

   }

void loop() {

   moistureValue= analogRead(moisturePin);

   moistureValue = map(moistureValue,550,0,0,200);

   Serial.print("Mositure : ");

   Serial.print(moistureValue);

   Serial.println("%");

   delay(1000);

   }
