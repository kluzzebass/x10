
#include <X10/LEDTest.h>

void X10_LEDTest::begin()
{
}

void X10_LEDTest::loop()
{
	clear();
	leds[xy(0,0)] = CRGB(255, 0, 0);
	leds[xy(15,0)] = CRGB(0, 255, 0);
	leds[xy(0,15)] = CRGB(0, 0, 255);
	leds[xy(15,15)] = CRGB(255, 255, 0);
	FastLED.show();
}