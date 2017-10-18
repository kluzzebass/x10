
#include <X10.h>

void X10::begin()
{
	s.println(F("\n\nStarting X10 initialization."));
	beginMatrix(0);
	beginRTC(1);
//	beginWifi(2);

	clock = new X10_Clock(leds, rtc);
	ledTest = new X10_LEDTest(leds);
	colorCycle = new X10_ColorCycle(leds);
	wibbleWobble = new X10_WibbleWobble(leds);

	s.println(F("X10 initialization complete."));
	delay(1000);

	clear();
	FastLED.show();


	wibbleWobble->begin();

}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void X10::loop()
{
	
	clock->loop();
	// ledTest->loop();
	// colorCycle->loop();
	// wibbleWobble->loop();

}

void X10::bootStatus(int x, uint8_t r, uint8_t g, uint8_t b)
{
	leds[xy(x, 0)] = CRGB(r, g, b);
	FastLED.show();
}


// https://github.com/adafruit/Adafruit_NeoMatrix
void X10::beginMatrix(int x)
{
	s.println(F("Matrix: Initializing..."));

	FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
	FastLED.setBrightness(MAX_BRIGHTNESS);

	s.println(F("Matrix: Done."));
	bootStatus(x, 0, 100, 0);
}

void X10::beginRTC(int x)
{
	s.println(F("RTC: Initializing..."));

    rtc.Begin();

	// If RTC is invalid, set the time to compile time.
	RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

	if (!rtc.IsDateTimeValid()) 
    {
        s.println("RTC: Invalid time, setting clock to compile time.");
        rtc.SetDateTime(compiled);
    }

    if (!rtc.GetIsRunning())
    {
        Serial.println("RTC: Not actively running, starting now.");
        rtc.SetIsRunning(true);
	}

	rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);

	s.println(F("RTC: Done."));
	bootStatus(x, 0, 100, 0);
}

void X10::beginWifi(int x)
{
	// print(F("Initializing WiFiManager..."));
	// bootStatus(x, 100, 0, 100);

	// wifiManager = new WiFiManager();

	// if(!wifiManager->autoConnect())
	// {
	// 	println(F(" failed to connect and hit timeout"));
	// 	bootStatus(x, 100, 0, 0);
    // 	// reset and try again
    // 	delay(2000);
	// 	ESP.reset();
	// } 

	// println(F(" done."));
	// bootStatus(x, 0, 100, 0);
}

