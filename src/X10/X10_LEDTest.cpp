
#include <X10/X10_LEDTest.h>

void X10_LEDTest::begin()
{
}

void X10_LEDTest::loop()
{
	unsigned long now = millis();

	if (now >= (tick + 1000))
	{
		switch (step)
		{
			default:
			case 0:
				matrix->fillScreen(matrix->Color(255, 0, 0));
				break;
			case 1:
				matrix->fillScreen(matrix->Color(0, 255, 0));
				break;
			case 2:
				matrix->fillScreen(matrix->Color(0, 0, 255));
				break;
			case 3:
				matrix->fillScreen(matrix->Color(255, 0, 255));
				break;
			case 4:
				matrix->fillScreen(matrix->Color(0, 255, 255));
				break;
			case 5:
				matrix->fillScreen(matrix->Color(255, 255, 0));
				break;
			case 6:
				matrix->fillScreen(matrix->Color(255, 255, 255));
				break;
		}
		step = (step + 1) % 7;
		tick = now;
		matrix->show();
	}
}