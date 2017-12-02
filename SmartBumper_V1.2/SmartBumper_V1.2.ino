#include <RedBot.h>
RedBotMotors motors;
RedBotBumper lBumper = RedBotBumper(3);  // initialzes bumper object on pin 3
RedBotBumper rBumper = RedBotBumper(11); // initialzes bumper object on pin 11

const int buttonPin = 12; // variable to store the button Pin
int lBumperState;  // state variable to store the bumper value
int rBumperState;  // state variable to store the bumper value
int xx = 1;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}


void loop()
{
  Serial.print(xx);
  Serial.print(", ");
  
  if ( digitalRead(buttonPin) == LOW ) // if the button is pushed
  {
    xx = xx + 1;
  }

  while (xx == 2);
  {
    
    motors.drive(100);
    delay(100);
    motors.brake();
    delay(500);
  }
 
 delay(100);
}
