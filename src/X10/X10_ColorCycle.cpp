
#include <X10/X10_ColorCycle.h>

void X10_ColorCycle::begin()
{
}

void X10_ColorCycle::loop()
{
	unsigned long now = millis();

	if (now >= (tick + 10))
	{
		for (int x = 0; x < cfg.width; x++)
		{
			for (int y = 0; y < cfg.height; y++)
			{
				matrix->drawPixel(x, y, colorSequence(sequencePosition + (10 * (x + y)), true));
			}
		}

		tick = now;
		sequencePosition++;
		matrix->show();
	}
}