
#include <X10/X10_Config.h>

X10_Config::X10_Config()
{
	// A few sensible defaults
	stripPin = D1;
	csPin = D8;
	width = 16;
	height = 16;
	otaPort = 8266;

}