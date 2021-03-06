
#ifndef X10_CLOCK_H
#define X10_CLOCK_H

#include <X10/Effect.h>

#define X10_CLOCK_FONT_WIDTH 3
#define X10_CLOCK_FONT_HEIGHT 10
#define X10_CLOCK_DIGITS 10
#define X10_CLOCK_PIXEL_MASK (1 << (X10_CLOCK_FONT_WIDTH - 1))

class X10_Clock : public X10_Effect
{
public:

	X10_Clock(
		NeoPixelBusType *leds,
		Stream &s, RtcDS1307<TwoWire> &rtc)
		: X10_Effect(leds, s), rtc(rtc) {}

	void begin();
	void init() {};
	void loop();

protected:
	void drawDigit(uint digit, uint xPos, uint yPos);
	bool hiLite(uint digit, uint x, uint y);
	void drawSeconds(uint seconds);

	RtcDS1307<TwoWire> &rtc;

	uint seconds;
	uint minutesLo;
	uint minutesHi;
	uint hoursLo;
	uint hoursHi;

	unsigned long clTick = 0;
	unsigned long fxTick = 0;
	uint16_t sequencePosition = 0;

	RgbColor bg[NUM_LEDS];

	uint8_t font[10][10] = {
		{ // 0
			0b111,
			0b111,
			0b101,
			0b101,
			0b101,
			0b101,
			0b101,
			0b101,
			0b111,
			0b111
		},
		{ // 1
			0b110,
			0b110,
			0b010,
			0b010,
			0b010,
			0b010,
			0b010,
			0b010,
			0b111,
			0b111
		},
		{ // 2
			0b111,
			0b111,
			0b101,
			0b001,
			0b010,
			0b010,
			0b100,
			0b100,
			0b111,
			0b111
		},
		{ // 3
			0b111,
			0b111,
			0b101,
			0b001,
			0b010,
			0b010,
			0b001,
			0b101,
			0b111,
			0b111
		},
		{ // 4
			0b100,
			0b101,
			0b101,
			0b101,
			0b111,
			0b111,
			0b001,
			0b001,
			0b001,
			0b001
		},
		{ // 5
			0b111,
			0b111,
			0b100,
			0b100,
			0b111,
			0b111,
			0b001,
			0b101,
			0b111,
			0b111
		},
		{ // 6
			0b111,
			0b111,
			0b100,
			0b100,
			0b111,
			0b111,
			0b101,
			0b101,
			0b111,
			0b111
		},
		{ // 7
			0b111,
			0b111,
			0b001,
			0b001,
			0b011,
			0b011,
			0b001,
			0b001,
			0b001,
			0b001
		},
		{ // 8
			0b111,
			0b111,
			0b101,
			0b101,
			0b111,
			0b111,
			0b101,
			0b101,
			0b111,
			0b111
		},
		{ // 9
			0b111,
			0b111,
			0b101,
			0b101,
			0b111,
			0b111,
			0b001,
			0b101,
			0b111,
			0b111
		}
	};

	uint8_t secondHand[60][2] = {
		{ 0x70, 0x80 },
		{ 0x80, 0x90 },
		{ 0x90, 0xa0 },
		{ 0xa0, 0xb0 },
		{ 0xb0, 0xc0 },
		{ 0xc0, 0xd0 },
		{ 0xd0, 0xe0 },
		{ 0xe0, 0xf0 },
		{ 0xf0, 0xf1 },
		{ 0xf1, 0xf2 },

		{ 0xf2, 0xf3 },
		{ 0xf3, 0xf4 },
		{ 0xf4, 0xf5 },
		{ 0xf5, 0xf6 },
		{ 0xf6, 0xf7 },
		{ 0xf7, 0xf8 },
		{ 0xf8, 0xf9 },
		{ 0xf9, 0xfa },
		{ 0xfa, 0xfb },
		{ 0xfb, 0xfc },

		{ 0xfc, 0xfd },
		{ 0xfd, 0xfe },
		{ 0xfe, 0xff },
		{ 0xff, 0xef },
		{ 0xef, 0xdf },
		{ 0xdf, 0xcf },
		{ 0xcf, 0xbf },
		{ 0xbf, 0xaf },
		{ 0xaf, 0x9f },
		{ 0x9f, 0x8f },

		{ 0x8f, 0x7f },
		{ 0x7f, 0x6f },
		{ 0x6f, 0x5f },
		{ 0x5f, 0x4f },
		{ 0x4f, 0x3f },
		{ 0x3f, 0x2f },
		{ 0x2f, 0x1f },
		{ 0x1f, 0x0f },
		{ 0x0f, 0x0e },
		{ 0x0e, 0x0d },

		{ 0x0d, 0x0c },
		{ 0x0c, 0x0b },
		{ 0x0b, 0x0a },
		{ 0x0a, 0x09 },
		{ 0x09, 0x08 },
		{ 0x08, 0x07 },
		{ 0x07, 0x06 },
		{ 0x06, 0x05 },
		{ 0x05, 0x04 },
		{ 0x04, 0x03 },

		{ 0x03, 0x02 },
		{ 0x02, 0x01 },
		{ 0x01, 0x00 },
		{ 0x00, 0x10 },
		{ 0x10, 0x20 },
		{ 0x20, 0x30 },
		{ 0x30, 0x40 },
		{ 0x40, 0x50 },
		{ 0x50, 0x60 },
		{ 0x60, 0x70 }
	};

};

#endif /* X10_CLOCK_H */
