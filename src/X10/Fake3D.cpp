

#include <X10/Fake3D.h>


Model blah;

void X10_Fake3D::begin()
{
	blah += Vertex( 10,  10,  10, 1, 1, 1);
	blah += Vertex( 10, -10,  10, 1, 1, 1);
	blah += Vertex(-10,  10,  10, 1, 1, 1);
	blah += Vertex(-10, -10,  10, 1, 1, 1);
	blah += Vertex( 10,  10, -10, 1, 1, 1);
	blah += Vertex( 10, -10, -10, 1, 1, 1);
	blah += Vertex(-10,  10, -10, 1, 1, 1);
	blah += Vertex(-10, -10, -10, 1, 1, 1);

}

void X10_Fake3D::loop()
{
	clear();
	leds->SetPixelColor(xy(0, 0), RgbColor(255, 0, 0));
	leds->SetPixelColor(xy(15, 0), RgbColor(0, 255, 0));
	leds->SetPixelColor(xy(0, 15), RgbColor(0, 0, 255));
	leds->SetPixelColor(xy(15, 15), RgbColor(255, 255, 0));
	leds->Show();
}
