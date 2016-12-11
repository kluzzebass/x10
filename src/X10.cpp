
#include <X10.h>



void X10::begin()
{
	println(F("\n\nStarting X10 initialization."));
	beginMatrix(0);
	beginSdFat(1);
	beginWifi(2);
	beginOTA(3);
	beginNTP(4);

	clock = new X10_Clock(cfg, matrix, sd);
	ledTest = new X10_LEDTest(cfg, matrix, sd);
	colorCycle = new X10_ColorCycle(cfg, matrix, sd);


	println(F("X10 initialization complete."));
	delay(1000);
	matrix->fillScreen(0);
	matrix->show();

}

void X10::loop()
{
	ArduinoOTA.handle();
	//loopNTP();

//	clock->loop();
	colorCycle->loop();
}

void X10::bootStatus(int x, uint8_t r, uint8_t g, uint8_t b)
{
	matrix->drawPixel(x, cfg.height - 1, matrix->Color(r, g, b));
	matrix->show();
}


// https://github.com/adafruit/Adafruit_NeoMatrix
void X10::beginMatrix(int x)
{
	print(F("Initializing matrix..."));

	matrix = new Adafruit_NeoMatrix(cfg.width, cfg.height, cfg.matrixPin, cfg.matrixLayout, cfg.pixelType);
	matrix->begin();
	matrix->setBrightness(cfg.maxBrightness);
	matrix->fillScreen(0);

	println(F(" done."));
	bootStatus(x, 0, 100, 0);
}

// https://github.com/greiman/SdFat
void X10::beginSdFat(int x)
{
	print(F("Initializing SD card..."));
	bootStatus(x, 100, 0, 100);

	sd = new SdFat();
	if (!sd->cardBegin(cfg.csPin, cfg.spiSpeed))
	{
		println(F(" failed!"));
		bootStatus(x, 100, 0, 0);
		return;
	}

	println(F(" done."));
	bootStatus(x, 0, 100, 0);
}

void X10::beginWifi(int x)
{
	print(F("Initializing WiFiManager..."));
	bootStatus(x, 100, 0, 100);

	wifiManager = new WiFiManager();

	if(!wifiManager->autoConnect())
	{
		println(F(" failed to connect and hit timeout"));
		bootStatus(x, 100, 0, 0);
    	// reset and try again
    	delay(2000);
		ESP.reset();
	} 

	println(F(" done."));
	bootStatus(x, 0, 100, 0);
}

// https://github.com/esp8266/Arduino/blob/master/doc/ota_updates/readme.md
void X10::beginOTA(int x)
{
	print(F("Initializing ArduinoOTA..."));
	bootStatus(x, 100, 0, 100);

	ArduinoOTA.setPort(cfg.otaPort);
	ArduinoOTA.setHostname(cfg.otaName);
	//ArduinoOTA.setPassword(cfg.otaPass);

	ArduinoOTA.begin();

	ArduinoOTA.onStart([]() {
		println(F("Starting OTA update."));
	});

	// Restart everything after an update.
	ArduinoOTA.onEnd([]() {
		println(F("OTA Update done, rebooting."));
		ESP.reset();
		delay(1000);
	});

	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		printf("OTA update progress: %u%%\r", (progress / (total / 100)));
	});

	ArduinoOTA.onError([](ota_error_t error) {
		printf("Error[%u]: ", error);
		switch(error)
		{
			case OTA_AUTH_ERROR:
				println(F("Auth Failed"));
				break;
			case OTA_BEGIN_ERROR:
				println(F("Begin Failed"));
				break;
			case OTA_CONNECT_ERROR:
				println(F("Connect Failed"));
				break;
			case OTA_RECEIVE_ERROR:
				println(F("Receive Failed"));
				break;
			case OTA_END_ERROR:
				println(F("End Failed"));
				break;
			default:
				println(F("Unknown error"));
		}
	});

	println(F(" done."));
	bootStatus(x, 0, 100, 0);
}

// https://github.com/arduino-libraries/NTPClient
void X10::beginNTP(int x)
{
	print(F("Initializing NTP Client..."));
	bootStatus(x, 100, 0, 100);

	ntpUDP = new WiFiUDP();
	ntpClient = new NTPClient(*ntpUDP, cfg.ntpServer, cfg.ntpTimeOffset, cfg.ntpUpdateInterval);
	ntpClient->begin();

	println(F(" done."));
	bootStatus(x, 0, 100, 0);
}




void X10::loopNTP()
{
		ntpClient->update();

}



