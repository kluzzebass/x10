
#include <X10/Effect.h>

#define WIBBLE_OFFSET 20
#define WIBBLE_RANGE 30
#define WOBBLE_OFFSET 250
#define WOBBLE_RANGE 200

class X10_WibbleWobble : public X10_Effect
{
public:
	X10_WibbleWobble(CRGB *leds, Stream &s)
		: X10_Effect(leds, s) {}

	void begin();
	void init();
	void loop();

	void changeRate(uint16_t r) { mChangeRate = r; }
	uint16_t changeRate() { return mChangeRate; }
	void randomize();

	uint16_t wibbleX() { return mWibbleX; }
	uint16_t wibbleY() { return mWibbleY; }
	uint16_t wobbleX() { return mWobbleX; }
	uint16_t wobbleY() { return mWobbleY; }

	void wibbleX(uint16_t w) { mWibbleX = w; }
	void wibbleY(uint16_t w) { mWibbleY = w; }
	void wobbleX(uint16_t w) { mWobbleX = w; }
	void wobbleY(uint16_t w) { mWobbleY = w; }

protected:

	unsigned long mLastChange;
	unsigned long mChangeRate;

	int mWibbleX;
	int mWibbleY;
	int mWobbleX;
	int mWobbleY;

	void drawOneFrame(byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8);
};