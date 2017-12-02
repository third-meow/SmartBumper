#include <RedBot.h>
#define trigPin A0
#define echoPin A1
RedBotMotors motors;
RedBotEncoder encoder = RedBotEncoder(A2, 10);

const int buttonPin = 12; // variable to store the button Pin

int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
float wheelDiam = 65;  // diam = 65mm
float wheelCirc = PI * wheelDiam; // Redbot wheel circumference = pi*D


void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  do
  {
    delay(10);
    Serial.print("Waiting");
    Serial.print("\n");
  } while ( digitalRead(buttonPin) == HIGH );

  Serial.print("Starting");
  Serial.print("\n");
}


void loop()
{
  //driveStraight(20, 180);
  detectObjectDistance();
  
  //if ((lBumperState == LOW) || (rBumperState == LOW))
  //{
   // motors.brake();
   // motors.drive(-100);
   // delay(600);
   // motors.brake();
   // motors.rightMotor(-100); 
   // motors.leftMotor(-100);  
   // delay(500);
   // motors.brake();
  //}
}

void detectObjectDistance()
{
  float duration, objectDistance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  objectDistance = (duration / 2) * 0.0344;
  
  if (objectDistance >= 400 || objectDistance <= 2){
    Serial.print("Distance = ");
    Serial.print(objectDistance);
    Serial.println(" cm");
    Serial.println(" (Out of range)");
  }
  else {
    Serial.print("Distance = ");
    Serial.print(objectDistance);
    Serial.println(" cm");
    delay(500);
  }
  delay(500);
}






void driveStraight(float distance, int motorPower)
{
  long lCount = 0;
  long rCount = 0;
  long targetCount;
  float numRev;

  // variables for tracking the left and right encoder counts
  long prevlCount, prevrCount;

  long lDiff, rDiff;  // diff between current encoder count and previous count

  // variables for setting left and right motor power
  int leftPower = motorPower;
  int rightPower = motorPower;

  // variable used to offset motor power on right vs left to keep straight.
  int offset = 5;  // offset amount to compensate Right vs. Left drive

  numRev = distance / wheelCirc;  // calculate the target # of rotations
  targetCount = numRev * countsPerRev;    // calculate the target count


  encoder.clearEnc(BOTH);    // clear the encoder count
  delay(100);  // short delay before starting the motors.

  motors.drive(motorPower);  // start motors

  while (rCount < targetCount)
  {
  
    // while the right encoder is less than the target count -- debug print
    // the encoder values and wait -- this is a holding loop.
    lCount = encoder.getTicks(LEFT);
    rCount = encoder.getTicks(RIGHT);

    motors.leftDrive(leftPower);
    motors.rightDrive(rightPower);

    // calculate the rotation "speed" as a difference in the count from previous cycle.
    lDiff = (lCount - prevlCount);
    rDiff = (rCount - prevrCount);

    // store the current count as the "previous" count for the next cycle.
    prevlCount = lCount;
    prevrCount = rCount;

    // if left is faster than the right, slow down the left / speed up right
    if (lDiff > rDiff)
    {
      leftPower = leftPower - offset;
      rightPower = rightPower + offset;
    }
    // if right is faster than the left, speed up the left / slow down right
    else if (lDiff < rDiff)
    {
      leftPower = leftPower + offset;
      rightPower = rightPower - offset;
    }
    delay(50);  // short delay to give motors a chance to respond.
  }
  // now apply "brakes" to stop the motors.
  motors.stop();
}
