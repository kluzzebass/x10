
#include <X10/Animator.h>

void X10_Animator::begin()
{
	animationCount = 0;
	animDir = animDir ? animDir : ANIM_DIR;
	animCfgFile = animCfgFile ? animCfgFile : ANIM_CFG_FILE;

	if (!d.open(animDir))
	{
		s.print(F("Unable to open animation directory: "));
		s.println(animDir);
		return;
	}

	s.println(F("Successfully opened animation directory."));

	while (File f = d.openNextFile())
	{
		animationCount++;
	}

	d.rewindDirectory();

	s.print("Number of files: ");
	s.println(animationCount);

	// Force animation election
	nextAnimationPlz = true;
}

void X10_Animator::loop()
{
	unsigned long now = millis();

	bool timeHasCome = (now >= (animationTick + cycleTimes[cycle] * 1000));
	bool okayToChange = (!forceFinish || (forceFinish && finished));

	if (nextAnimationPlz || (timeHasCome && okayToChange))
	{
		if (nextAnimation())
		{
			// We've succeeded in finding a new animation.
			nextAnimationPlz = false;
			animationTick = now;
		}
	}
	else
	{
		// It is okay to progress the current animation
		animate();
	}
}

const uint16_t *X10_Animator::getCycleTimes()
{
	return cycleTimes;
}

uint8_t X10_Animator::getCycle()
{
	return cycle;
}

void X10_Animator::setCycle(uint8_t c)
{
	if (c < (sizeof(cycleTimes) / sizeof(cycleTimes[0])))
		cycle = c;
}

bool X10_Animator::getRandomize()
{
	return randomize;
}

void X10_Animator::setRandomize(bool r)
{
	randomize = r;
}

void X10_Animator::next()
{
	nextAnimationPlz = true;
}

bool X10_Animator::next(const char *anim)
{
	// Keep track of the nested status so we don't ruin the animation
	// sequence even if initAnimation() fails.
	bool oldNested = nested;
	nested = false;
	if (!initAnimation(anim))
	{
		nested = oldNested;
		return false;
	}
	return true;
}

const char *X10_Animator::currentAnim()
{
	return nested ? nestName : animName;
}


bool X10_Animator::nextAnimation()
{
	File f;
	char buffer[BUFFER_LEN + 1];

	if (!nested)
	{
		// Animation chain?
		if (nextFolder[0])
			return initAnimation(nextFolder);

		if (randomize)
		{
			for (int i = 0; i < random(animationCount); i++)
			{
				// Keep opening and rewinding
				if (!(f = d.openNextFile())) d.rewindDirectory();
			}
		}
		else
		{
			if (!(f = d.openNextFile()))
			{
				// That didn't work. Rewind, and try different animation.
				d.rewindDirectory();
				return false;
			}
		}

		f.getName(buffer, BUFFER_LEN);
	}
	else
	{
		snprintf(buffer, BUFFER_LEN, "%s/%d", nestName, ++currentNest);
		s.print(F("Trying to locate next nest dweller: "));
		s.println(buffer);
		if (!d.exists(buffer)) {
			// We've reached the end of the nest
			s.println(F("Did not find the next dweller."));
			nested = false;
			nestName[0] = 0;
			nextAnimationPlz = true;
			return false;
		}
		else
		{
			s.println(F("Next nested animation."));
		}
	}
	return initAnimation(buffer);
}

void X10_Animator::animate()
{
	char path[BUFFER_LEN + 1];
	
	unsigned long now = millis();

	// Time for a new frame?
	if (now < (frameTick + animationHold))
		return;

	finished = false;

	if (!singleBitmap)
	{
		snprintf(path, BUFFER_LEN, "%s/%d.bmp", animationPath, currentFrame++);

		if (!bmp.open(path))
		{
			// We've probably reached the end of an animation
			if (animationLoop) {
				finished = true;
				currentFrame = 0;
			}
			else
			{
				// Request next animation
				s.println(F("Time for the next animation."));
				nextAnimationPlz = true;
				frameTick = now;
			}
			
			return;
		}
		else
		{
			updatePanningLimits();
		}
	}

	clear();
	drawFrame(bmp, offsetX, offsetY);

	// Update offsets
	offsetX += offsetSpeedX;
	if (offsetSpeedX < 0 && offsetX < minX) {
		if (translationLoop) {
			offsetX = maxX;
		} else {
			nextAnimationPlz = true;
		}
		finished = true;
	}
	if (offsetSpeedX > 0 && offsetX > maxX) {
		if (translationLoop) {
			offsetX = minX;
		} else {
			nextAnimationPlz = true;
		}
		finished = true;
	}

	offsetY -= offsetSpeedY;
	if (offsetSpeedY > 0 && offsetY < minY) {
		if (translationLoop) {
			offsetY = maxY;
		} else {
			nextAnimationPlz = true;
		}
		finished = true;
	}
	if (offsetSpeedY < 0 && offsetY > maxY) {
		if (translationLoop) {
			offsetY = minY;
		} else {
			nextAnimationPlz = true;
		}
		finished = true;
	}

	leds->Show();

	frameTick = now;

}

bool X10_Animator::initAnimation(const char *anim)
{

	File f;
	char path[PATH_MAX + 1];

	snprintf(path, PATH_MAX, "%s/%s", animDir, anim);

	// Maybe it is a system thing, and should be skipped?
	if (anim[0] == '0' && anim[1] == '0')
	{
		s.println(F("Skipping system directory."));
		return false;
	}

	// If this file even a thing
	if (!f.open(path))
	{
		s.print(F("Unable to open animation path: "));
		s.println(path);
		return false;
	}

	// Is this a directory
	if (!f.isDir())
	{
		s.print(F("Animation path is not a directory: "));
		s.println(path);
		return false;
	}

	// Is this a nested animation dir?
	if (f.exists("0/0.bmp"))
	{
		s.print(F("Animation path is nested: "));
		s.println(path);
		nested = true;
		currentNest = 0;
		nextAnimationPlz = false;
		strncpy(nestName, anim, BUFFER_LEN);
		char buf[BUFFER_LEN + 1];
		snprintf(buf, BUFFER_LEN, "%s/0", anim);

		// This will initiate a search for animations within the nest
		return initAnimation(buf);
	}

	// Can we find the first animation frame in the directory?
	if (!f.exists("0.bmp"))
	{
		s.print(F("No animation file found directory: "));
		s.println(path);
		return false;
	}

	// Does the animation consist of a single bitmap?
	singleBitmap = !f.exists("1.bmp");

	// We're now reasonably certain that this is an animation
	// directory, so it's time to rig up the defaults
	strncpy(animationPath, path, PATH_MAX);
	strncpy(animName, anim, BUFFER_LEN);
	frameTick = millis();
	currentFrame = 0;
	animationHold = ANIM_HOLD_TIME;
	animationLoop = true;
	forceFinish = false;
	finished = false;
	offsetX = 0;
	offsetY = 0;
	offsetSpeedX = 0;
	offsetSpeedY = 0;
	translationLoop = false;
	panOff = true;
	nextFolder[0] = 0;
	nextAnimationPlz = false;

	s.print(F("Loading animation: "));
	s.println(path);
	
	loadAnimationCfg(path);

	s.print(F("Single bitmap animation: "));
	s.println(singleBitmap ? F("true") : F("false"));
	s.print(F("Animation hold time: "));
	s.println(animationHold);
	s.print(F("Animation loop: "));
	s.println(animationLoop ? F("true") : F("false"));
	s.print(F("Animation force finish: "));
	s.println(forceFinish ? F("true") : F("false"));
	s.print(F("Offset Speed X: "));
	s.println(offsetSpeedX);
	s.print(F("Offset Speed Y: "));
	s.println(offsetSpeedY);
	s.print(F("Translation loop: "));
	s.println(translationLoop ? F("true") : F("false"));
	s.print(F("Start movement off screen: "));
	s.println(panOff ? F("true") : F("false"));
	s.print(F("Next animation folder: "));
	s.println(nextFolder);

	// If it's a single bitmap, we might as well try to open the file.
	if (singleBitmap && !bmp.open("0.bmp"))
	{
		nextAnimationPlz = true;
		return false;
	}

	if (singleBitmap)
	{
		// Ignore forceFinish for multi-directional moving single bitmaps; it's
		// just too complicated
		if (offsetSpeedX != 0 && offsetSpeedY != 0) forceFinish = false;
		updatePanningLimits();
	}

	// Set the movement starting position.
	if (offsetSpeedX < 0) {
		offsetX = maxX;
	}

	if (offsetSpeedX > 0) {
		offsetX = minX;
	}

	if (offsetSpeedY < 0) {
		offsetY = maxY;
	}

	if (offsetSpeedY > 0) {
		offsetY = minY;
	}

	return true;
}

void X10_Animator::updatePanningLimits()
{
	if (panOff)
	{
		minX = -bmp.width();
		maxX = WIDTH;
		minY = -bmp.height();
		maxY = HEIGHT;
	}
	else
	{
		minX = -bmp.width() + WIDTH;
		maxX = 0;
		minY = -bmp.height() + HEIGHT;
		maxY = 0;
	}

}

void X10_Animator::loadAnimationCfg(const char *path)
{
	char buffer[BUFFER_LEN + 1];
	const char *animation = "animation";
	const char *translate = "translate";

	SD.chdir(path);

	IniFile ini(animCfgFile);

	if (!ini.open())
	{
		s.println(F("No config file found."));
		return;
	}
	
	if (!ini.validate(buffer, BUFFER_LEN))
	{
		s.println(F("Validation failed."));
		return;
	}

	s.println(F("Loading configuration."));

	ini.getValue(animation, "hold", buffer, BUFFER_LEN, animationHold);
	ini.getValue(animation, "loop", buffer, BUFFER_LEN, animationLoop);
	ini.getValue(animation, "finish", buffer, BUFFER_LEN, forceFinish);
	ini.getValue(translate, "moveX", buffer, BUFFER_LEN, offsetSpeedX);
	ini.getValue(translate, "moveY", buffer, BUFFER_LEN, offsetSpeedY);
	ini.getValue(translate, "loop", buffer, BUFFER_LEN, translationLoop);
	ini.getValue(translate, "panoff", buffer, BUFFER_LEN, panOff);
	if (ini.getValue(translate, "nextfolder", buffer, BUFFER_LEN))
		strncpy(buffer, nextFolder, BUFFER_LEN);
}