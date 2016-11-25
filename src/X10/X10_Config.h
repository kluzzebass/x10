
#ifndef X10_CONFIG_H
#define X10_CONFIG_H

#include <Arduino.h>

class X10_Config
{
public:
	int stripPin;
	int csPin;
	int width;
	int height;
	int otaPort;
	
	X10_Config();
};

#endif /* X10_CONFIG_H */
