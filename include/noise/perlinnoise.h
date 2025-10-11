#ifndef _PERLIN_NOISE_H_
#define _PERLIN_NOISE_H_

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <iostream>
#include <cmath>

struct Vector
{
    float x;
    float y;

    Vector(float x, float y) : x(x), y(y) {}
    Vector() : x(0.0f), y(0.0f) {}
};

class PerlinNoise
{
public:
    static float perlin2D(float x, float y, float scale);

private:
    static float lerp(float a, float b, float t);
    static float fade(float t);
    static float dotProduct(const Vector& gradient, const Vector& gridPoint);
    static Vector getGradient(int x, int y);
    static Vector getVectorDistance(float xf, float x, float yf, float y);
    static int hash(int x, int y);

    static const int permutation[256];
    static int perm[512];
    static bool initialized;
    static void initPermTable();
};

#endif
