
#ifndef X10_EFFECT_H
#define X10_EFFECT_H

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <SdFat.h>

#include <X10/X10_Base.h>

class X10_Effect : public X10_Base
{
public:

	X10_Effect(X10_Config &cfg, Adafruit_NeoMatrix *matrix, SdFat *sd)
		: X10_Base(cfg)
	{
		this->matrix = matrix;
		this->sd = sd;
	}

	

protected:
	Adafruit_NeoMatrix *matrix;
	SdFat *sd;

	// Cyclic color sequencer with or without white
	int16_t colorSequence(int, bool);

};

#endif /* X10_EFFECT_H */
