#include <Servo.h>
#include <RedBot.h>

const int TrigPin = A1;
const int EchoPin = A0;
const int buzzerPin = 11;
const int buttonPin = 12;
const int ledPin = 13;

RedBotMotors motors;
RedBotEncoder encoder = RedBotEncoder(A2, 10);
Servo neck;

int countsPerRev = 192;
float wheelDiam = 65;
float wheelCirc = PI * wheelDiam;
float dis;
int neckPos = 0;
int ultraPos[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};
int neckMoPos[] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165};


void setup()
{
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  neck.attach(9);

  Serial.println("Waiting");
  do
  {
    delay(10);
  } while ( digitalRead(buttonPin) == HIGH );

  Serial.println("Starting");
  tone(buzzerPin, 200);
  delay(500);
  noTone(buzzerPin);

  neck.write(0);

}


void reverse (int delayTime)
{
  motors.drive(-100);
  delay(delayTime);
  motors.brake();
}

float detectDistance()
{
  float duration, distance;
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  duration = pulseIn(EchoPin, HIGH);
  distance = (duration / 2) * 0.0344;

  if (distance > 400)
  {
    distance = 400;
  }
  if (distance < 3)
  {
    distance = 3;
  }
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

void loop()
{
  dis = detectDistance();

  neck.write(90);
  delay(700);

  while (dis >= 20)
  {
    driveStraight(100);
    dis = detectDistance();
    delay(10);
    tone(buzzerPin, 50);
    delay(10);
    noTone(buzzerPin);
  }
  if (dis < 20)
  {
    motors.drive(0);
    delay(1000);

    for (int i = 0; i >= 33; i++)
    {
      neck.write(neckMoPos[i]);
      delay(80);
      ultraPos[i] = detectDistance();
      delay(80);
    }


    delay(99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999);
  }

}


