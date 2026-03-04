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
	float eps = 0.001f;

	float noise_dx_plus = noise.Get((x + eps) * frequency, z * frequency);
	float noise_dx_minus = noise.Get((x - eps) * frequency, z * frequency);
	float dNoise_dx = (noise_dx_plus - noise_dx_minus) / (2.0f * eps);

	float noise_dz_plus = noise.Get(x * frequency, (z + eps) * frequency);
	float noise_dz_minus = noise.Get(x * frequency, (z - eps) * frequency);
	float dNoise_dz = (noise_dz_plus - noise_dz_minus) / (2.0f * eps);

	x = x + (dNoise_dz * multiplier);
	z = z + (-dNoise_dx * multiplier);
}

void DoubleWarp::UpdateParameters(float freq, float mult)
{
	frequency = freq;
	multiplier = mult;
}