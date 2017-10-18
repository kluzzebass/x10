
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
				leds[xy(x, y)] = colorSequence(sequencePosition + (10 * (x + y)), true);
			}
		}

		tick = now;
		sequencePosition++;
		FastLED.show();
	}
}