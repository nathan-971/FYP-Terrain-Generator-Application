#include "terrain/height/basenoisegenerator.h"
#include "core/parallelutils.h"

BaseNoiseGenerator::BaseNoiseGenerator(
	const TerrainConfig& config, 
	std::shared_ptr<INoise> noise, 
	std::unique_ptr<IWarp> warp) :
	config(config), noise(noise), warp(std::move(warp)) { }

void BaseNoiseGenerator::Generate(HeightMap& map)
{
	int width = map.getWidth();
	int depth = map.getDepth();

	Parallel::ParallelForRange(width, depth, [&](int startX, int endX) {
		for (int x = startX; x < endX; x++)
		{
			for (int z = 0; z < map.getDepth(); z++)
			{
				float amplitude = this->config.amplitude;
				float frequency = this->config.frequency;
				float totalNoise = 0.0f;

				float angle = 0.785398f; //45 Degrees In Radians
				float cosOfAngle = cos(angle);
				float sinOfAngle = sin(angle);

				float rotatedX = x * cosOfAngle - z * sinOfAngle;
				float rotatedZ = x * sinOfAngle + z * cosOfAngle;

				float noiseX = rotatedX * this->config.scale;
				float noiseZ = rotatedZ * this->config.scale;

				if (warp)
				{
					this->warp->Apply(noiseX, noiseZ, *noise);
				}

				for (int i = 0; i < this->config.octaves; i++)
				{
					float n = noise->Get(noiseX * frequency, noiseZ * frequency);
					totalNoise += n * amplitude;

					frequency *= this->config.lacunarity;
					amplitude *= this->config.persistence;
				}
				map.getHeight(x, z) = totalNoise;
			}
		}
	});
}

void BaseNoiseGenerator::SetConfig(const TerrainConfig& cfg)
{
	this->config = cfg;

	if (warp)
	{
		warp->UpdateParameters(cfg.warpFrequency, cfg.warpMultiplier);
	}
}