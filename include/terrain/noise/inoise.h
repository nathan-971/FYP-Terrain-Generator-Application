#ifndef _NOISE_H_
#define _NOISE_H_

struct Vector
{
    float x;
    float y;

    Vector(float x, float y) : x(x), y(y) {}
    Vector() : x(0.0f), y(0.0f) {}
};

class INoise
{
public:
	virtual ~INoise() = default;
	virtual float Get(float x, float y) = 0;
    virtual void ApplySeed(int seed) = 0;
};

#endif