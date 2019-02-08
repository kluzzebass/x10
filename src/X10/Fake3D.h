
#ifndef X10_FAKE3D_H
#define X10_FAKE3D_H

#include <X10/Effect.h>

class Vertex {
	public:
		float x, y, z;
		float r, g, b;
		float tx, ty, tz;

		Vertex() {}
		Vertex(float x, float y, float z, float r, float g, float b) : x(x), y(y), z(z), r(r), g(g), b(b) {
			reset();
		};

		void reset() {
			tx = x;
			ty = y;
			tz = z;
		}
};

class Model {
	protected:
		std::vector<Vertex> vertices;
	public:

		Model& operator+=(const Vertex& vtx) {
			vertices.push_back(vtx);
			return *this;
		}

		Vertex& operator[] (const int index) {
			return vertices[index];
		}

		void clear() {
			vertices.clear();
		}

		void resetTransformations() {
			for (unsigned int i = 0; i < vertices.size(); i++) {
				vertices[i].reset();
			}
		}
};



class X10_Fake3D : public X10_Effect
{
public:
	X10_Fake3D(
		NeoPixelBusType *leds,
		Stream &s)
		: X10_Effect(leds, s) {}

	void begin();
	void init() {};
	void loop();

protected:
	int step = 0;
	unsigned long tick = 0;
};

#endif /* X10_FAKE3D_H */