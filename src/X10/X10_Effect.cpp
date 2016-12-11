
#include <X10/X10_Effect.h>

/*
 * Cyclic color sequencer without white
 *
 * Red		255,	0,		0
 * Yellow	255,	255,	0
 * Green	0,		255,	0
 * Cyan     0,		255,	255
 * Blue		0,		0,		255
 * Magenta	255,	0,		255
 * White	255,	255,	255
 */

int16_t X10_Effect::colorSequence(int seqPos, bool includeWhite)
{
	int colorsPerSegment = 255;
	int totalSegments = includeWhite ? 7 : 6;
	seqPos %= colorsPerSegment * totalSegments;
	int segment = seqPos / colorsPerSegment;
	int segmentColor = seqPos % colorsPerSegment;

	// Red -> Yellow
	if (segment == 0) return matrix->Color(255, segmentColor, 0);

	// Yellow -> Green
	if (segment == 1) return matrix->Color(255 - segmentColor, 255, 0);

	// Green -> Cyan
	if (segment == 2) return matrix->Color(0, 255, segmentColor);

	// Cyan -> Blue
	if (segment == 3) return matrix->Color(0, 255 - segmentColor, 255);
	
	// Blue -> Magenta
	if (segment == 4) return matrix->Color(segmentColor, 0, 255);

	if (includeWhite)
	{
		// Magenta -> White
		if (segment == 5) return matrix->Color(255, segmentColor, 255);

		// White -> Red
		return matrix->Color(255, 255 - segmentColor, 255 - segmentColor);
	}

	// Magenta -> Red
	return matrix->Color(255, 0, 255 - segmentColor);
}

