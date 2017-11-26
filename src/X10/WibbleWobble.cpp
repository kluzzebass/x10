
#include <X10/WibbleWobble.h>

void X10_WibbleWobble::begin()
{
	mLastChange = millis();
	init();
}

void X10_WibbleWobble::init()
{
	randomize();
}

void X10_WibbleWobble::randomize()
{
	mWibbleX = random(WIBBLE_RANGE + 1);
	do
		mWibbleY = random(WIBBLE_RANGE + 1);
	while (mWibbleX == mWibbleY);

	mWobbleX = random(WOBBLE_RANGE + 1);
	do
		mWobbleY = random(WOBBLE_RANGE + 1);
	while (mWobbleX == mWobbleY);
}

void X10_WibbleWobble::loop()
{
	uint32_t ms = millis();
	int32_t yHueDelta32 = ((int32_t)cos16(ms * (mWibbleY + WIBBLE_OFFSET)) * ((mWobbleY + WOBBLE_OFFSET) / WIDTH));
	int32_t xHueDelta32 = ((int32_t)cos16(ms * (mWibbleX + WIBBLE_OFFSET)) * ((mWobbleX + WOBBLE_OFFSET) / HEIGHT));
	drawOneFrame(ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
	FastLED.show();

	unsigned long now = millis();
	if (mChangeRate && (now >= (mLastChange + mChangeRate * 1000)))
	{
		randomize();
		mLastChange = now;
	}
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
