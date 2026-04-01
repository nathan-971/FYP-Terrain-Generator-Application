#include "terrain/warp/doublewarp.h"

DoubleWarp::DoubleWarp(float frequency, float multiplier) :
	frequency(frequency), multiplier(multiplier) { }

void DoubleWarp::Apply(float& x, float& z, INoise& noise) const
{
	warp(x, z, noise);
	warp(x, z, noise);
}

void DoubleWarp::warp(float& x, float& z, INoise& noise) const
{
	float shift = 0.001f;

	float noiseXChangePlus = noise.Get((x + shift) * frequency, z * frequency);
	float noiseXChangeMinus = noise.Get((x - shift) * frequency, z * frequency);
	float noiseSlopeX = (noiseXChangePlus - noiseXChangeMinus) / (2.0f * shift);

	float noiseZChangePlus = noise.Get(x * frequency, (z + shift) * frequency);
	float noiseZChangeMinus = noise.Get(x * frequency, (z - shift) * frequency);
	float noiseSlopeZ = (noiseZChangePlus - noiseZChangeMinus) / (2.0f * shift);

	x = x + (noiseSlopeZ * multiplier);
	z = z + (-noiseSlopeX * multiplier);
}

void DoubleWarp::UpdateParameters(float freq, float mult)
{
	frequency = freq;
	multiplier = mult;
}