
#include <X10/Effect.h>

class X10_ColorCycle : public X10_Effect
{
public:
	X10_ColorCycle(
		NeoPixelBusType *leds,
		Stream &s)
		: X10_Effect(leds, s) {}

	void begin();
	void init() {};
	void loop();

protected:
	unsigned long tick = 0;
	uint8_t sequencePosition = 0;
};