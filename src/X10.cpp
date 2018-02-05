
#include <X10.h>

SdFat SD;

void X10::begin()
{
	s.println(F("\n\nStarting X10 initialization."));

	// Insert some hardware info here

	// Seed the random generator
	unsigned long seed = 0;
	for (int i = 0; i < sizeof(seed * 8); i++)
	{
		seed <<= 1;
		seed |= analogRead(0) & 1;
	}
	randomSeed(seed);

	EEPROM.begin(sizeof(persist_t));
	readFromEEPROM();

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
	FastLED.show();

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
	srv->handleClient();
}


void X10::bootStatus(int x, uint8_t r, uint8_t g, uint8_t b)
{
	leds[xy(x, 0)] = CRGB(r, g, b);
	FastLED.show();
}


void X10::writeToEEPROM()
{
	persist_t p;
	memset(&p, 0, sizeof(persist_t));

	s.println(F("EEPROM: Writing..."));

	// Populate the persist structure
	p.effect = currentEffect;
	p.brightness = FastLED.getBrightness();

	// Write to eeprom
	EEPROM.put(0, p);

	EEPROM.commit();

	s.println(F("EEPROM: Done."));
}

void X10::readFromEEPROM()
{
	persist_t p;
	memset(&p, 0, sizeof(persist_t));

	// Read from eeprom
	s.println(F("EEPROM: Reading..."));
	EEPROM.get(0, p);

	s.print("EEPROM: checksum = 0x");
	s.println(p.checksum, HEX);
	s.print("EEPROM: effect = ");
	s.println(p.effect);
	s.print("EEPROM: brightness = ");
	s.println(p.brightness);

	// Spread persisted data
	switchEffect(p.effect);
	setBrightness(p.brightness);

	s.println(F("EEPROM: Done."));
}


void X10::beginMatrix(int x)
{
	s.println(F("Matrix: Initializing..."));

	FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
	FastLED.setCorrection(cfg.colorCorrection);
	FastLED.setTemperature(cfg.colorTemperature);

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
	jsonm = mimeTypeIndex(jsons);

	// Gotta love those lambda functions.
	srv->onNotFound([&]() {
		if (srv->uri().startsWith("/api/")) {
			String path = srv->uri().substring(5);
			handleApi(path);
		}
		else handleStaticContent();
	});

	if (cfg.webDir != NULL)
	{
		wd = SD.open(cfg.webDir);

		if (wd.isOpen() && !wd.isDir())
		{
			wd.close();
		}
	}

	srv->begin();

	s.println(F("Web: Done."));
	bootStatus(x, 0, 100, 0);
}

bool X10::setBrightness(uint8_t brightness)
{
	if (brightness < cfg.minBrightness || brightness > cfg.maxBrightness)
		return false;

	s.print(F("Setting brightness to: "));
	s.println(brightness);
	
	FastLED.setBrightness(brightness);
	return true;
}

bool X10::switchEffect(uint8_t effect)
{
	if (effect >= EFFECTS) return false;

	s.print(F("Switching to effect: "));
	s.println(effect);

	currentEffect = effect;
	initializePlz = true;

	writeToEEPROM();

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
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */


void X10::notFound()
{
	srv->send(404, "text/plain", "Not found.\n");
}

void X10::badRequest()
{
	srv->send(400, "text/plain", "Bad request.\n");
}

void X10::handleStaticContent()
{
	if (!wd.isOpen())
		return notFound();

	String path = srv->uri();

	if (path.endsWith("/"))
	{
		if (cfg.indexFile)
			path += cfg.indexFile;
		else
			return notFound();
	}

	// Clean up the path a bit, also wrecking relative paths.
	while (path.indexOf("//") >= 0)
		path.replace("//", "/");
	while (path.indexOf("..") >= 0)
		path.replace("..", "");
	while (path.startsWith("/") || path.startsWith("."))
		path.remove(0, 1);

	s.print(F("Web: path = "));
	s.println(path);

	File f;
	if (!f.open(&wd, path.c_str(), O_READ) || f.isDir())
		return notFound();

	int m = mimeTypeIndex(path);	
	s.println("Web: mime-type = " + mimeTypes[m].subtype);

	// All our ducks are in a row; time to send the file.
	size_t sent = srv->streamFile(f, mimeTypes[m].subtype);
	f.close();

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

void X10::handleApi(String &path)
{
	if (!path.length())
		return badRequest();

	srv->sendHeader("Access-Control-Allow-Origin", "*", false);
	srv->sendHeader("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE,OPTIONS", false);
	srv->sendHeader("Access-Control-Allow-Headers", "Content-Type, Access-Control-Allow-Headers, Authorization, X-Requested-With", false);

	switch (srv->method())
	{
		case HTTP_GET:
			if (path.equals("display")) return hGetDisplay();
			if (path.equals("effect")) return hGetEffect();
			if (path.equals("datetime")) return hGetDateTime();
			if (path.equals("animator")) return hGetAnimator();
			if (path.equals("wibblewobble")) return hGetWibbleWobble();
			break;
		case HTTP_POST:
			if (path.equals("display")) return hPostDisplay();
			if (path.equals("effect")) return hPostEffect();
			if (path.equals("datetime")) return hPostDateTime();
			if (path.equals("animator")) return hPostAnimator();
			if (path.equals("wibblewobble")) return hPostWibbleWobble();
			break;
		case HTTP_OPTIONS:
			jsonOk();
			break;
		default:
			return;
	}

	return jsonNotFound();
}

void X10::jsonStatus(int status, String title)
{
	String msg = "{\"status\":";
	msg += status;
	msg += ",\"title\":\"";
	msg += title;
	msg += "\"}\n";
	srv->send(status, mimeTypes[jsonm].subtype, msg);
}

void X10::jsonBadRequest()
{
	jsonStatus(400, "Bad request");
}

void X10::jsonNotFound()
{
	jsonStatus(404, "Not found");
}

void X10::jsonOk()
{
	jsonStatus(200, "OK");
}


// curl -s http://x10/api/display | jq .

void X10::hGetDisplay()
{
	String msg;

	msg += "{\"brightness\":";
	msg += FastLED.getBrightness();
	msg += ",\"maxBrightness\":";
	msg += cfg.maxBrightness;
	msg += ",\"minBrightness\":";
	msg += cfg.minBrightness;
	msg += "}\n";

	srv->send(200, mimeTypes[jsonm].subtype, msg);
}

// curl -s -X POST -H "Content-Type: application/json" -d '{"brightness":50}' http://x10/api/display | jq .

void X10::hPostDisplay()
{
	StaticJsonBuffer<BUFFER_LEN> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(srv->arg("plain"));

	if (!root.success()) return jsonBadRequest();
	int brightness = root[F("brightness")];

	if (!setBrightness(brightness)) return jsonBadRequest();

	writeToEEPROM();

	jsonOk();
}




// curl -s http://x10/api/effect | jq .

void X10::hGetEffect()
{
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

	srv->send(200, mimeTypes[jsonm].subtype, msg);
}

// curl -s -X POST -H "Content-Type: application/json" -d '{"current":1}' http://x10/api/effect | jq .

void X10::hPostEffect()
{
	StaticJsonBuffer<BUFFER_LEN> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(srv->arg("plain"));

	if (!root.success()) return jsonBadRequest();
	int effect = root[F("current")];

	if (!switchEffect(effect)) return jsonBadRequest();

	writeToEEPROM();

	jsonOk();
}





// curl -s http://x10/api/datetime | jq .

void X10::hGetDateTime()
{
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

	srv->send(200, mimeTypes[jsonm].subtype, msg);
}

// curl -s -X POST -H "Content-Type: application/json" -d '{"date":"Nov 25 2017","time":"18:28:45"}' http://x10/api/datetime | jq .

void X10::hPostDateTime()
{
	StaticJsonBuffer<BUFFER_LEN> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(srv->arg("plain"));

	if (!root.success()) return jsonBadRequest();

	if (!root["date"].is<const char*>())
		return jsonBadRequest();
	if (!root["time"].is<const char*>())
		return jsonBadRequest();

	String d = root[F("date")];
	String t = root[F("time")];

	RtcDateTime compiled = RtcDateTime(d.c_str(), t.c_str());
	rtc.SetDateTime(compiled);

	jsonOk();
}





// curl -s http://x10/api/animator | jq .

void X10::hGetAnimator()
{
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

	srv->send(200, mimeTypes[jsonm].subtype, msg);
}

// curl -s -X POST -H "Content-Type: application/json" -d '{"randomize":true}' http://x10/api/animator | jq .
// curl -s -X POST -H "Content-Type: application/json" -d '{"cycle":1}' http://x10/api/animator | jq .
// curl -s -X POST -H "Content-Type: application/json" -d '{"next":true}' http://x10/api/animator | jq .
// curl -s -X POST -H "Content-Type: application/json" -d '{"next":""}' http://x10/api/animator | jq .

void X10::hPostAnimator()
{
	StaticJsonBuffer<BUFFER_LEN> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(srv->arg("plain"));

	if (!root.success()) return jsonBadRequest();

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
			if (!animator->next(next))
				return jsonNotFound();
		}
		else
			return jsonBadRequest();
	}

	if (root.containsKey("randomize"))
	{
		if (!root["randomize"].is<bool>())
			return jsonBadRequest();
		bool randomize = root[F("randomize")];
		animator->setRandomize(randomize);
	}

	if (root.containsKey("cycle"))
	{
		if (!root["cycle"].is<int>())
			return jsonBadRequest();
		uint8_t cycle = root[F("cycle")];
		animator->setCycle(cycle);
	}

	jsonOk();
}










// curl -s http://x10/api/wibblewobble | jq .

void X10::hGetWibbleWobble()
{
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

	srv->send(200, mimeTypes[jsonm].subtype, msg);
}

// curl -s -X POST -H "Content-Type: application/json" -d '{"randomize":true,"changeRate":15}' http://x10/api/wibblewobble | jq .

void X10::hPostWibbleWobble()
{
	StaticJsonBuffer<BUFFER_LEN> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(srv->arg("plain"));

	if (!root.success()) {
		Serial.println("Unable to parse JSON:");
		Serial.println(srv->arg("plain"));
		return jsonBadRequest();
	}

	if (root.containsKey("changeRate"))
	{
		if (!root["changeRate"].is<uint16_t>())
			return jsonBadRequest();
		uint16_t w = root[F("changeRate")];
		wibbleWobble->changeRate(w);
	}

	if (root.containsKey("wibbleX"))
	{
		if (!root["wibbleX"].is<uint16_t>())
			return jsonBadRequest();
		uint16_t w = root[F("wibbleX")];
		if (w > WIBBLE_RANGE)
			return jsonBadRequest();
		wibbleWobble->wibbleX(w);
	}

	if (root.containsKey("wibbleY"))
	{
		if (!root["wibbleY"].is<uint16_t>())
			return jsonBadRequest();
		uint16_t w = root[F("wibbleY")];
		if (w > WIBBLE_RANGE)
			return jsonBadRequest();
		wibbleWobble->wibbleY(w);
	}

	if (root.containsKey("wobbleX"))
	{
		if (!root["wobbleX"].is<uint16_t>())
			return jsonBadRequest();
		uint16_t w = root[F("wobbleX")];
		if (w > WOBBLE_RANGE)
			return jsonBadRequest();
		wibbleWobble->wobbleX(w);
	}

	if (root.containsKey("wobbleY"))
	{
		if (!root["wobbleY"].is<uint16_t>())
			return jsonBadRequest();
		uint16_t w = root[F("wobbleY")];
		if (w > WOBBLE_RANGE)
			return jsonBadRequest();
		wibbleWobble->wobbleY(w);
	}

	if (root.containsKey("randomize"))
	{
		if (!root["randomize"].is<bool>())
			return jsonBadRequest();
		bool w = root[F("randomize")];
		if (w) wibbleWobble->randomize();
	}

	jsonOk();
}
