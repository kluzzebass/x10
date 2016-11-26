
#ifndef X10_CLOCK_H
#define X10_CLOCK_H

#include <X10/X10_Effect.h>

class X10_Clock : public X10_Effect
{
public:

	X10_Clock(X10_Config &cfg, ArduinoOutStream &cout, Adafruit_NeoMatrix &matrix, SdFat &sd);

	void begin();
	void loop();
};

#endif /* X10_CLOCK_H */
