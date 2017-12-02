#include <RedBot.h>
#define centerTrigPin A0
#define centerEchoPin A1
#define leftTrigPin A4
#define leftEchoPin A5
#define rightTrigPin 3
#define rightEchoPin 9
#define backTrigPin A6
#define backEchoPin A7

#define buzzerPin 11
#define buttonPin 12
#define ledPin 13

RedBotMotors motors;
RedBotEncoder encoder = RedBotEncoder(A2, 10);


int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
float wheelDiam = 65;  // diam = 65mm
float wheelCirc = PI * wheelDiam; // Redbot wheel circumference = pi*D

float Cdis;
float Rdis;
float Ldis;
float Bdis;

void setup()
{
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  pinMode(backTrigPin, OUTPUT);
  pinMode(centerTrigPin, OUTPUT);
  pinMode(leftTrigPin, OUTPUT);
  pinMode(rightTrigPin, OUTPUT);
  pinMode(centerEchoPin, INPUT);
  pinMode(leftEchoPin, INPUT);
  pinMode(rightEchoPin, INPUT);
  pinMode(backEchoPin, INPUT);
 

  //:::::::::::::::::::::::::::::::::::

  Serial.println("Waiting");
  do
  {
    delay(10);
  } while ( digitalRead(buttonPin) == HIGH );

  Serial.println("Starting");
  tone(buzzerPin, 200);
  delay(500);
  noTone(buzzerPin);


}


void loop()
{
  Cdis = detectFrontDistance();
  Ldis = detectLeftDistance();
  Rdis = detectRightDistance();
  Bdis = detectBackDistance();

  if ( Cdis < 3 )
  {
    Cdis = 3;
  }

  if ( Cdis > 390 )
  {
    Cdis = 390;
  }
  if ( Ldis < 3 )
  {
    Ldis = 3;
  }

  if ( Ldis > 390 )
  {
    Ldis = 390;
  }
    if ( Rdis < 3 )
  {
    Rdis = 3;
  }

  if ( Rdis > 390 )
  {
    Rdis = 390;
  }
    if ( Bdis < 3 )
  {
    Bdis = 3;
  }

  if ( Bdis > 390 )
  {
    Bdis = 390;
  }
  //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  
  if ( Cdis >= 10 && Rdis >= 10 && Ldis >= 10 )
  {
    driveStraight (180);
  }
  else
  {
    motors.stop();

    if ( Ldis < 10 )
    {
      reverse(300);
      motors.leftDrive(100);
      motors.rightDrive(-100);
      delay(100);
      motors.drive(100);
      delay(100);      
    }

    if ( Rdis < 10 )
    {
      reverse(300);
      motors.leftDrive(-100);
      motors.rightDrive(100);
      delay(100);
      motors.drive(100);
      delay(100);
    }
  }
}

void reverse (int delayTime)
{
  motors.drive(-100);
  delay(delayTime);
  motors.brake();
}

float detectFrontDistance()
{
  float duration, distance;
  digitalWrite(centerTrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(centerTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(centerTrigPin, LOW);

  duration = pulseIn(centerEchoPin, HIGH);
  distance = (duration / 2) * 0.0344;
  return distance;
  delay(500);
}


float detectLeftDistance()
{
  float duration, distance;
  digitalWrite(leftTrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(leftTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(leftTrigPin, LOW);

  duration = pulseIn(leftEchoPin, HIGH);
  distance = (duration / 2) * 0.0344;
  return distance;
  delay(500);
}

float detectRightDistance()
{
  float duration, distance;
  digitalWrite(rightTrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(rightTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(rightTrigPin, LOW);

  duration = pulseIn(rightEchoPin, HIGH);
  distance = (duration / 2) * 0.0344;
  return distance;
  delay(500);
}


float detectBackDistance()
{
  float duration, distance;
  digitalWrite(backTrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(backTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(backTrigPin, LOW);

  duration = pulseIn(backEchoPin, HIGH);
  distance = (duration / 2) * 0.0344;
  return distance;
  delay(500);
}



void driveStraight(int motorPower)
{
  long lCount = 0;
  long rCount = 0;
  float numRev;

  long prevlCount, prevrCount;
  long lDiff, rDiff;

  int leftPower = motorPower;
  int rightPower = motorPower;

  int offset = 5;
  //:::::::::::::::::::::::::::::::

  encoder.clearEnc(BOTH);
  delay(100);
  motors.drive(motorPower);

  lCount = encoder.getTicks(LEFT);
  rCount = encoder.getTicks(RIGHT);

  motors.leftDrive(leftPower);
  motors.rightDrive(rightPower);

  lDiff = (lCount - prevlCount);
  rDiff = (rCount - prevrCount);

  prevlCount = lCount;
  prevrCount = rCount;

  //:::::::::::::::::::::::::::::::
  if (lDiff > rDiff)
  {
    leftPower = leftPower - offset;
    rightPower = rightPower + offset;
  }

  else if (lDiff < rDiff)
  {
    leftPower = leftPower + offset;
    rightPower = rightPower - offset;
  }
  delay(50);
}

