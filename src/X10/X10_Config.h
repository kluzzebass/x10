
#ifndef X10_CONFIG_H
#define X10_CONFIG_H

#include <Arduino.h>
#include <Adafruit_NeoMatrix.h>
#include <SPI.h>
#include <SdFat.h>

class X10_Config
{
public:
	// For OTA updates
	int otaPort;

	// For the Adafruit_NeoMatrix library
	int matrixPin;
	int width;
	int height;
	int matrixLayout;
	int pixelType;
	int maxBrightness;

	// For the SdFat library
	int csPin;
	int spiSpeed;
	
	X10_Config();
};

#endif /* X10_CONFIG_H */
