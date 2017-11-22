
#ifndef X10_H
#define X10_H

#include <X10/Base.h>

#include <X10/Clock.h>
#include <X10/LEDTest.h>
#include <X10/ColorCycle.h>
#include <X10/WibbleWobble.h>
#include <X10/Animator.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

extern SdFat SD;

class X10 : public X10_Base
{
public:

	X10(CRGB *leds, Stream &s, RtcDS1307<TwoWire> &rtc)
		: X10_Base(leds, s), rtc(rtc) {}

	void begin();
	void loop();

protected:

	RtcDS1307<TwoWire> &rtc;
	ESP8266WebServer srv;

	X10_Clock *clock;
	X10_LEDTest *ledTest;
	X10_ColorCycle *colorCycle;
	X10_WibbleWobble *wibbleWobble;
	X10_Animator *animator;

	uint8_t currentEffect;

	// Matrix Config
	uint32_t colorCorrection = COLOR_CORRECTION;
	uint32_t colorTemperature = COLOR_TEMPERATURE;

	// Wifi Config
	char *wifiSSID = NULL;
	char *wifiPSK = NULL;

	// NTP Config
	IPAddress ntpServer;

	// Animation config
	char *animDir;
	char *animCfgFile;


	void bootStatus(int x, uint8_t r, uint8_t g, uint8_t b);

	void beginMatrix(int x);
	void beginSD(int x);
	void beginRTC(int x);
	void beginConfig(int x);
	void beginWifi(int x);
	void beginWeb(int x);

	void handleAPI(String &path);
	void handleStaticContent();

};

#endif /* X10_H */

