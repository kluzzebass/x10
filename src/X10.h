
#ifndef X10_H
#define X10_H

#include <X10/Base.h>

#include <X10/LEDTest.h>
#include <X10/Animator.h>
#include <X10/Clock.h>
#include <X10/ColorCycle.h>
#include <X10/WibbleWobble.h>
#include <X10/Fake3D.h>

#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <FS.h>
#include <ArduinoOTA.h>

class X10 : public X10_Base
{
public:

	X10(
		NeoPixelBusType *leds,
		 Stream &s, RtcDS1307<TwoWire> &rtc)
		: X10_Base(leds, s), rtc(rtc) {
		srv = new AsyncWebServer(80);
	}

	void begin();
	void loop();

protected:

	struct mime_t
	{
		String suffix;
		String subtype;
	};

	#define MIME_TYPES 16

	mime_t mimeTypes[MIME_TYPES] = {
		{ ".",		"application/octet-stream" },
		{ ".txt",	"text/plain" },
		{ ".htm",	"text/html" },
		{ ".html",	"text/html" },
		{ ".css",	"text/css" },
		{ ".scss",	"text/scss" },
		{ ".xml",	"text/xml" },
		{ ".js",	"application/javascript" },
		{ ".png",	"image/png" },
		{ ".gif",	"image/gif" },
		{ ".jpg",	"image/jpeg" },
		{ ".ico",	"image/x-icon" },
		{ ".json",	"application/json" },
		{ ".pdf",	"application/x-pdf" },
		{ ".zip",	"application/x-zip" },
		{ ".gz",	"application/x-gzip" }
	};

	// Quick access to the JSON and text/plain mime subtypes
	int jsonm;
	int plainm;

	RtcDS1307<TwoWire> &rtc;
	AsyncWebServer *srv;

	#define EFFECTS 6

	String effects[EFFECTS] = {
		"LED Test",
		"Animator",
		"Clock",
		"Color Cycle",
		"Wibble Wobble",
		"Fake 3D"
	};

	X10_LEDTest *ledTest;
	X10_Animator *animator;
	X10_Clock *clock;
	X10_ColorCycle *colorCycle;
	X10_WibbleWobble *wibbleWobble;
	X10_Fake3D *fake3D;

	// Power On/Off switch
	bool power;
	bool switchPower;

	// Current brightness
	uint8_t currentBrightness;

	// Currently running effect
	uint8_t currentEffect;
	bool initializePlz;

	// File handke for the static web content directory
	FatFile wd;

	struct cfg_t {
		// Net Config
		char *wifiSSID = NULL;
		char *wifiPSK = NULL;
		IPAddress ip;
		IPAddress gateway;
		IPAddress netmask;

		// Display Config
		uint16_t maxBrightness = MAX_BRIGHTNESS;
		uint16_t minBrightness = MIN_BRIGHTNESS;

		// Web Config
		char *webDir = NULL;
		char *indexFile = NULL;

		// Animation config
		char *animDir = NULL;
		char *animCfgFile = NULL;
	};

	cfg_t cfg;

	// Anything in here will be persisted to /settings
	struct persist_t {
		uint8_t effect;
		uint8_t brightness;
		uint8_t animCycle;
		bool animRandom;
	};

	void writeSettings();
	void readSettings();

	void beginMatrix(int x);
	void beginSD(int x);
	void beginRTC(int x);
	void beginConfig(int x);
	void beginWifi(int x);
	void beginOTA(int x);
	void beginWeb(int x);

	void registerWebHandlers();
	void notFound(AsyncWebServerRequest *request);
	void badRequest(AsyncWebServerRequest *request);
	int mimeTypeIndex(String &path);

	bool setPower(bool onon);
	bool switchEffect(uint8_t effect);
	bool setBrightness(uint8_t brightness);

	// API methods
	void jsonStatus(AsyncWebServerRequest *request, int status, String title);
	void jsonBadRequest(AsyncWebServerRequest *request);
	void jsonNotFound(AsyncWebServerRequest *request);
	void jsonOk(AsyncWebServerRequest *request);

};

#endif /* X10_H */

