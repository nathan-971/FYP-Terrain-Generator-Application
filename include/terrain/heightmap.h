#ifndef _HEIGHTMAP_H_
#define _HEIGHTMAP_H_

#include <vector>

class HeightMap
{
public:
	HeightMap(int width, int depth);
	float& getHeight(int x, int z);
	float sample(float x, float z) const;
	float addBilinear(int x, int z, float fx, float fz, float amount);

	int getWidth();
	int getDepth();

private:
	int width;
	int depth;
	std::vector<float> data;
};

#endif