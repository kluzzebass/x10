
#ifndef X10_EFFECT_H
#define X10_EFFECT_H

#include <X10/Base.h>

class X10_Effect : public X10_Base
{
public:

	X10_Effect(
		NeoPixelBusType *leds,
		Stream &s)
		: X10_Base(leds, s) {}

	virtual void init() = 0;

};

#endif /* X10_EFFECT_H */
