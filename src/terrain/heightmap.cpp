#include "terrain/heightmap.h"
#include <algorithm>

HeightMap::HeightMap(int width, int depth) : width(width), depth(depth), data(width * depth) { }

float HeightMap::sample(float x, float z) const
{
	x = std::clamp(x, 0.0f, static_cast<float>(width - 2));
	z = std::clamp(z, 0.0f, static_cast<float>(depth - 2));

	int ix = static_cast<int>(x);
	int iz = static_cast<int>(z);

	float fx = x - ix;
	float fz = z - iz;

	int ix1 = ix + 1;
	int iz1 = iz + 1;

	float h00 = data[iz * width + ix];
	float h10 = data[iz * width + ix1];
	float h01 = data[iz1 * width + ix];
	float h11 = data[iz1 * width + ix1];

	return h00 * (1 - fx) * (1 - fz) + h10 * fx * (1 - fz) + h01 * (1 - fx) * fz + h11 * fx * fz;
}

float HeightMap::addBilinear(int x, int z, float fx, float fz, float amount)
{
	float w00 = (1 - fx) * (1 - fz);
	float w10 = fx * (1 - fz);
	float w01 = (1 - fx) * fz;
	float w11 = fx * fz;

	data[z * width + x] += amount * w00;
	data[z * width + (x + 1)] += amount * w10;
	data[(z + 1) * width + x] += amount * w01;
	data[(z + 1) * width + (x + 1)] += amount * w11;

	return amount;
}

float& HeightMap::getHeight(int x, int z)
{
	return data[z * width + x];
}

int HeightMap::getWidth()
{
	return this->width;
}

int HeightMap::getDepth()
{
	return this->depth;
}