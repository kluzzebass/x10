
#ifndef X10_H
#define X10_H

#include <X10/X10_Config.h>

class X10
{
public:
	X10_Config &cfg;

	X10(X10_Config &cfg);

	void begin(Stream &out);
	void tick();

private:
	Stream *out;

};

#endif /* X10_H */








