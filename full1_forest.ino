#include <stdlib.h>
#include <Wire.h>

#define m1 10
#define m2 6
#define m3 8
#define m1pwm 11
#define m2pwm 7
#define m3pwm 9
#define proxy_bridge 3

int forest = 1, bridge = 20, gerege = 20;
int a=1, error, f, integrate, pid, pwm1, pwm2, pwm3, d, i = 1, set_pwm = 100;
int dist[7] = {0, 146, 120, 165, 55 ,1000, 100000};
int c[7] = {0, 2, -20, 0 , -15,-15, 5};
float kp[7] = {0 , 5, 3, 4, 1.2, 1.5, 4};
char z;
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
{ 
  Serial.print(z);
  Serial.print("\t");
  Serial.print(i);
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
  z = zone();
  switch(z)
  {
    case 'f': 
    {
      distance();
      angle();
      rotation();
      calc();
      pwm();
    }
    break;
    case 'b':
    {
      bridge_forward();
      calc();
      pwm();
    }
    break ;
    case 'g':
    {
      stop_motors();
    }
    break;
    case 's':
    {
      
    }
    break;
    case 't':
    {
      
    }
    break;
    default:
    {
      stop_motors();
    }
  }
}

char zone()
{
  if (forest == 1)
  {
    return 'f';
  }
  else if ( bridge == 1)
  {
    return 'b';
  }
  else if( gerege == 1)
  {
    return 'g';
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
{
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

int rotation ()
{
  if ( d == dist[i] )
  {
    i++;
    if (( i % 2) == 0 )
      a = 2 ;
    else
      a = 1;
  }
  if(i == 5)
  {
    forest = 0;
    bridge = 1;
  }
}

void bridge_forward()
{
  if( digitalRead(proxy_bridge) == LOW)
  {
    bridge = 1;
  }
  else 
  {
    i++;
    bridge = 0;
    gerege = 1;
  }
}
void calc()
{
  error = f - c[i];
  pid = (kp[i] * error);
  if(forest == 1)
  {
    if (a == 2) // when using wheel 1 and 3
    {
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
      digitalWrite(m2,LOW);
      digitalWrite(m1,HIGH);
    }
  }
  else if (bridge == 1)
  {
   // Serial.println("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    pwm3 = set_pwm + pid; // should move anticlockwise or else 0
    pwm1 = set_pwm - pid;
    pwm2=0;
    digitalWrite(m3,LOW);
    digitalWrite(m1,HIGH); 
  }
  else 
  {
    pwm1 = 0 ;
    pwm2 = 0 ;
    pwm3 = 0 ;
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

void stop_motors()
{
  analogWrite(m1pwm, 0);
  analogWrite(m2pwm, 0);
  analogWrite(m3pwm, 0);
}

