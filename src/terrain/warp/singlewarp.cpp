#include "terrain/warp/singlewarp.h"

SingleWarp::SingleWarp(float frequency, float multiplier) :
	frequency(frequency), multiplier(multiplier) { }

void SingleWarp::Apply(float& x, float& z, INoise& noise) const
{
	warp(x, z, noise);
}

void SingleWarp::warp(float& x, float& z, INoise& noise) const
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

void SingleWarp::UpdateParameters(float freq, float mult)
{
	frequency = freq;
	multiplier = mult;
}