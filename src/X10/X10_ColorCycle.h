
#include <X10/X10_Effect.h>


class X10_ColorCycle : public X10_Effect
{
public:
	X10_ColorCycle(X10_Config &cfg, Adafruit_NeoMatrix *matrix, SdFat *sd)
		: X10_Effect(cfg, matrix, sd) {}

	void begin();
	void loop();

protected:
	unsigned long tick = 0;
	int sequencePosition = 0;
};