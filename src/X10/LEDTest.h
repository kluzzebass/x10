
#include <X10/Effect.h>

class X10_LEDTest : public X10_Effect
{
public:
	X10_LEDTest(CRGB *leds, Stream &s)
		: X10_Effect(leds, s) {}

	void begin();
	void init() {};
	void loop();

protected:
	int step = 0;
	unsigned long tick = 0;
};