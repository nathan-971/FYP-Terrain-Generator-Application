#ifndef _PERLIN_NOISE_H_
#define _PERLIN_NOISE_H_

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "terrain/noise/inoise.h"

#include <cmath>

class PerlinNoise : public INoise
{
public:
    PerlinNoise() = default;
    float Get(float x, float y) override;
    void ApplySeed(int seed) override;

private:
    float lerp(float a, float b, float t);
    float fade(float t);
    float dotProduct(const Vector& gradient, const Vector& gridPoint);
    Vector getGradient(int x, int y);
    Vector getVectorDistance(float xf, float x, float yf, float y);
    int hash(int x, int y);
    void initPermTable(int seed);

    static int perm[512];
    int seed;
    static bool initialized;
};

#endif
