
#include <X10.h>

X10::X10(X10_Config &cfg) : cfg(cfg)
{
}

void X10::begin(Stream &out)
{
	this->out = &out;
}

void X10::tick()
{

}