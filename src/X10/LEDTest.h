
#include <X10/Effect.h>

class X10_LEDTest : public X10_Effect
{
public:
	X10_LEDTest(CRGB *leds)
		: X10_Effect(leds) {}

	void begin();
	void loop();

protected:
	int step = 0;
	unsigned long tick = 0;
};