
#include <X10/LEDTest.h>

void X10_LEDTest::begin()
{
}

void X10_LEDTest::loop()
{
	clear();
#ifdef NEOPIXELBUS
	leds->SetPixelColor(xy(0, 0), RgbColor(255, 0, 0));
	leds->SetPixelColor(xy(15, 0), RgbColor(0, 255, 0));
	leds->SetPixelColor(xy(0, 15), RgbColor(0, 0, 255));
	leds->SetPixelColor(xy(15, 15), RgbColor(255, 255, 0));
	leds->Show();
#else
	leds[xy(0, 0)] = CRGB(255, 0, 0);
	leds[xy(15, 0)] = CRGB(0, 255, 0);
	leds[xy(0, 15)] = CRGB(0, 0, 255);
	leds[xy(15, 15)] = CRGB(255, 255, 0);
	FastLED.show();
#endif
}