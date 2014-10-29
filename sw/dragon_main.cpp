/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Arduino.h> 
#include <Servo.h> 
 
Servo look_servo; 
Servo leye_servo; 
Servo reye_servo; 
Servo lip_servo; 

// create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
int pos = 0;    // variable to store the servo position 
int alive_led=13;

void setup() 
{ 
  Serial.begin(38400);
  pinMode(alive_led, OUTPUT);
  look_servo.attach(14);  // look is pin 14
  leye_servo.attach(15);  // look is pin 15
  reye_servo.attach(16);  // look is pin 16
  lip_servo.attach(17);  // look is pin 17
  Serial.println("Hello");
} 

int checkKey()
{
  int a = Serial.peek();
  if (a != -1)
  {
     a = Serial.read();
     return a;
  }  
  return -1;
}

bool big = true;
void loop() 
{  
  for(;;)
  {
    digitalWrite(alive_led, HIGH);
    delay(100);
    digitalWrite(alive_led, LOW);
    delay(100);
    yield();
  }

  bool cmd = false;
  Serial.println("Up");
  digitalWrite(alive_led, HIGH);
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    look_servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    if (checkKey() != -1)
    {
       cmd=true;
       break; 
    }
  } 
  Serial.println("Dn");
  digitalWrite(alive_led, LOW);
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    look_servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    if (checkKey() != -1)
    {
       cmd=true;
       break; 
    }
  }
  
  while (cmd)
  {
    int v = Serial.read();
    if (v >= 'a' && v <= 'z')
    {
       v -= 'a';
       if (big)
       {
         v *= 255;
         v /= 26;
       }
       else
       {
         v = 128 + v - 13;
       }
       char buf[100];
       sprintf(buf,"val=%d",v);
       Serial.println(buf);
       look_servo.write(v);
       pos=v;
    }
    if (v=='B')
      big=true;
    if (v=='S')
      big=false; 
    if (v=='Q')
      cmd=false;
  }
  
  // Range of HS300 servo: d=29...176=s  mid: 107=l
} 

extern "C" int main(void)
{
	setup();
	while (1) {
		loop();
		yield();
	}
}

