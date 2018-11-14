#include <Wire.h>
#include <stdlib.h>
#define aPulse1 2
#define aPulse2 3

volatile double pulse1, count = 0, pulse2, count2;
long interval;
int  flag = 1, flag1 = 0,i=0,a,d;
long int distance1 = 0, distance2 = 0;
unsigned long t;
float r = 4.3;
int dist[5] = {0 , 146, 120, 165, 60};

void setup() {
  Wire.begin(8);
  Serial.begin(9600);// join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  pinMode(aPulse1, INPUT_PULLUP);
  pinMode(aPulse2, INPUT_PULLUP);
}

void encoderA()
{
  pulse1++;
}
void encoderB()
{
  pulse2++;
}

void d1()
{
  attachInterrupt(digitalPinToInterrupt(aPulse1), encoderA, RISING);
  count = pulse1;
  distance1 = 2 * 3.14 * r * count / 600;
  d = distance1;
}
void d2()
{
  attachInterrupt(digitalPinToInterrupt(aPulse2), encoderB, RISING);
  count2 = pulse2;
  distance2 = 2 * 3.14 * r * count2 / 600;
  d = distance2;
}

void loop() {
  Serial.print(distance1);
  Serial.print("\t");
  Serial.println(distance2);
  if ((d > dist[i]) || (d == dist[i]))
  {
    i++;
    if (( i % 2) == 0 )
      a = 2 ;
    else
      a = 1;
  }
  if (a == 1)
  {
    distance2 = 0;
    pulse2 = 0;
    flag = 1;
    detachInterrupt(digitalPinToInterrupt(aPulse2));
    d1();
  }
  {
    if (a == 2)
    {
      pulse1 = 0;
      distance1 = 0;
      detachInterrupt(digitalPinToInterrupt(aPulse1));
      d2();
    }
  }

}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  if (a == 1)
  {
    Wire.write(distance1);
  }
  if (a ==2)
  {
    Wire.write(distance2);
  }
}

