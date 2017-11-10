
#ifndef X10_BASE_H
#define X10_BASE_H


#include <X10/Config.h>

#include <Arduino.h>

#include <FastLED.h>

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <RtcDS1307.h>
#include <SPI.h> 
#include <SdFat.h>
#include <IniFile.h>

#include <BMPReader.h>

#define s Serial


class X10_Base
{
public:
	enum error_t {
		eNoError = 0,
		eFileNotFound,
		eBitMapHeader,
		eBitMapFileHeader,
		eBitMapPlanes,
		eBitMapDepth,
		eBitMapCompression
	};

	X10_Base(CRGB *leds)
		: leds(leds) {};

	virtual void begin() = 0;
	virtual void loop() = 0;

protected:
	CRGB *leds;

	uint16_t xy(uint8_t x, uint8_t y);
	void fill(CRGB color);
	void clear();

	void drawBitmap(BMPReader &bmp, uint8_t x, uint8_t y, uint16_t dx, uint16_t dy, uint16_t bmpX, uint16_t bmpY);

};

#endif /* X10_BASE_H */
