#include <Arduino.h> 
#include <ac_alive.h> 
 
void setup() 
{ 
  Serial.begin(38400);
  acAliveInit();
} 

void loop() 
{  
  acAliveUpdate();
} 

extern "C" int main(void)
{
	setup();
	while (1) {
		loop();
		yield();
	}
}

