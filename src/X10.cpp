
#include <X10.h>


X10::X10(X10_Config &cfg)
	: X10_Base(cfg)
{
}

void X10::begin()
{
	print(F("Starting X10 initialization."));
	beginMatrix();
	beginSdFat();
	beginWifi();
	beginOTA();
	beginNTP();


	clock = new X10_Clock(cfg, *matrix, *sd);
}

void X10::loop()
{
	ArduinoOTA.handle();
	ntpClient->update();










}


// https://github.com/adafruit/Adafruit_NeoMatrix
void X10::beginMatrix()
{
	print(F("Initializing matrix..."));
	matrix = new Adafruit_NeoMatrix(cfg.width, cfg.height, cfg.matrixPin, cfg.matrixLayout, cfg.pixelType);
	matrix->begin();
	matrix->fillScreen(0);
	println(F(" done."));
}

void X10::beginSdFat()
{
	print(F("Initializing SD card..."));
	sd = new SdFat();
	if (!sd->cardBegin(cfg.csPin, cfg.spiSpeed))
	{
		println(F(" failed!"));
		return;
	}

	println(F(" done."));
}

void X10::beginWifi()
{
	print(F("Initializing WiFiManager..."));

	wifiManager = new WiFiManager();

	if(!wifiManager->autoConnect())
	{
		println(F(" failed to connect and hit timeout"));
    	// reset and try again
		ESP.reset();
    	delay(1000);
	} 

	println(F(" done."));
}

void X10::beginOTA()
{
	print(F("Initializing ArduinoOTA..."));

	ArduinoOTA.setPort(cfg.otaPort);
	ArduinoOTA.setHostname(cfg.otaName);
	//ArduinoOTA.setPassword(cfg.otaPass);

	ArduinoOTA.begin();

	// Restart everything after an update.
	ArduinoOTA.onEnd([]() {
		println(F("OTA Update done; rebooting."));
		ESP.reset();
		delay(1000);
	});

	println(" done.");
}

void X10::beginNTP()
{
	print(F("Initializing NTP Client..."));

	ntpUDP = new WiFiUDP();
	ntpClient = new NTPClient(*ntpUDP, cfg.ntpServer, cfg.ntpTimeOffset, cfg.ntpUpdateInterval);
	ntpClient->begin();

	println(" done.");
}