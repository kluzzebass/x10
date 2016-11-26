
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

	X10_Effect(X10_Config &, ArduinoOutStream &, Adafruit_NeoMatrix &, SdFat &);

protected:
	Adafruit_NeoMatrix &matrix;
	SdFat &sd;
};

#endif /* X10_EFFECT_H */
