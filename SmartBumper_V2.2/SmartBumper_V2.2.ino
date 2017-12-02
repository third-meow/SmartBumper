#include <RedBot.h>
#define centerTrigPin A0
#define centerEchoPin A1
#define leftTrigPin A6
#define leftEchoPin A7
#define rightTrigPin 3
#define rightEchoPin 9

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

void setup()
{
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  pinMode(centerTrigPin, OUTPUT);
  pinMode(leftTrigPin, OUTPUT);
  pinMode(rightTrigPin, OUTPUT);
  pinMode(centerEchoPin, INPUT);
  pinMode(leftEchoPin, INPUT);
  pinMode(rightEchoPin, INPUT);

  //:::::::::::::::::::::::::::::::::::

  Serial.println("Waiting");
  do
  {
    delay(10);
  } while ( digitalRead(buttonPin) == HIGH );

  Serial.println("Starting");
  
}







void loop()
{
  Cdis = detectFrontDistance;
  Ldis = detectLeftDistance;
  Rdis = detectRightDistance;





  driveStraight (180);

  if ( Cdis < 7 || Ldis < 7 || Rdis < 7 );
  {
    motors.stop();

    if ( Cdis < 7 );
    {
      reverse (200);
      
      if ( ( Ldis < 7 ) && ( Rdis < 7 ) )
      {
        
      }
      
      
      
      
      
      if ( Ldis >= Rdis )
      {
        motors.leftDrive(-100);
        motors.rightDrive(100);
        delay(250);
        motors.stop();
      }
      else
      {
        motors.leftDrive(100);
        motors.rightDrive(-100);
        delay(250);
        motors.stop();
      }
    }

    if ( Ldis < 7 );
    {
      motors.leftDrive(100);
      motors.rightDrive(-100);
      delay(250);
      motors.stop();
    }

    if ( Rdis < 7 );
    {
      motors.rightDrive(100);
      motors.leftDrive(-100);
      delay(250);
      motors.stop();
    }
  }
}

void reverse (int delayTime)
{
  motors.drive(-100);
  delay(delayTime);
  motors.brake();
}

int detectFrontDistance()
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


int detectLeftDistance()
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

int detectRightDistance()
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

