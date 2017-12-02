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

int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
float wheelDiam = 65;  // diam = 65mm
float wheelCirc = PI * wheelDiam; // Redbot wheel circumference = pi*D
float dis;
int neckPos;


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


void loop()
{
  dis = detectDistance();
  
  neck.write(90);
  delay(700);
  
  while (dis > 20)
  {
    driveStraight(100);
    dis = detectDistance();
  }
  while (dis <=20)
  {
    dis = detectDistance();
    neck.write(neckPos);
    if (neckPos >= 165)
    {
      neckPos++ 
    }
    else
        
    
  }

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

