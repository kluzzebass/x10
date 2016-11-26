
#include <X10/X10_Effect.h>

X10_Effect::X10_Effect(X10_Config &cfg, ArduinoOutStream &cout, Adafruit_NeoMatrix &matrix, SdFat &sd)
	: X10_Base(cfg, cout), matrix(matrix), sd(sd) {}