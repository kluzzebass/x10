
#include <X10.h>

SdFat SD;

void X10::begin()
{
	s.println(F("\n\nStarting X10 initialization."));
	beginMatrix(0);
	beginSD(1);
	beginConfig(2);
	beginRTC(3);
	beginWifi(4);

	clock = new X10_Clock(leds, rtc);
	ledTest = new X10_LEDTest(leds);
	colorCycle = new X10_ColorCycle(leds);
	wibbleWobble = new X10_WibbleWobble(leds);
	animator = new X10_Animator(leds);

	clock->begin();
	ledTest->begin();
	colorCycle->begin();
	wibbleWobble->begin();
	animator->begin();

	s.println(F("X10 initialization complete."));
	delay(1000);

	clear();
	FastLED.show();

}


void X10::loop()
{
	
	// clock->loop();
	// ledTest->loop();
	// colorCycle->loop();
	// wibbleWobble->loop();
	animator->loop();

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
	FastLED.setCorrection(colorCorrection);
	FastLED.setTemperature(colorTemperature);

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

void X10::beginSD(int x)
{
	s.println(F("SD: Initializing card..."));
	bootStatus(x, 100, 0, 100);

	// sd = new SdFat();
	if (!SD.begin(CS_PIN, SPI_SPEED))
	{
		s.println(F("SD: Failed!"));
		bootStatus(x, 100, 0, 0);
		return;
	}

	s.println(F("SD: Done."));
	bootStatus(x, 0, 100, 0);
}

void X10::beginConfig(int x)
{
	char buffer[BUFFER_LEN];

	s.println(F("CFG: Reading config file..."));
	bootStatus(x, 100, 0, 100);

	SD.chdir(X10_CFG_DIR);
	IniFile ini(X10_CFG_FILE);

	if (!ini.open()) {
		s.println(F("CFG: Unable to open config file!"));
		bootStatus(x, 100, 0, 0);
		return;		
	}

	if (!ini.validate(buffer, BUFFER_LEN)) {
		s.print(F("CFG: Error validating config, error: "));
		s.println(ini.getError());
		bootStatus(x, 100, 0, 0);
		return;		
	}

	if (!ini.getValue("wifi", "ssid", buffer, BUFFER_LEN)) {
		s.println(F("CFG: Error reading wifi ssid!"));
		bootStatus(x, 100, 0, 0);
		return;
	}
	wifi_ssid = strdup(buffer);

	if (!ini.getValue("wifi", "psk", buffer, BUFFER_LEN)) {
		s.println(F("CFG: Error reading wifi psk!"));
		bootStatus(x, 100, 0, 0);
		return;
	}
	wifi_psk = strdup(buffer);

	if (!ini.getIPAddress("ntp", "server", buffer, BUFFER_LEN, ntp_server)) {
		s.println(F("CFG: Error reading ntp server!"));
		bootStatus(x, 100, 0, 0);
		return;
	}

	s.print("CFG: wifi ssid: ");
	s.println(wifi_ssid);
	s.print("CFG: wifi psk: ");
	s.println(wifi_psk);
	s.print("CFG: ntp server: ");
	s.println(ntp_server);

	s.println(F("CFG: Done."));
	bootStatus(x, 0, 100, 0);
}


void X10::beginWifi(int x)
{
	s.print(F("Wifi: Initializing"));
	bootStatus(x, 100, 0, 100);

	WiFi.mode(WIFI_STA);
	WiFi.begin(wifi_ssid, wifi_psk);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		s.print(".");
	}

	s.println("");
	s.print("Wifi: Connected, address: ");  
	s.println(WiFi.localIP());
	s.println(F("Wifi: Done."));
	bootStatus(x, 0, 100, 0);
}

