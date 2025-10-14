#ifndef _SIMPLEX_NOISE_H_
#define _SIMPLEX_NOISE_H_

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#include "INoise.h"

class SimplexNoise : public INoise
{
public:
	SimplexNoise() = default;
	float Get(float x, float y, float freq) override;

private:
	static float dotProduct(const Vector& gradient, const Vector& gridPoint);
	static Vector getGradient(int hash);

	static const int permutation[256];
	static int perm[512];
	static bool initialized;
	static void initPermTable();
};

#endif