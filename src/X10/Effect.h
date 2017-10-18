
#ifndef X10_EFFECT_H
#define X10_EFFECT_H

#include <X10/Base.h>

class X10_Effect : public X10_Base
{
public:

	X10_Effect(CRGB *leds)
		: X10_Base(leds) {}

protected:

	// Cyclic color sequencer with or without white
	CRGB colorSequence(int, bool);

};

#endif /* X10_EFFECT_H */
