
#ifndef X10_BASE_H
#define X10_BASE_H


#include <X10/Config.h>

#include <Arduino.h>

#include <NeoPixelBus.h>
#include <NeoPixelBrightnessBus.h>
#define NeoPixelBusType NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod>

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <RtcDS1307.h>
#include <SPI.h> 
#include <SdFat.h>
#include <IniFile.h>

#include <BMPReader.h>

class X10_Base
{
public:

	X10_Base(
		NeoPixelBusType *leds,
		Stream &s)
		: leds(leds), s(s) {};

	virtual void begin() = 0;
	virtual void loop() = 0;

protected:
	NeoPixelBusType *leds;
	Stream &s;

	uint16_t xy(uint8_t x, uint8_t y);
	void fill(RgbColor color);
	void clear();

	void drawBitmap(BMPReader &bmp, uint16_t x, uint16_t y, uint16_t dx, uint16_t dy, uint16_t bmpX, uint16_t bmpY);
	void drawFrame(BMPReader &bmp, int16_t x, int16_t y);

};

#endif /* X10_BASE_H */
