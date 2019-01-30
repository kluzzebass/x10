
#include <X10/Effect.h>

#define WIBBLE_OFFSET 20
#define WIBBLE_RANGE 30
#define WOBBLE_OFFSET 250
#define WOBBLE_RANGE 200

class X10_WibbleWobble : public X10_Effect
{
public:
	X10_WibbleWobble(
		NeoPixelBusType *leds,
		Stream &s)
		: X10_Effect(leds, s) {}

	void begin();
	void init();
	void loop();

	void changeRate(uint16_t r);
	uint16_t changeRate() { return mChangeRate; }
	void randomize();

	uint16_t wibbleX() { return mWibbleX; }
	uint16_t wibbleY() { return mWibbleY; }
	uint16_t wobbleX() { return mWobbleX; }
	uint16_t wobbleY() { return mWobbleY; }

	void wibbleX(uint16_t w);
	void wibbleY(uint16_t w);
	void wobbleX(uint16_t w);
	void wobbleY(uint16_t w);

protected:

	unsigned long mLastChange;
	unsigned long mChangeRate;

	int mWibbleX;
	int mWibbleY;
	int mWobbleX;
	int mWobbleY;

	int16_t cos16(uint16_t r);
	void drawOneFrame(byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8);
};