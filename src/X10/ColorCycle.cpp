
#include <X10/ColorCycle.h>

void X10_ColorCycle::begin()
{
}

void X10_ColorCycle::loop()
{
	unsigned long now = millis();

	if (now >= (tick + 10))
	{
		for (int x = 0; x < WIDTH; x++)
		{
			for (int y = 0; y < HEIGHT; y++)
			{
#ifdef NEOPIXELBUS
				leds->SetPixelColor(xy(x, y), HsbColor((uint8_t)(sequencePosition + (10 * (x + y))) / 255.0, 1, 1));
#else
				leds[xy(x, y)] = CHSV((uint8_t)(sequencePosition + (10 * (x + y))), 255, 255);
#endif
			}
		}

		tick = now;
		sequencePosition++;
#ifdef NEOPIXELBUS
		leds->Show();
#else
		FastLED.show();
#endif
	}
}