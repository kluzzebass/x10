
#include <X10/Base.h>

uint16_t X10_Base::xy(uint8_t x, uint8_t y)
{
	uint16_t i;

	y = HEIGHT - 1 - y;

	if (y & 0x01)
	{
		uint8_t revX = (WIDTH - 1) - x;
		i = (y * WIDTH) + revX;
	}
	else
	{
		i = (y * WIDTH) + x;
	}
  
	return i;
}

void X10_Base::fill(CRGB color)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = color;
	}
}

void X10_Base::clear()
{
	memset(leds, 0, sizeof(CRGB) * NUM_LEDS);
}