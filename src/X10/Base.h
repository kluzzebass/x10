
#ifndef X10_BASE_H
#define X10_BASE_H

#include <X10/Config.h>

#include <Arduino.h>

#include <FastLED.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#define s Serial

class X10_Base
{
public:

	X10_Base(CRGB *leds)
		: leds(leds) {};

	virtual void begin() = 0;
	virtual void loop() = 0;

protected:
	CRGB *leds;

	uint16_t xy(uint8_t x, uint8_t y);
	void fill(CRGB color);
	void clear();
};

#endif /* X10_BASE_H */
