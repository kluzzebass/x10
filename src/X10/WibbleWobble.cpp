
#include <X10/WibbleWobble.h>

void X10_WibbleWobble::begin()
{
	wibbleX = 39;
	wibbleY = 27;
	wobbleX = 310;
	wobbleY = 350;
}

void X10_WibbleWobble::loop()
{
	uint32_t ms = millis();
	int32_t yHueDelta32 = ((int32_t)cos16(ms * wibbleY) * (wobbleY / WIDTH));
	int32_t xHueDelta32 = ((int32_t)cos16(ms * wibbleX) * (wobbleX / HEIGHT));
	drawOneFrame(ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
	FastLED.show();
}

void X10_WibbleWobble::drawOneFrame(byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
	byte lineStartHue = startHue8;
	for (byte y = 0; y < HEIGHT; y++)
	{
		lineStartHue += yHueDelta8;
		byte pixelHue = lineStartHue;      
		for (byte x = 0; x < WIDTH; x++)
		{
			pixelHue += xHueDelta8;
			leds[xy(x, y)] = CHSV(pixelHue, 255, 255);
		}
	}
}
