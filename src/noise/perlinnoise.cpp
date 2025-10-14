#include "noise/perlinnoise.h"

//Ken perlin's permutation table
const int PerlinNoise::permutation[256] = {
    151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,
    140,36,103,30,69,142,8,99,37,240,
    21,10,23,190, 6,148,247,120,234,75,
    0,26,197,62,94,252,219,203,117,35,
    11,32,57,177,33,88,237,149,56,87,
    174,20,125,136,171,168, 68,175,74,
    165,71,134,139,48,27,166,77,146,158,
    231,83,111,229,122,60,211,133,230,220,
    105,92,41,55,46,245,40,244,102,143,
    54, 65,25,63,161, 1,216,80,73,209,
    76,132,187,208,89,18,169,200,196,135,
    130,116,188,159,86,164,100,109,198,173,
    186, 3,64,52,217,226,250,124,123, 5,
    202,38,147,118,126,255,82,85,212,207,
    206,59,227,47,16,58,17,182,189,28,
    42,223,183,170,213,119,248,152, 2,44,
    154,163,70,221,153,101,155,167, 43,
    172, 9,129,22,39,253, 19,98,108,
    110,79,113,224,232,178,185,112,104,
    218,246,97,228,251,34,242,193,238,210,
    144,12,191,179,162,241,81,51,145,235,
    249,14,239,107,49,192,214, 31,181,199,
    106,157,184, 84,204,176,115,121,50,
    45,127,  4,150,254,138,236,205,93,
    222,114, 67,29, 24, 72,243,141,128,
    195,78,66,215,61,156,180
};

int PerlinNoise::perm[512];
bool PerlinNoise::initialized = false;

void PerlinNoise::initPermTable()
{
    if (initialized)
    {
        return;
    }
    for (int i = 0; i < 256; i++) 
    {
        perm[i] = permutation[i];
        perm[i + 256] = permutation[i];
    }
    initialized = true;
}

float PerlinNoise::Get(float x, float y, float freq)
{
    initPermTable();

    float xf = x * freq;
    float yf = y * freq;

    int x0 = (int)floor(xf);
    int y0 = (int)floor(yf);
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

    return result;
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
    int h = hash(x, y);
    float angle = (h / 256.0f) * 2.0f * M_PI;
    return Vector(cos(angle), sin(angle));
}

int PerlinNoise::hash(int x, int y)
{
    return perm[(x + perm[y & 255]) & 255];
}

Vector PerlinNoise::getVectorDistance(float xf, float x, float yf, float y)
{
    return Vector(xf - x, yf - y);
}