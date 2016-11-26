
#ifndef X10_H
#define X10_H

#include <Arduino.h>
#include <Adafruit_NeoMatrix.h>
#include <SPI.h>
#include <SdFat.h>

#include <X10/X10_Config.h>
#include <X10/X10_Base.h>
#include <X10/X10_Effect.h>
#include <X10/X10_Clock.h>



class X10 : public X10_Base
{
public:

	X10(X10_Config &, ArduinoOutStream &);

	void begin();
	void loop();

protected:

	void beginMatrix();
	void beginSdFat();

	Adafruit_NeoMatrix *matrix;
	SdFat *sd;

	X10_Clock *clock;
};

#endif /* X10_H */








