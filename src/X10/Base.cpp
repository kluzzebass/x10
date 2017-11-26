
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

void X10_Base::drawBitmap(BMPReader &bmp, uint16_t x, uint16_t y, uint16_t dx, uint16_t dy, uint16_t bmpX, uint16_t bmpY)
{
	uint32_t p[WIDTH];
	uint16_t r;

	// No point drawing outside the frame
	if (x >= WIDTH || y >= HEIGHT)
		return;

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

void X10_Base::drawFrame(BMPReader &bmp, int16_t x, int16_t y)
{
	uint32_t p[WIDTH];
	uint16_t r;

	int16_t offsetX, offsetY, dX, dY, remX, remY;

	// Make sure we have an open bitmap
	if (!bmp.isOpen()) {
		s.println("drawBitmap(): Bitmap file is not open!");
		return;
	}

	// Get rid of cases that requires no drawing
	if (x >= WIDTH) return;
	if (y >= HEIGHT) return;
	if ((x + bmp.width()) < 0) return;
	if ((y + bmp.height()) < 0) return;

	if (x < 0) {
		offsetX = -x;
		x = 0;
	} else {
		offsetX = 0;
	}

	if (y < 0) {
		offsetY = -y;
		y = 0;
	} else {
		offsetY = 0;
	}

	dX = WIDTH - x;
	dY = HEIGHT - y;
	remX = bmp.width() - offsetX;
	remY = bmp.height() - offsetY;

	if (dX > remX) dX = remX;
	if (dY > remY) dY = remY;


	// Fetch all the pixels
	for (uint8_t i = 0; i < dY; i++)
	{
		r = bmp.getPixels(p, offsetX, offsetY + i, dX);

		for (uint8_t j = 0; j < r; j++)
		{
			leds[xy(x + j, y + i)] = CRGB(p[j]);
		}
	}



}