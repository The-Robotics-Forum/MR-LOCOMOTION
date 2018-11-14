#include <stdlib.h>
#include <Wire.h>

#define m1 10
#define m2 6
#define m3 8
#define m1pwm 11
#define m2pwm 7
#define m3pwm 9

int a, error, f, integrate, pid, pwm1, pwm2, pwm3, d, i = 0, set_pwm = 100;
int dist[5] = {0, 146, 120, 165, 60};
int c[5] = {0, 2, -18, 2 , -15};
float kp[5] = {0 , 5, 3, 4, 4 };
int flag = 0;
void setup()
{
  delay(5000);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m1pwm, OUTPUT);
  pinMode(m2pwm, OUTPUT);
  pinMode(m3pwm, OUTPUT);
  Wire.begin();
  Serial.begin (2000000);
  Serial3.begin(115200);
}
void loop()
{ Serial.print(i);
  Serial.print("\t");
  Serial.print(a);
  Serial.print("\t");
  Serial.print(d);
  Serial.print("\t");
  Serial.print(f);
  Serial.print("\t");
  Serial.print(dist[i]);
  Serial.print("\t");
  Serial.print(c[i]);
  Serial.print("\t");
  // Serial.print(flag);
  //Serial.print("\t");
  Serial.print(pwm1);
  Serial.print("\t");
  Serial.print(pwm2);
  Serial.print("\t");
  Serial.println(pwm3);
  distance();
  angle();
  rotation();
  calc();
  pwm();
}

int rotation ()
{
  if ( d == dist[i] )
  {
    //Serial.println("aaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    i++;
    if (( i % 2) == 0 )
      a = 2 ;
    else
      a = 1;
  }
}
void distance()
{
  Wire.requestFrom(8, 1);
  while (Wire.available())
  {
    d = Wire.read();
  }
}

void angle()
{ //Serial.print("In sending\t");
  if (Serial3.available())
  {
    int b = Serial3.read();
    f = map(b, 0, 255, 0, 359);
    if (f > 180)
    {
      f = f - 360;
    }
  }
}
void calc()
{
  error = f - c[i];
  pid = (kp[i] * error);
  if (a == 2) // when using wheel 1 and 3
  {
    /*if ( flag == 0 )
      {
      int pid1 = (4 * error);
      pwm1 = 150 - pid1;
      pwm3 = 150 + pid1;
      pwm2 = 0;
      if (((error < 2 ) || (error == 2)) && ((error > -2) || (error == -2)))
      {
        flag = 1;
      }
      }*/
    pwm2 = set_pwm - pid; // should move anticlockwise or else 0
    pwm3 = set_pwm + pid; // should move clockwise or else 0
    pwm1 = pid;
    if ( error > 0 )
    {
      digitalWrite(m1, LOW);
    }
    else if ( error < 0)
    {
      digitalWrite(m1, HIGH);
    }
    else if (error == 0)
    {
      pwm1 = 0;
    }
    digitalWrite(m2, HIGH);
    digitalWrite(m3, LOW);
  }
  else if (a == 1) // when using wheel 1 and 2
  {
    pwm2 = set_pwm + pid;
    pwm1 = set_pwm - pid;
    pwm3 = 0;
    digitalWrite(m2, LOW);
    digitalWrite(m1 , HIGH);
  }
  if (i > 4)
  {
    pwm1 = pwm2 = pwm3 = 0;
  }
}

void pwm()
{
  if (pwm1 > 254)
  {
    pwm1 = 254;
  }
  else if (pwm1 < 0 )
  {
    pwm1 = 0 ;
  }
  if (pwm2 > 254)
  {
    pwm2 = 254;
  }
  else if (pwm2 < 0)
  {
    pwm2 = 0;
  }
  if (pwm3 > 254)
  {
    pwm3 = 254;
  }
  else if (pwm3 < 0)
  {
    pwm3 = 0;
  }
  analogWrite(m1pwm, pwm1);
  analogWrite(m2pwm, pwm2);
  analogWrite(m3pwm, pwm3);
}
