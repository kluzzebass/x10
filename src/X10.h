
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

#define print Serial.print
#define println Serial.println
#define printf Serial.printf

class X10 : public X10_Base
{
public:

	X10(X10_Config &);

	void begin();
	void loop();

protected:

	void beginMatrix();
	void beginSdFat();
	void beginWifi();
	void beginOTA();
	void beginNTP();

	Adafruit_NeoMatrix *matrix;
	SdFat *sd;
	WiFiManager *wifiManager;
	WiFiUDP *ntpUDP;
	NTPClient *ntpClient;

	X10_Clock *clock;
};

#endif /* X10_H */








