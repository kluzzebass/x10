
#include <X10/Effect.h>


class X10_ColorCycle : public X10_Effect
{
public:
	X10_ColorCycle(CRGB *leds)
		: X10_Effect(leds) {}

	void begin();
	void loop();

protected:
	unsigned long tick = 0;
	int sequencePosition = 0;
};