
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

void X10_Base::drawBitmap(BMPReader &bmp, uint8_t x, uint8_t y, uint16_t dx, uint16_t dy, uint16_t bmpX, uint16_t bmpY)
{
	uint32_t p[WIDTH];
	uint16_t r;

	// No point drawing outside the frame
	if (x >= WIDTH || y >= HEIGHT) return;

	// Crop deltas to fit the frame
	if ((x + dx) > WIDTH) dx = WIDTH - x;
	if ((y + dy) > WIDTH) dy = HEIGHT - y;

	// Make sure we have an open bitmap
	if (!bmp.isOpen()) {
		s.println("drawBitmap(): Bitmap file is not open!");
		return;
	}

	// Fetch all the pixels
	for (uint8_t i = 0; i < dy; i++)
	{
		r = bmp.getPixels(p, bmpX, bmpY + i, dx);

		for (uint8_t j = 0; j < r; j++)
		{
			leds[xy(x + j, y + i)] = CRGB(p[j]);
		}
	}
}
