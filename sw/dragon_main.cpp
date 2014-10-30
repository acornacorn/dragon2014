/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Arduino.h> 
//#include <Servo.h> 
#include <ac_alive.h> 
#include <ac_printf.h> 
#include <dragon_servos.h> 
#include <dragon_pins.h> 
 
#if 0
int pos = 0;    // variable to store the servo position 
#endif



void setup() 
{ 
  Serial.begin(38400);

  acAliveInit();
  dragonServoInit();

  Serial.println("Hello");
} 

#if 0
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

bool g_big_servo_motion = true;
#endif


#if 0
void doServos()
{
  uint32_t now = millis;

  static mode = 0;

  if (mode == 0)
  {

  }

  bool cmd = false;
  Serial.println("Up");
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    look_servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    if (checkKey() != -1)
    {
       cmd=true;
       break; 
    }
    yield();
  } 
  Serial.println("Dn");
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    look_servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    if (checkKey() != -1)
    {
       cmd=true;
       break; 
    }
    yield();
  }
  
  while (cmd)
  {
    int v = Serial.read();
    if (v >= 'a' && v <= 'z')
    {
       v -= 'a';
       if (g_big_servo_motion)
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
      g_big_servo_motion=true;
    if (v=='S')
      g_big_servo_motion=false; 
    if (v=='Q')
      cmd=false;

    yield();
  }
  
  // Range of HS300 servo: d=29...176=s  mid: 107=l
}
#endif

void loop() 
{  
#if 0
  digitalWrite(dpin_alive_led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300);               // wait for a second
  digitalWrite(dpin_alive_led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
#endif

  Serial.println("a");
  Serial.flush();
  delay(10);
  yield();

  acPrintf("helloz %d\n", millis());

  //printf("hello\n");

#if 1
  acAliveUpdate();
#endif



#if 0
  dragonServoUpdate();
#endif

#if 0
#if 0
  int x = 0;
  for(;;)
  {

    char buf[20];
    x++;
    snprintf(buf, sizeof(buf), "x=%x", x);
    Serial.println(buf);

    yield();
  }
#endif

#if 0
  doServos();
#else
  bool cmd = false;
  Serial.println("Up");
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    look_servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    if (checkKey() != -1)
    {
       cmd=true;
       break; 
    }
    yield();
  } 
  Serial.println("Dn");
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    look_servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    if (checkKey() != -1)
    {
       cmd=true;
       break; 
    }
    yield();
  }
  
  while (cmd)
  {
    int v = Serial.read();
    if (v >= 'a' && v <= 'z')
    {
       v -= 'a';
       if (g_big_servo_motion)
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
      g_big_servo_motion=true;
    if (v=='S')
      g_big_servo_motion=false; 
    if (v=='Q')
      cmd=false;

    yield();
  }
  
  // Range of HS300 servo: d=29...176=s  mid: 107=l
#endif
#endif
} 

extern "C" int main(void)
{
	setup();
	while (1) {
		loop();
		yield();
	}
}

