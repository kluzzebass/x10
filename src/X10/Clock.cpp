
#include <X10/Clock.h>

void X10_Clock::begin()
{
	
}

void X10_Clock::loop()
{
	unsigned long now = millis();

	if (now >= (fxTick + 10))
	{
		// fill(CRGB(255, 255, 0));
		clear();

		drawDigit(hoursHi, 0, 3);
		drawDigit(hoursLo, 4, 3);
		drawDigit(minutesHi, 9, 3);
		drawDigit(minutesLo, 13, 3);
		drawSeconds(seconds);
	
		FastLED.show();
	
		sequencePosition++;
		fxTick = now;
	}

	// Grab the time from the RTC twice every second
	// Each RTC call takes just over 1ms.
	if (now >= (clTick + 500))
	{
		RtcDateTime dt = rtc.GetDateTime();

		seconds = dt.Second();
		minutesLo = dt.Minute() % 10; 
		minutesHi = dt.Minute() / 10;
		hoursLo = dt.Hour() % 10;
		hoursHi = dt.Hour() / 10;
		
		clTick = now;
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
