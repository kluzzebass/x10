
#include <X10/X10_Config.h>

X10_Config::X10_Config()
{
	// A few sensible defaults
	otaPort = 8266;

	matrixPin = D1;
	width = 16;
	height = 16;
	matrixLayout = NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG;
	pixelType = NEO_GRB + NEO_KHZ800;
	maxBrightness = 100;

	csPin = D8;
	spiSpeed = SPI_QUARTER_SPEED;
}