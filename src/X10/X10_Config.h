
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
	int otaPort = 8266;
	char *otaName = (char *)"x10";
	char *otaPass = NULL;
	
	// For NTP
	int ntpTimeOffset = 0;
	int ntpUpdateInterval = 60000; // ms
	char *ntpServer = (char *)"pool.ntp.org";

	// For the Adafruit_NeoMatrix library
	int matrixPin = D1;
	int width = 16;
	int height = 16;
	int matrixLayout = NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG;;
	int pixelType = NEO_GRB + NEO_KHZ800;
	int maxBrightness = 100;

	// For the SdFat library
	int csPin = D8;
	int spiSpeed = SPI_QUARTER_SPEED;
	
	X10_Config();
};

#endif /* X10_CONFIG_H */
