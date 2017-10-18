
#include <X10/Effect.h>

class X10_WibbleWobble : public X10_Effect
{
public:
	X10_WibbleWobble(CRGB *leds)
		: X10_Effect(leds) {}

	void begin();
	void loop();

protected:

	int wibbleX;
	int wibbleY;
	int wobbleX;
	int wobbleY;

	void drawOneFrame(byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8);
};