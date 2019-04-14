
#ifndef X10_FAKE3D_H
#define X10_FAKE3D_H

#include <X10/Effect.h>

class Vertex {
	public:
		float x, y, z;
		float tx, ty, tz; // transformed

		Vertex() {}
		Vertex(float x, float y, float z)
			: x(x), y(y), z(z) {
			reset();
		};

		void reset() {
			tx = x;
			ty = y;
			tz = z;
		}
};

class Triangle {
	public:
		uint8_t va, vb, vc;
		uint8_t r = 255, g = 255, b = 255;

		// Normal vector
		float nx, ny, nz;

		// Centroid
		float cx, cy, cz;

		Triangle(uint8_t va, uint8_t vb, uint8_t vc, uint8_t r, uint8_t g, uint8_t b )
			: va(va), vb(vb), vc(vc), r(r), g(g), b(b) {}

};

class Model {
	public:
		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;

		Model& operator+=(const Vertex& v){
					vertices.push_back(v);
					return *this;
		}

		Model& operator+=(const Triangle& t){
					triangles.push_back(t);
					return *this;
		}

		void clear() {
			vertices.clear();
			triangles.clear();
		}

		void reset() {
			for (unsigned int i = 0; i < vertices.size(); i++) {
				vertices[i].reset();
			}
		}

		void scale(float s) {
			for (unsigned int i = 0; i < vertices.size(); i++) {
				// Scale each vertex
				vertices[i].tx *= s;
				vertices[i].ty *= s;
				vertices[i].tz *= s;
			}
		}

		/*
		* | cos θ   −sin θ   0| |x|   | x cos θ − y sin θ|   |x'|
		* | sin θ    cos θ   0| |y| = | x sin θ + y cos θ| = |y'|
		* |   0       0      1| |z|   |        z         |   |z'|
		*
		* | cos θ    0   sin θ| |x|   | x cos θ + z sin θ|   |x'|
		* |   0      1       0| |y| = |         y        | = |y'|
		* |−sin θ    0   cos θ| |z|   |−x sin θ + z cos θ|   |z'|
		*
		* |1       0         0| |x|   |        x         |   |x'|
		* |0   cos θ    −sin θ| |y| = | y cos θ − z sin θ| = |y'|
		* |0   sin θ     cos θ| |z|   | y sin θ + z cos θ|   |z'|
		*/
		void rotate(float rX, float rY, float rZ) {
			float cosRX = cos(rX);
			float sinRX = sin(rX);
			float cosRY = cos(rY);
			float sinRY = sin(rY);
			float cosRZ = cos(rZ);
			float sinRZ = sin(rZ);

			for (unsigned int i = 0; i < vertices.size(); i++) {
				float x = vertices[i].tx;
				float y = vertices[i].ty;
				float z = vertices[i].tz;
				float x1, y1, z1;

				// Rotate around Z axis
				x1 = x * cosRZ - y * sinRZ;
				y1 = x * sinRZ + y * cosRZ;
				x = x1;
				y = y1;

				// Rotate around Y axis
				x1 = x * cosRY - z * sinRY;
				z1 = x * sinRY + z * cosRY;
				x = x1;
				z = z1;

				// Rotate around X axis
				y1 = y * cosRX - z * sinRX;
				z1 = y * sinRX + z * cosRX;
				y = y1;
				z = z1;

				// Store the rotated coordinates
				vertices[i].tx = x;
				vertices[i].ty = y;
				vertices[i].tz = z;
			}
		}

		void move(float dx, float dy, float dz) {
			for (unsigned int i = 0; i < vertices.size(); i++) {
				vertices[i].tx += dx;
				vertices[i].ty += dy;
				vertices[i].tz += dz;
			}
		}

		void project2D(float d) {
			for (unsigned int i = 0; i < vertices.size(); i++) {
				vertices[i].tx = vertices[i].tx * (d + vertices[i].tz) / d;
				vertices[i].ty = vertices[i].ty * (d + vertices[i].tz) / d;
			}
		}

				/*
		* Nx = Uy * Vz - Uz * Vy
		* Ny = Uz * Vx - Ux * Vz
		* Nz = Ux * Vy - Uy * Vx
		*/ 
		void calculateNormals() {

			for (unsigned int i = 0; i < triangles.size(); i++) {

				Vertex& va = vertices[triangles[i].va];
				Vertex& vb = vertices[triangles[i].vb];
				Vertex& vc = vertices[triangles[i].vc];

				// u = vb - va
				float ux = vb.tx - va.tx;
				float uy = vb.ty - va.ty;
				float uz = vb.tz - va.tz;
				// v = vc - va
				float vx = vc.tx - va.tx;
				float vy = vc.ty - va.ty;
				float vz = vc.tz - va.tz;

				triangles[i].nx = uy * vz - uz * vy;
				triangles[i].ny = uz * vx - vx * vz;
				triangles[i].nz = ux * vy - uy * vx;
			}
		}

		// This is a overly simple way to compute the center of a triangle, but
		// it's good enough for simple polygon sorting.
		void calculateCentroids() {

			for (unsigned int i = 0; i < triangles.size(); i++) {
				Vertex& va = vertices[triangles[i].va];
				Vertex& vb = vertices[triangles[i].vb];
				Vertex& vc = vertices[triangles[i].vc];

				triangles[i].cx = (va.tx + vb.tx + vc.tx) / 3; 
				triangles[i].cy = (va.ty + vb.ty + vc.ty) / 3; 
				triangles[i].cz = (va.tz + vb.tz + vc.tz) / 3; 
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
	unsigned long tick = 0;

	float xAngle = 0;
	float yAngle = 0;
	float zAngle = 0;

	void fillBottomFlatTriangle(Vertex& v1, Vertex& v2, Vertex& v3, RgbColor &col);
	void fillTopFlatTriangle(Vertex& v1, Vertex& v2, Vertex& v3, RgbColor &col);
	void drawTriangle(Vertex &v1, Vertex &v2, Vertex &v3, RgbColor &col);
};

#endif /* X10_FAKE3D_H */