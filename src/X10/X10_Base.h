
#ifndef X10_BASE_H
#define X10_BASE_H

#include <X10/X10_Config.h>
#include <SdFat.h>

#define print Serial.print
#define println Serial.println
#define printf Serial.printf

class X10_Base
{
public:

	X10_Base(X10_Config &cfg) : cfg(cfg) {}

	virtual void begin() = 0;
	virtual void loop() = 0;

protected:
	X10_Config &cfg;

};

#endif /* X10_BASE_H */
