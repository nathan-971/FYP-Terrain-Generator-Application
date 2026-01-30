#ifndef _NOISE_H_
#define _NOISE_H_

struct Vector
{
    float x;
    float y;

    Vector(float x, float y) : x(x), y(y) {}
    Vector() : x(0.0f), y(0.0f) {}
};

class Noise
{
public:
	virtual ~Noise() = default;
	virtual float Get(float x, float y) = 0;
};

#endif