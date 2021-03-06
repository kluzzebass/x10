
#include <X10/Effect.h>

// extern SdFat SD;

class X10_Animator : public X10_Effect
{
public:
	X10_Animator(
		NeoPixelBusType *leds,
		Stream &s, const char *animDir, const char *animCfgFile, SdFat &sd)
		: X10_Effect(leds, s),
			animDir(animDir),
			animCfgFile(animCfgFile),
			bmp(sd) {}

	void begin();
	void init() {};
	void loop();

	const uint16_t *getCycleTimes();
	uint8_t getCycle();
	uint8_t getCycleTimesCount() { return (sizeof(cycleTimes) / sizeof(uint16_t)); };
	void setCycle(uint8_t);
	bool getRandomize();
	void setRandomize(bool r);
	void next();
	bool next(const char *anim);
	const char *currentAnim();

protected:

	// Here there be animations
	const char *animDir;

	// Here there be configs
	const char *animCfgFile;

	// Current open working directory
	File d;

	// Bitmap reader
	BMPReader bmp;

	// Keep track of the previous animation time
	uint32_t animationTick = 0;

	// Various animation duration levels
	#define CYCLE_TIMES 7
	const uint16_t cycleTimes[CYCLE_TIMES] = { 10, 30, 60, 300, 900, 1800, 3600 };

	// Currently chosen cycle time
	uint8_t cycle = 0;

	// Should we step randomly between top level animations?
	bool randomize = false; 

	// Number of top level animation dirs in the SD card
	uint16_t animationCount;

	// Animation mame
	char animName[BUFFER_LEN + 1];

	// Top level animation folder (could be the top of a nested set of animations)
	char nestName[BUFFER_LEN + 1];

	// Top level or current nested level folder. 
	char animationPath[PATH_MAX + 1];

	// Is this a nested animation folder?
	bool nested;

	// Is this a single bitmap animation?
	bool singleBitmap;

	// Which nest are we currently playing?
	uint16_t currentNest;

	// Keep track of when the current frame was started
	uint32_t frameTick;

	// Which animation frame are we currently showing?
	uint16_t currentFrame;

	// How long should each frame be held?
	uint16_t animationHold;

	// Should the animation loop?
	bool animationLoop;

	// Force finish playing animation, even if alotted time has elapsed.
	bool forceFinish;

	// Did the animation run to completion?
	bool finished;

	// How much to move the animation per frame?
	int offsetSpeedX, offsetSpeedY;
	int16_t offsetX, offsetY;
	int16_t minX, maxX, minY, maxY;

	// Should the translations loop?
	bool translationLoop;

	// Begin and end movement off screen?
	bool panOff;

	// Next folder in a chain
	char nextFolder[BUFFER_LEN + 1];
	
	// This flag is set when animate() detects that an animation is finished.
	bool nextAnimationPlz;

	bool nextAnimation();
	bool initAnimation(const char *anim);
	void updatePanningLimits();
	void loadAnimationCfg(const char *cfgpath);
	void animate();
};