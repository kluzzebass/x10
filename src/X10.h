
#ifndef X10_H
#define X10_H

#include <X10/Base.h>

#include <X10/Clock.h>
#include <X10/LEDTest.h>
#include <X10/ColorCycle.h>
#include <X10/WibbleWobble.h>
#include <X10/Animator.h>

extern SdFat SD;

class X10 : public X10_Base
{
public:

	X10(CRGB *leds, RtcDS1307<TwoWire> &rtc)
		: X10_Base(leds), rtc(rtc) {}

	void begin();
	void loop();

protected:

	RtcDS1307<TwoWire> &rtc;
	
	X10_Clock *clock;
	X10_LEDTest *ledTest;
	X10_ColorCycle *colorCycle;
	X10_WibbleWobble *wibbleWobble;
	X10_Animator *animator;

	// Matrix Config
	uint32_t colorCorrection = COLOR_CORRECTION;
	uint32_t colorTemperature = COLOR_TEMPERATURE;

	// Wifi Config
	char *wifi_ssid = NULL;
	char *wifi_psk = NULL;
	IPAddress ntp_server;

	// NTP Config


	void bootStatus(int x, uint8_t r, uint8_t g, uint8_t b);

	void beginMatrix(int x);
	void beginSD(int x);
	void beginRTC(int x);
	void beginConfig(int x);
	void beginWifi(int x);

};

#endif /* X10_H */

