
#include <X10/X10_Effect.h>

X10_Effect::X10_Effect(X10_Config &cfg, Adafruit_NeoMatrix &matrix, SdFat &sd)
	: X10_Base(cfg), matrix(matrix), sd(sd) {}