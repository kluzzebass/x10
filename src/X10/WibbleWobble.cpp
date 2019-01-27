
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

void X10_WibbleWobble::changeRate(uint16_t r)
{
	if (mChangeRate == r) return;
	mChangeRate = r;
	s.print("changeRate = ");
	s.println(mChangeRate);
}

void X10_WibbleWobble::wibbleX(uint16_t w)
{
	if (mWibbleX == w) return;
	mWibbleX = w;
	if (mWibbleX < 0) mWibbleX = 0;
	if (mWibbleX > WIBBLE_RANGE) mWibbleX = WIBBLE_RANGE;
	s.print("wibbleX = ");
	s.println(mWibbleX);
}

void X10_WibbleWobble::wibbleY(uint16_t w)
{
	if (mWibbleY == w) return;
	mWibbleY = w;
	if (mWibbleY < 0) mWibbleY = 0;
	if (mWibbleY > WIBBLE_RANGE) mWibbleY = WIBBLE_RANGE;
	s.print("wibbleY = ");
	s.println(mWibbleY);
}

void X10_WibbleWobble::wobbleX(uint16_t w)
{
	if (mWobbleX == w) return;
	mWobbleX = w;
	if (mWobbleX < 0) mWobbleX = 0;
	if (mWobbleX > WIBBLE_RANGE) mWobbleX = WIBBLE_RANGE;
	s.print("wobbleX = ");
	s.println(mWobbleX);
}

void X10_WibbleWobble::wobbleY(uint16_t w)
{
	if (mWobbleY == w) return;
	mWobbleY = w;
	if (mWobbleY < 0) mWobbleY = 0;
	if (mWobbleY > WIBBLE_RANGE) mWobbleY = WIBBLE_RANGE;
	s.print("wobbleY = ");
	s.println(mWobbleY);
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
	s.println("Randomizing WibbleWobble.");
}


int16_t X10_WibbleWobble::cos16(uint16_t r) {
	float rad = (2 * PI) / 65536 * (float)r;
	return (int16_t)(cos(rad) * 32768);
}

void X10_WibbleWobble::loop()
{
	uint32_t ms = millis();
	int32_t yHueDelta32 = ((int32_t)cos16(ms * (mWibbleY + WIBBLE_OFFSET)) * ((mWobbleY + WOBBLE_OFFSET) / WIDTH));
	int32_t xHueDelta32 = ((int32_t)cos16(ms * (mWibbleX + WIBBLE_OFFSET)) * ((mWobbleX + WOBBLE_OFFSET) / HEIGHT));
	drawOneFrame(ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
#ifdef NEOPIXELBUS
	leds->Show();
#else
	FastLED.show();
#endif

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
#ifdef NEOPIXELBUS
			leds->SetPixelColor(xy(x, y), HsbColor((float)pixelHue / 255.0, 1, 1));
#else
			leds[xy(x, y)] = CHSV(pixelHue, 255, 255);
#endif
		}
	}
}
