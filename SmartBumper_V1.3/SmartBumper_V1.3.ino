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

  do
  {
    delay(10);
    Serial.print("Waiting");
    Serial.print("\n");
  }while ( digitalRead(buttonPin) == HIGH );
}


void loop()
{
  delay(200);
  motors.drive(-70);
  delay(70);
  motors.stop();
  delay(300);
}
