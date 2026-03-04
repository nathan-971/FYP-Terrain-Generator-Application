#include "terrain/noise/perlinnoise.h"

#include <random>
#include <algorithm>
#include <numeric>

int PerlinNoise::perm[512];
bool PerlinNoise::initialized = false;

void PerlinNoise::initPermTable(int seed)
{
    if (initialized)
    {
        return;
    }

    int p[256];
    std::iota(p, p + 256, 0);

    std::mt19937 rng(seed);
    std::shuffle(p, p + 256, rng);

    for (int i = 0; i < 256; i++)
    {
        perm[i] = p[i];
        perm[i + 256] = p[i];
    }

    initialized = true;
}

void PerlinNoise::ApplySeed(int seed)
{
    initialized = false;
    this->seed = seed;
    initPermTable(this->seed);
}

float PerlinNoise::Get(float x, float y)
{
    float xf = x;
    float yf = y;

    int x0 = static_cast<int>(floor(xf));
    int y0 = static_cast<int>(floor(yf));
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    Vector g00 = getGradient(x0, y0);
    Vector g10 = getGradient(x1, y0);
    Vector g01 = getGradient(x0, y1);
    Vector g11 = getGradient(x1, y1);

    Vector d00(xf - x0, yf - y0);
    Vector d10(xf - x1, yf - y0);
    Vector d01(xf - x0, yf - y1);
    Vector d11(xf - x1, yf - y1);

    float s = dotProduct(g00, d00);
    float t = dotProduct(g10, d10);
    float u = dotProduct(g01, d01);
    float v = dotProduct(g11, d11);

    float fadeX = fade(xf - x0);
    float fadeY = fade(yf - y0);

    float lerpX1 = lerp(s, t, fadeX);
    float lerpX2 = lerp(u, v, fadeX);
	float result = lerp(lerpX1, lerpX2, fadeY);

    return result * 1.41421356f;
}

float PerlinNoise::lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

float PerlinNoise::fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::dotProduct(const Vector& gradient, const Vector& gridPoint)
{
    return (gradient.x * gridPoint.x) + (gradient.y * gridPoint.y);
}

Vector PerlinNoise::getGradient(int x, int y)
{
    int h = hash(x, y) & 255;
    float angle = (h / 255.0f) * 6.28318f;
    return Vector(cos(angle), sin(angle));
}

int PerlinNoise::hash(int x, int y)
{
    return perm[(perm[x & 255] + perm[y & 255]) & 255];
}

Vector PerlinNoise::getVectorDistance(float xf, float x, float yf, float y)
{
    return Vector(xf - x, yf - y);
}