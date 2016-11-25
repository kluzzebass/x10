
#ifndef X10_H
#define X10_H

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>

#include <X10/X10_Config.h>
#include <X10/X10_Effect.h>
#include <X10/X10_Clock.h>



class X10
{
public:

	X10(X10_Config &cfg, ArduinoOutStream &cout);

	void begin();
	void loop();

protected:
	X10_Config &cfg;
	ArduinoOutStream &cout;
	Adafruit_NeoMatrix *matrix;
	SdFat *sd;

	void beginMatrix();
	void beginSdFat();


};

#endif /* X10_H */








