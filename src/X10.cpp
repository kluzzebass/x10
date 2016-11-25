
#include <X10.h>


X10::X10(X10_Config &cfg, ArduinoOutStream &cout) : cfg(cfg), cout(cout)
{
}

void X10::begin()
{
	cout << endl << "Starting X10 initialization." << endl;
	beginMatrix();
	beginSdFat();
}

void X10::loop()
{

}

void X10::beginMatrix()
{
	matrix = new Adafruit_NeoMatrix(cfg.width, cfg.height, cfg.matrixPin, cfg.matrixLayout, cfg.pixelType);
	matrix->begin();
	matrix->fillScreen(0);
	cout << F("Matrix initialized.") << endl;
}

void X10::beginSdFat()
{
	sd = new SdFat();
	if (!sd->cardBegin(cfg.csPin, cfg.spiSpeed))
	{
		cout << F("sd->cardBegin() failed!") << endl;
		return;
	}

	cout << F("SD card initialized.") << endl;

}

