#include <Arduino.h> 
#include <ac_alive.h> 
#include <ac_printf.h> 
#include <dragon_servos.h> 
#include <dragon_buttons.h> 
#include <dragon_leds.h> 
#include <dragon_pins.h> 
#include <dragon.h> 
 

Dragon g_dragon;


void setup() 
{ 
  Serial.begin(38400);

  acAliveInit();

  delay(1000);
  acPrintf("BOOT\n");

  //dragonServoInit();
  dragonButtonInit();
  dragonLedInit();

  g_dragon.init();

  //g_dragon.setMode(Dragon::MODE_TEST);
  g_dragon.setMode(Dragon::MODE_KEY_R);

  //Serial.println("HelloA");
  //acPrintf("HelloB");
} 

void loop() 
{  
  acAliveUpdate();
  //dragonServoUpdate();
  dragonButtonUpdate();
  dragonLedUpdate();
  g_dragon.update();
} 

extern "C" int main(void)
{
	setup();
	while (1) {
		loop();
		yield();
	}
}

