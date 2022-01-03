

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Message Recieved");
  lcd.setCursor(0,1);
  lcd.print("PESU");
  lcd.setCursor(-4,2);
  lcd.print("Moving X by");
  lcd.setCursor(-4,3);
  lcd.print("HI");
}


void loop()
{
}
