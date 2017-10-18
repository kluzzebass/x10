
#ifndef X10_H
#define X10_H

#include <X10/Base.h>

#include <X10/Clock.h>
#include <X10/LEDTest.h>
#include <X10/ColorCycle.h>
#include <X10/WibbleWobble.h>

class X10 : public X10_Base
{
public:

	X10(CRGB *leds, RtcDS1307<TwoWire> &rtc)
		: X10_Base(leds), rtc(rtc) {}

	void begin();
	void loop();

protected:
	// WiFiManager *wifiManager;

	RtcDS1307<TwoWire> &rtc;

	X10_Clock *clock;
	X10_LEDTest *ledTest;
	X10_ColorCycle *colorCycle;
	X10_WibbleWobble *wibbleWobble;

	void bootStatus(int x, uint8_t r, uint8_t g, uint8_t b);

	void beginMatrix(int x);
	void beginRTC(int x);
	void beginWifi(int x);

};

#endif /* X10_H */

