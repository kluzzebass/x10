
#include <X10/X10_Effect.h>


class X10_LEDTest : public X10_Effect
{
public:
	X10_LEDTest(X10_Config &cfg, Adafruit_NeoMatrix *matrix, SdFat *sd)
		: X10_Effect(cfg, matrix, sd) {}

	void begin();
	void loop();

protected:
	int step = 0;
	unsigned long tick = 0;
};