
#include <X10.h>

SdFat SD;

void X10::begin()
{
	s.println(F("\n\nStarting X10 initialization."));

	// Insert some hardware info here

	// Seed the random generator
	unsigned long seed = 0;
	for (unsigned int i = 0; i < sizeof(seed * (unsigned long)8); i++)
	{
		seed <<= 1;
		seed |= analogRead(0) & 1;
	}
	randomSeed(seed);

	SPIFFS.begin();
	readSettings();

	beginMatrix(0);
	beginSD(1);
	beginConfig(2);
	beginRTC(3);
	beginWifi(4);
	beginWeb(5);

	// Set up the Clock Effect
	clock = new X10_Clock(leds, s, rtc);
	clock->begin();

	// Set up the LEDTest Effect
	ledTest = new X10_LEDTest(leds, s);
	ledTest->begin();

	// Set up the ColorCycle Effect
	colorCycle = new X10_ColorCycle(leds, s);
	colorCycle->begin();

	// Set up the WibbleWobble Effect
	wibbleWobble = new X10_WibbleWobble(leds, s);
	wibbleWobble->begin();

	// Set up the Animator Effect
	animator = new X10_Animator(leds, s, cfg.animDir, cfg.animCfgFile, SD);
	animator->begin();

	// Make sure the first effect is initialized
	initializePlz = true;

	clear();
	leds->Show();

	s.println(F("X10 initialization complete."));
}

void X10::loop()
{
	switch (currentEffect)
	{
		case 1:
			if (initializePlz) animator->init();
			animator->loop();
			break;
		case 2:
			if (initializePlz) clock->init();
			clock->loop();
			break;
		case 3:
			if (initializePlz) colorCycle->init();
			colorCycle->loop();
			break;
		case 4:
			if (initializePlz) wibbleWobble->init();
			wibbleWobble->loop();
			break;
		default:
			if (initializePlz) ledTest->init();
			ledTest->loop();
			break;
	}
	initializePlz = false;
}


void X10::bootStatus(int x, uint8_t r, uint8_t g, uint8_t b)
{
	leds->SetPixelColor(xy(x, 0), RgbColor(r, g, b));
	leds->Show();
}


void X10::writeSettings()
{
	persist_t p;
	memset(&p, 0, sizeof(persist_t));

	s.println(F("Settings: Writing..."));

	// Populate the persist structure
	p.effect = currentEffect;
	p.brightness = currentBrightness;

	fs::File f = SPIFFS.open(X10_SETTINGS_FILE, "w");
	if (!f) {
		s.println("Settings: File open failed.");
	} else {
		size_t wrote = f.write((uint8_t*)&p, sizeof(persist_t));
		f.close();
		s.print(F("Settings: Wrote "));
		s.print(wrote);
		s.println(F(" bytes."));
	}
	s.println(F("Settings: Done."));
}

void X10::readSettings()
{
	persist_t p;
	memset(&p, 0, sizeof(persist_t));

	// Read from settings
	s.println(F("Settings: Reading..."));

	// Fill inn the defaults
	p.brightness = MIN_BRIGHTNESS;
	p.effect = DEFAULT_EFFECT;

	if (SPIFFS.exists(X10_SETTINGS_FILE)) {
		fs::File f = SPIFFS.open(X10_SETTINGS_FILE, "r");
		if (!f) {
			s.println("Settings: File open failed");
		} else {
			size_t read = f.read((uint8_t*)&p, sizeof(persist_t));
			f.close();
			s.print(F("Settings: Read "));
			s.print(read);
			s.println(F(" bytes."));
		}
	}

	s.print("Settings: effect = ");
	s.println(p.effect);
	s.print("Settings: brightness = ");
	s.println(p.brightness);

	// Spread persisted data
	setBrightness(p.brightness);
	switchEffect(p.effect);

	s.println(F("Settings: Done."));
}


void X10::beginMatrix(int x)
{
	s.println(F("Matrix: Initializing..."));

	leds->Begin();

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
		s.println(F("RTC: Invalid time, setting clock to compile time."));
		rtc.SetDateTime(compiled);
	}

	if (!rtc.GetIsRunning())
	{
		Serial.println(F("RTC: Not actively running, starting now."));
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
	const char *net = "net";
	const char *display = "display";
	const char *web = "web";
	const char *animator = "animator";

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

	// Net

	if (!ini.getValue(net, "ssid", buffer, BUFFER_LEN)) {
		s.println(F("CFG: Error reading wifi ssid!"));
		bootStatus(x, 100, 0, 0);
		return;
	}
	cfg.wifiSSID = strdup(buffer);

	if (!ini.getValue(net, "psk", buffer, BUFFER_LEN)) {
		s.println(F("CFG: Error reading wifi psk!"));
		bootStatus(x, 100, 0, 0);
		return;
	}
	cfg.wifiPSK = strdup(buffer);

	ini.getIPAddress(net, "ip", buffer, BUFFER_LEN, cfg.ip);
	ini.getIPAddress(net, "gateway", buffer, BUFFER_LEN, cfg.gateway);
	ini.getIPAddress(net, "netmask", buffer, BUFFER_LEN, cfg.netmask);

	// Display

	ini.getValue(display, "maxBrightness", buffer, BUFFER_LEN, cfg.maxBrightness);
	if (cfg.maxBrightness <= 0 || cfg.maxBrightness > MAX_BRIGHTNESS)
		cfg.maxBrightness = MAX_BRIGHTNESS;

	ini.getValue(display, "minBrightness", buffer, BUFFER_LEN, cfg.minBrightness);
	if (cfg.minBrightness < MIN_BRIGHTNESS)
		cfg.minBrightness = MIN_BRIGHTNESS;
	if (cfg.minBrightness > cfg.maxBrightness)
		cfg.minBrightness = cfg.maxBrightness;

	// Web

	if (ini.getValue(web, "webDir", buffer, BUFFER_LEN)) {
		cfg.webDir = strdup(buffer);
	}

	if (ini.getValue(web, "indexFile", buffer, BUFFER_LEN)) {
		cfg.indexFile = strdup(buffer);
	}

	// Animator

	if (ini.getValue(animator, "animDir", buffer, BUFFER_LEN)) {
		cfg.animDir = strdup(buffer);
	}

	if (!ini.getValue(animator, "animCfgFile", buffer, BUFFER_LEN)) {
		cfg.animCfgFile = strdup(buffer);
	}
	
	s.println(F("CFG: Done."));
	bootStatus(x, 0, 100, 0);
}


void X10::beginWifi(int x)
{
	s.println(F("Wifi: Initializing..."));
	bootStatus(x, 100, 0, 100);

	WiFi.mode(WIFI_STA);

	// Static IP? All three addresses need to be filled in.
	if (cfg.ip != 0 && cfg.gateway != 0 && cfg.netmask != 0)
	{
		s.print(F("Wifi: Setting static ip: "));
		s.println(cfg.ip);
		WiFi.config(cfg.ip, cfg.gateway, cfg.netmask);
	}

	WiFi.begin(cfg.wifiSSID, cfg.wifiPSK);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}

	s.print(F("Wifi: Connected, address: "));
	s.println(WiFi.localIP());
	s.println(F("Wifi: Done."));
	bootStatus(x, 0, 100, 0);
}

void X10::beginWeb(int x)
{
	String jsons = ".json";
	String txts = ".txt";
	jsonm = mimeTypeIndex(jsons);
	plainm = mimeTypeIndex(txts);

	// Open the webdir
	if (cfg.webDir != NULL)
	{
		wd = SD.open(cfg.webDir);

		if (wd.isOpen() && !wd.isDir())
		{
			wd.close();
		}
	}

	registerWebHandlers();
	srv->begin();

	s.println(F("Web: Done."));
	bootStatus(x, 0, 100, 0);
}

bool X10::setBrightness(uint8_t brightness)
{
	// Cap the brightness
	if (brightness < cfg.minBrightness)
		brightness = cfg.minBrightness;
	if (brightness > cfg.maxBrightness)
		brightness = cfg.maxBrightness;

	s.print(F("Setting brightness to: "));
	s.println(brightness);

	currentBrightness = brightness;
	leds->SetBrightness(brightness);

	return true;
}

bool X10::switchEffect(uint8_t effect)
{
	if (effect >= EFFECTS) return false;

	s.print(F("Switching to effect: "));
	s.println(effect);

	currentEffect = effect;
	initializePlz = true;

	writeSettings();

	return true;
}




/*
 *
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 												WEB
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

void X10::registerWebHandlers() {

	// Deal with CORS
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE,OPTIONS");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type, Access-Control-Allow-Headers, Authorization, X-Requested-With");


	// API

	// curl -s http://x10/api/display | jq .
	srv->on("/api/display", HTTP_GET, [&](AsyncWebServerRequest *request) {
		String msg;

		msg += "{\"brightness\":";
		msg += leds->GetBrightness();
		msg += ",\"maxBrightness\":";
		msg += cfg.maxBrightness;
		msg += ",\"minBrightness\":";
		msg += cfg.minBrightness;
		msg += "}\n";

		request->send(200, mimeTypes[jsonm].subtype, msg);
	});

	// curl -s http://x10/api/effect | jq .
	srv->on("/api/effect", HTTP_GET, [&](AsyncWebServerRequest *request) {
		String msg;

		msg += "{\"current\":";
		msg += currentEffect;
		msg += ",\"effects\":[";
		for (int i = 1; i < EFFECTS; i++)
		{

			msg += "{\"id\":";
			msg += i;
			msg += ",\"name\":\"";
			msg += effects[i];
			msg += "\"}";
			if (i < (EFFECTS - 1)) msg += ",";
		}
		msg += "]}\n";

		request->send(200, mimeTypes[jsonm].subtype, msg);
	});

	// curl -s http://x10/api/animator | jq .
	srv->on("/api/animator", HTTP_GET, [&](AsyncWebServerRequest *request) {
		String msg;

		File d;
		if (!d.open(cfg.animDir))
		{
			s.print(F("Unable to open animation directory: "));
			s.println(cfg.animDir);
			return;
		}

		const uint16_t *cycleTimes = animator->getCycleTimes();

		msg += "{\"currentAnimation\":\"";
		msg += animator->currentAnim();
		msg += "\",\"randomize\":";
		msg += animator->getRandomize() ? "true" : "false";
		msg += ",\"cycle\":";
		msg += animator->getCycle();
		msg += ",\"cycleTimes\":[";
		for (int i = 0; i < animator->getCycleTimesCount(); i++)
		{
			msg += cycleTimes[i];
			if (i < (animator->getCycleTimesCount() - 1)) msg += ",";
		}
		msg += "],\"animations\":[";
		while (File f = d.openNextFile())
		{
			char buffer[BUFFER_LEN + 1];
			f.getName(buffer, BUFFER_LEN);
			msg += "\"";
			msg += buffer;
			msg += "\",";
		}
		if (msg.endsWith(",")) msg.remove(msg.length() - 1, 1);
		msg += "]}\n";

		d.close();

		request->send(200, mimeTypes[jsonm].subtype, msg);
	});

	// curl -s http://x10/api/datetime | jq .
	srv->on("/api/datetime", HTTP_GET, [&](AsyncWebServerRequest *request) {
		String msg;

		RtcDateTime dt = rtc.GetDateTime();
		String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
		char d[BUFFER_LEN];
		char t[BUFFER_LEN];

		snprintf(d, BUFFER_LEN, "%s %d %04d", months[dt.Month() - 1].c_str(), dt.Day(), dt.Year());
		snprintf(t, BUFFER_LEN, "%02d:%02d:%02d", dt.Hour(), dt.Minute(), dt.Second());

		msg += "{\"date\":\"";
		msg += d;
		msg += "\",\"time\":\"";
		msg += t;
		msg += "\"}\n";

		request->send(200, mimeTypes[jsonm].subtype, msg);
	});

	// curl -s http://x10/api/wibblewobble | jq .
	srv->on("/api/wibblewobble", HTTP_GET, [&](AsyncWebServerRequest *request) {
		String msg;

		msg += "{\"wibbleRange\":";
		msg += WIBBLE_RANGE;
		msg += ",\"wobbleRange\":";
		msg += WOBBLE_RANGE;
		msg += ",\"wibbleX\":";
		msg += wibbleWobble->wibbleX();
		msg += ",\"wibbleY\":";
		msg += wibbleWobble->wibbleY();
		msg += ",\"wobbleX\":";
		msg += wibbleWobble->wobbleX();
		msg += ",\"wobbleY\":";
		msg += wibbleWobble->wobbleY();
		msg += ",\"changeRate\":";
		msg += wibbleWobble->changeRate();
		msg += "}\n";

		request->send(200, mimeTypes[jsonm].subtype, msg);
	});

	// curl -s -X POST -H "Content-Type: application/json" -d '{"brightness":50}' http://x10/api/display
	srv->addHandler(new AsyncCallbackJsonWebHandler("/api/display", [&](AsyncWebServerRequest *request, JsonVariant &json) {
		JsonObject& root = json.as<JsonObject>();
		int brightness;
		switch (request->method()) {
			case HTTP_POST:
				if (!root.success()) {
					jsonBadRequest(request);
					break;
				}
				brightness = root[F("brightness")];

				if (!setBrightness(brightness)) {
					jsonBadRequest(request);
					break;
				}

				writeSettings();
				jsonOk(request);
				break;
			default:
				jsonNotFound(request);
				break;
		}	
	}));

	// curl -s -X POST -H "Content-Type: application/json" -d '{"current":1}' http://x10/api/effect | jq .
	srv->addHandler(new AsyncCallbackJsonWebHandler("/api/effect", [&](AsyncWebServerRequest *request, JsonVariant &json) {
		JsonObject& root = json.as<JsonObject>();
		int effect;
		switch (request->method()) {
			case HTTP_POST:
				if (!root.success()) {
					jsonBadRequest(request);
					break;
				}
				effect = root[F("current")];

				if (!switchEffect(effect)) {
					jsonBadRequest(request);
					break;
				}

				writeSettings();
				jsonOk(request);
				break;
			default:
				jsonNotFound(request);
				break;
		}	
	}));

	// curl -s -X POST -H "Content-Type: application/json" -d '{"date":"Nov 25 2017","time":"18:28:45"}' http://x10/api/datetime | jq .
	srv->addHandler(new AsyncCallbackJsonWebHandler("/api/datetime", [&](AsyncWebServerRequest *request, JsonVariant &json) {
		JsonObject& root = json.as<JsonObject>();
		switch (request->method()) {
			case HTTP_POST:
				if (!root.success()) {
					jsonBadRequest(request);
					break;
				}

				if (root.containsKey("date") && root.containsKey("time"))
				{
					if (!root["date"].is<const char*>()) {
						jsonBadRequest(request);
						break;
					}

					if (!root["time"].is<const char*>()) {
						jsonBadRequest(request);
						break;
					}

					String d = root[F("date")].as<char*>();
					String t = root[F("time")].as<char*>();

					RtcDateTime compiled = RtcDateTime(d.c_str(), t.c_str());
					rtc.SetDateTime(compiled);

					jsonOk(request);
				} else {
					jsonBadRequest(request);
					break;
				}
				break;
			default:
				jsonNotFound(request);
				break;
		}	
	}));

	// curl -s -X POST -H "Content-Type: application/json" -d '{"randomize":true}' http://x10/api/animator | jq .
	// curl -s -X POST -H "Content-Type: application/json" -d '{"cycle":1}' http://x10/api/animator | jq .
	// curl -s -X POST -H "Content-Type: application/json" -d '{"next":true}' http://x10/api/animator | jq .
	// curl -s -X POST -H "Content-Type: application/json" -d '{"next":""}' http://x10/api/animator | jq .
	srv->addHandler(new AsyncCallbackJsonWebHandler("/api/animator", [&](AsyncWebServerRequest *request, JsonVariant &json) {
		JsonObject& root = json.as<JsonObject>();
		switch (request->method()) {
			case HTTP_POST:
				if (!root.success()) {
					jsonBadRequest(request);
					break;
				}
				if (root.containsKey("next"))
				{
					if (root["next"].is<bool>())
					{
						bool next = root[F("next")];
						if (next) animator->next();
					}
					else if (root["next"].is<const char *>())
					{
						const char *next = root[F("next")];
						if (!animator->next(next)) {
							jsonNotFound(request);
							break;
						}
					}
					else {
						jsonBadRequest(request);
						break;
					}
				}

				if (root.containsKey("randomize"))
				{
					if (!root["randomize"].is<bool>()) {
						jsonNotFound(request);
						break;
					}
					bool randomize = root[F("randomize")];
					animator->setRandomize(randomize);
				}

				if (root.containsKey("cycle"))
				{
					if (!root["cycle"].is<int>()) {
						jsonNotFound(request);
						break;
					}
					uint8_t cycle = root[F("cycle")];
					animator->setCycle(cycle);
				}
				jsonOk(request);
				break;
			default:
				jsonNotFound(request);
				break;
		}	
	}));

	// curl -s -X POST -H "Content-Type: application/json" -d '{"randomize":true,"changeRate":15}' http://x10/api/wibblewobble | jq .
	srv->addHandler(new AsyncCallbackJsonWebHandler("/api/wibblewobble", [&](AsyncWebServerRequest *request, JsonVariant &json) {
		JsonObject& root = json.as<JsonObject>();
		switch (request->method()) {
			case HTTP_POST:
				if (!root.success()) {
					jsonBadRequest(request);
					break;
				}

				if (root.containsKey("changeRate"))
				{
					if (!root["changeRate"].is<uint16_t>()) {
						jsonBadRequest(request);
						break;
					}
					uint16_t w = root[F("changeRate")];
					wibbleWobble->changeRate(w);
				}

				if (root.containsKey("wibbleX"))
				{
					if (!root["wibbleX"].is<uint16_t>()) {
						jsonBadRequest(request);
						break;
					}
					uint16_t w = root[F("wibbleX")];
					if (w > WIBBLE_RANGE) {
						jsonBadRequest(request);
						break;
					}
					wibbleWobble->wibbleX(w);
				}

				if (root.containsKey("wibbleY"))
				{
					if (!root["wibbleY"].is<uint16_t>()) {
						jsonBadRequest(request);
						break;
					}
					uint16_t w = root[F("wibbleY")];
					if (w > WIBBLE_RANGE) {
						jsonBadRequest(request);
						break;
					}
					wibbleWobble->wibbleY(w);
				}

				if (root.containsKey("wobbleX"))
				{
					if (!root["wobbleX"].is<uint16_t>()) {
						jsonBadRequest(request);
						break;
					}
					uint16_t w = root[F("wobbleX")];
					if (w > WOBBLE_RANGE) {
						jsonBadRequest(request);
						break;
					}
					wibbleWobble->wobbleX(w);
				}

				if (root.containsKey("wobbleY"))
				{
					if (!root["wobbleY"].is<uint16_t>()) {
						jsonBadRequest(request);
						break;
					}
					uint16_t w = root[F("wobbleY")];
					if (w > WOBBLE_RANGE) {
						jsonBadRequest(request);
						break;
					}
					wibbleWobble->wobbleY(w);
				}

				if (root.containsKey("randomize"))
				{
					if (!root["randomize"].is<bool>()) {
						jsonBadRequest(request);
						break;
					}
					bool w = root[F("randomize")];
					if (w) wibbleWobble->randomize();
				}

				writeSettings();
				jsonOk(request);
				break;
			default:
				jsonNotFound(request);
				break;
		}	
	}));


	// Handle NOT FOUND and static content
	srv->onNotFound([&](AsyncWebServerRequest *request) {
		// Perhaps there's some static content we can serve?

		if (!wd.isOpen()) {
			notFound(request);
			return;
		}

		String path = request->url();

		if (path.endsWith("/"))
		{
			if (cfg.indexFile)
				path += cfg.indexFile;
			else {
				notFound(request);
				return;
			}
		}

		// Clean up the path a bit, also wrecking relative paths.
		while (path.indexOf("//") >= 0)
			path.replace("//", "/");
		while (path.indexOf("..") >= 0)
			path.replace("..", "");
		while (path.startsWith("/") || path.startsWith("."))
			path.remove(0, 1);


		int m = mimeTypeIndex(path);	
		s.print(F("Web: mime-type = "));
		s.println(mimeTypes[m].subtype);

		String fileName = cfg.webDir;
		fileName += "/";
		fileName += path;

		s.print(F("Web: fileName = "));
		s.println(fileName);

		File file;
		if (!file.open(&wd, path.c_str(), O_READ) || file.isDir()) {
			s.println(F("Web: Not found!"));
			notFound(request);
			return;
		}

		s.print(F("Web: size = "));
		s.println(file.size());
		file.close();

		request->send(mimeTypes[m].subtype, file.size(), [&, fileName](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {

			// The only way I've managed to make this work reliably is by
			// opening and closing the file in question with every chunk
			// the web server requests.
			File f;
			if (!f.open(fileName.c_str(), O_READ) || f.isDir()) {
				return maxLen;
			}
			f.seek(index);
			size_t len = f.available();
			if (len > maxLen) len = maxLen;
			size_t chunkSize = 256;
			size_t totalRead = 0;

			// For some reason reading large amounts of data in one operation causes the
			// ESP8266 to crash, so we have to resort to this chunked reading.
			do {
				size_t r = f.read(buffer + totalRead, len > chunkSize ? chunkSize : len);
				len -= r;
				totalRead += r;
			}	while (len);

			f.close();
			return totalRead;
		});

		s.println("Web: Response completed");

	});

}

int X10::mimeTypeIndex(String &path)
{
	int idx = 0;

	for (int i = 0; i < MIME_TYPES; i++)
	{
		if (path.endsWith(mimeTypes[i].suffix))
		{
			idx = i;
			break;
		}
	}

	return idx;
}

void X10::notFound(AsyncWebServerRequest *request)
{
	request->send(404, mimeTypes[plainm].subtype, "Not found.\n");
}

void X10::badRequest(AsyncWebServerRequest *request)
{
	request->send(400, mimeTypes[plainm].subtype, "Bad request.\n");
}

void X10::jsonStatus(AsyncWebServerRequest *request, int status, String title)
{
	String msg = "{\"status\":";
	msg += status;
	msg += ",\"title\":\"";
	msg += title;
	msg += "\"}\n";
	request->send(status, mimeTypes[jsonm].subtype, msg);
}

void X10::jsonBadRequest(AsyncWebServerRequest *request)
{
	jsonStatus(request, 400, "Bad request");
}

void X10::jsonNotFound(AsyncWebServerRequest *request)
{
	jsonStatus(request, 404, "Not found");
}

void X10::jsonOk(AsyncWebServerRequest *request)
{
	jsonStatus(request, 200, "OK");
}


