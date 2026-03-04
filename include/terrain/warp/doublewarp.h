#ifndef _DOUBLE_WARP_H_
#define _DOUBLE_WARP_H_

#include "terrain/warp/iwarp.h"
#include "terrain/noise/inoise.h"

class DoubleWarp : public IWarp
{
public:
    DoubleWarp(float frequency, float multiplier);
    void Apply(float& x, float& z, INoise& noise) const override;
    void UpdateParameters(float freq, float mult) override;

private:
    void warp(float& x, float& z, INoise& noise) const;

    float frequency;
    float multiplier;
};

#endif