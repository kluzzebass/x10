
#include <X10/Clock.h>

void X10_Clock::begin()
{
}

void X10_Clock::loop()
{
	unsigned long now = millis();

	if (now >= (tick + 10))
	{
		// clear();
		fill(CRGB(255, 255, 0));

		uint seconds = now / 1000;
		uint minutesLo = seconds / 60 % 10; 
		uint minutesHi = seconds / 600 % 10; 
		uint hoursLo = seconds / 60 / 60 % 10;
		uint hoursHi = seconds / 60 / 600 % 10;

		drawDigit(hoursHi, 0, 3);
		drawDigit(hoursLo, 4, 3);
		drawDigit(minutesHi, 9, 3);
		drawDigit(minutesLo, 13, 3);

		drawSeconds(seconds % 60);

		tick = now;
		sequencePosition++;
		FastLED.show();
	}
}

bool X10_Clock::hiLite(uint digit, uint x, uint y)
{
	if (digit >= X10_CLOCK_DIGITS || x >= X10_CLOCK_FONT_WIDTH || y >= X10_CLOCK_FONT_HEIGHT) return false;
	uint8_t row = font[digit][y];
	return row & (X10_CLOCK_PIXEL_MASK >> x);
}

void X10_Clock::drawDigit(uint digit, uint xPos, uint yPos)
{
	for (int x = 0; x < X10_CLOCK_FONT_WIDTH; x++)
	{
		for (int y = 0; y < X10_CLOCK_FONT_HEIGHT; y++)
		{
			if (hiLite(digit, x, y))
			{
				leds[xy(xPos + x, yPos + y)] = CRGB(0, 0, 255);
			}
		}
	}
}

void X10_Clock::drawSeconds(uint seconds)
{
	uint x1 = secondHand[seconds][0] >> 4;
	uint y1 = secondHand[seconds][0] & 0xf;

	uint x2 = secondHand[seconds][1] >> 4;
	uint y2 = secondHand[seconds][1] & 0xf;

	leds[xy(x1, y1)] = CRGB(255, 0, 0);
	leds[xy(x2, y2)] = CRGB(255, 0, 0);
}
