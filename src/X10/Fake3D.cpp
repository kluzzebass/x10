

#include <X10/Fake3D.h>


Model blah;

void X10_Fake3D::begin()
{
	blah += Vertex( 10,  10,  10);
	blah += Vertex( 10, -10,  10);
	blah += Vertex(-10,  10,  10);
	blah += Vertex(-10, -10,  10);
	blah += Vertex( 10,  10, -10);
	blah += Vertex( 10, -10, -10);
	blah += Vertex(-10,  10, -10);
	blah += Vertex(-10, -10, -10);

	blah += Triangle(0, 1, 2, 255, 0, 0);
	blah += Triangle(0, 3, 2, 0, 255, 0);

}

void X10_Fake3D::loop()
{
	unsigned long now = millis();
	if (now < (tick + 1020)) return;
	tick = now;

	xAngle += 0.01;
	yAngle += 0.02;
	zAngle += 0.03;

	// Paint whatever was calculated the last time around. That way
	// the frame rate is kept as consistent as possible.
	leds->Show();

	unsigned long m = micros();
	blah.reset();
	blah.scale(0.4);
	blah.rotate(xAngle, yAngle, zAngle);
	blah.project2D(20);
	blah.move(8, 8, 0);
	blah.calculateNormals();
	blah.calculateCentroids();
	m = micros() - m;


	// s.print("[");
	// s.print(blah[0].tx);
	// s.print(", ");
	// s.print(blah[0].ty);
	// s.print(", ");
	// s.print(blah[0].tz);
	// s.print("]: ");
	// s.println(m);


	clear();
	// for (unsigned int i = 0; i < blah.triangles.size(); i++) {
	// 	Triangle &t = blah.triangles[i];
	// 	RgbColor col(t.r, t.g, t.b);
	// 	Vertex &va = blah.vertices[t.va];
	// 	Vertex &vb = blah.vertices[t.vb];
	// 	Vertex &vc = blah.vertices[t.vc];

	// 	// Draw only if the surface normal is pointing towards us
	// 	if (t.nz < 0) {
	// 		drawTriangle(va, vb, vc, col);
	// 	}
	// }
	// leds->SetPixelColor(xy(0, 0), RgbColor(255, 0, 0));
	// leds->SetPixelColor(xy(15, 0), RgbColor(0, 255, 0));
	// leds->SetPixelColor(xy(0, 15), RgbColor(0, 0, 255));
	// leds->SetPixelColor(xy(15, 15), RgbColor(255, 255, 0));

	// drawHorizontal(3, 3, 10, RgbColor(255, 255, 0));

	// Vertex v1 = Vertex(4, 4, 0);
	// Vertex v2 = Vertex(2, 13, 0);
	// Vertex v3 = Vertex(12, 13, 0);

	// fillBottomFlatTriangle(v1, v2, v3, RgbColor(0, 0, 255));

	// Vertex v4 = Vertex(2, 2, 0);
	// Vertex v5 = Vertex(12, 2, 0);
	// Vertex v6 = Vertex(8, 14, 0);

	// fillTopFlatTriangle(v4, v5, v6, RgbColor(0, 255, 0));

	Vertex va = Vertex(0, 0, 0);
	Vertex vb = Vertex(3, 14, 0);
	Vertex vc = Vertex(12, 5, 0);
	RgbColor col(255, 0, 0);

	drawTriangle(va, vb, vc, col);

}

void X10_Fake3D::drawTriangle(Vertex &v1, Vertex &v2, Vertex &v3, RgbColor &col) {
	// blah
}
