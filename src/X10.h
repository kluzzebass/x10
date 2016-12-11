
#ifndef X10_H
#define X10_H

#include <Arduino.h>
#include <Adafruit_NeoMatrix.h>
#include <SPI.h>
#include <SdFat.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <X10/X10_Config.h>
#include <X10/X10_Base.h>
#include <X10/X10_Effect.h>
#include <X10/X10_Clock.h>
#include <X10/X10_LEDTest.h>
#include <X10/X10_ColorCycle.h>




class X10 : public X10_Base
{
public:

	X10(X10_Config &cfg)
		: X10_Base(cfg) {}

	void begin();
	void loop();

protected:
	Adafruit_NeoMatrix *matrix;
	SdFat *sd;
	WiFiManager *wifiManager;
	WiFiUDP *ntpUDP;
	NTPClient *ntpClient;

	X10_Clock *clock;
	X10_LEDTest *ledTest;
	X10_ColorCycle *colorCycle;

	void bootStatus(int x, uint8_t r, uint8_t g, uint8_t b);

	void beginMatrix(int x);
	void beginSdFat(int x);
	void beginWifi(int x);
	void beginOTA(int x);
	void beginNTP(int x);

	void loopNTP();
};

#endif /* X10_H */








