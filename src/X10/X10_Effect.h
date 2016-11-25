
#ifndef X10_EFFECT_H
#define X10_EFFECT_H

#include <X10.h>

class X10_Effect
{
public:

	X10_Effect(X10_Config &cfg, ArduinoOutStream &cout);

	virtual void begin() = 0;
	virtual void loop() = 0;

protected:
	X10_Config &cfg;
	ArduinoOutStream &cout;
};

#endif /* X10_EFFECT_H */
