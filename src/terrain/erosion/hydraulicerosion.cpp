#include "terrain/erosion/hydraulicerosion.h"

HydraulicErosion::HydraulicErosion(const HydraulicConfig& config, int seed) 
	: config(config), seed(seed), rng(this->seed) { }

HydraulicErosion::~HydraulicErosion() { }

void HydraulicErosion::Apply(HeightMap& heightmap)
{
	int width = heightmap.getWidth() - 2;
	int depth = heightmap.getDepth() - 2;

	std::uniform_real_distribution<float> distX(0.0f, static_cast<float>(width));
	std::uniform_real_distribution<float> distZ(0.0f, static_cast<float>(depth));

	for (int i = 0; i < config.numberOfDrops; i++)
	{
		Droplet drop;
		drop.posX = distX(rng);
		drop.posZ = distZ(rng);

		for (int step = 0; step < config.dropLifetime; step++)
		{
			drop.posX = std::clamp(drop.posX, 0.0f, static_cast<float>(width));
			drop.posZ = std::clamp(drop.posZ, 0.0f, static_cast<float>(depth));

			int x = static_cast<int>(drop.posX);
			int z = static_cast<int>(drop.posZ);

			float fx = drop.posX - x;
			float fz = drop.posZ - z;

			float currentHeight = heightmap.sample(drop.posX, drop.posZ);

			float gradX = heightmap.sample(drop.posX + 1.0f, drop.posZ) -
				heightmap.sample(drop.posX - 1.0f, drop.posZ);

			float gradZ = heightmap.sample(drop.posX, drop.posZ + 1.0f) -
				heightmap.sample(drop.posX, drop.posZ - 1.0f);

			drop.dirX = drop.dirX * config.inertia - gradX * (1 - config.inertia);
			drop.dirZ = drop.dirZ * config.inertia - gradZ * (1 - config.inertia);

			float len = std::sqrt(drop.dirX * drop.dirX + drop.dirZ * drop.dirZ);
			if (len != 0.0f)
			{
				drop.dirX /= len;
				drop.dirZ /= len;
			}

			drop.posX += drop.dirX * drop.speed;
			drop.posZ += drop.dirZ * drop.speed;

			float newHeight = heightmap.sample(drop.posX, drop.posZ);
			float deltaHeight = newHeight - currentHeight;
			float capacity = std::max(
				-deltaHeight * drop.speed * drop.water * 
				config.capacityFactor, config.minCapacity
			);

			if (drop.sediment > capacity || deltaHeight > 0.0f)
			{
				float depositAmount = (deltaHeight > 0.0f) ?
					std::min(deltaHeight, drop.sediment) : (drop.sediment - capacity) * config.depositSpeed;

				depositAmount = std::clamp(depositAmount, 0.0f, config.maxDepositPerStep);
				drop.sediment -= depositAmount;
				heightmap.addBilinear(x, z, fx, fz, depositAmount);
			}
			else
			{
				float erodeAmount = (capacity - drop.sediment) * config.erodeSpeed;

				erodeAmount = std::clamp(erodeAmount, 0.0f, config.maxErodePerStep);
				drop.sediment += erodeAmount;
				heightmap.addBilinear(x, z, fx, fz, -erodeAmount);
			}
			drop.water *= (1.0f - config.evaporation);
		}
	}
}

void HydraulicErosion::Step(HeightMap& heightmap, int dropsPerStep)
{
    if (this->Finished())
    {
        return;
    }

    int width = heightmap.getWidth() - 2;
    int depth = heightmap.getDepth() - 2;

    std::uniform_real_distribution<float> distX(0.0f, static_cast<float>(width));
    std::uniform_real_distribution<float> distZ(0.0f, static_cast<float>(depth));

    int dropsToSimulate = std::min(dropsPerStep, config.numberOfDrops - currentDrop);

    for (int i = 0; i < dropsToSimulate; i++)
    {
        Droplet drop;

        drop.posX = distX(rng);
        drop.posZ = distZ(rng);

        for (int step = 0; step < config.dropLifetime; step++)
        {
            drop.posX = std::clamp(drop.posX, 0.0f, static_cast<float>(width));
            drop.posZ = std::clamp(drop.posZ, 0.0f, static_cast<float>(depth));

            int x = static_cast<int>(drop.posX);
            int z = static_cast<int>(drop.posZ);

            float fx = drop.posX - x;
            float fz = drop.posZ - z;

            float currentHeight = heightmap.sample(drop.posX, drop.posZ);

            float gradX = heightmap.sample(drop.posX + 1.0f, drop.posZ) -
                heightmap.sample(drop.posX - 1.0f, drop.posZ);

            float gradZ = heightmap.sample(drop.posX, drop.posZ + 1.0f) -
                heightmap.sample(drop.posX, drop.posZ - 1.0f);

            drop.dirX = drop.dirX * config.inertia - gradX * (1 - config.inertia);
            drop.dirZ = drop.dirZ * config.inertia - gradZ * (1 - config.inertia);

            float len = std::sqrt(drop.dirX * drop.dirX + drop.dirZ * drop.dirZ);

            if (len != 0.0f)
            {
                drop.dirX /= len;
                drop.dirZ /= len;
            }

            drop.posX += drop.dirX * drop.speed;
            drop.posZ += drop.dirZ * drop.speed;

            float newHeight = heightmap.sample(drop.posX, drop.posZ);
            float deltaHeight = newHeight - currentHeight;

            float capacity = std::max(
                -deltaHeight * drop.speed * drop.water * config.capacityFactor,
                config.minCapacity
            );

			if (drop.sediment > capacity || deltaHeight > 0.0f)
			{
				float depositAmount = (deltaHeight > 0.0f) ?
					std::min(deltaHeight, drop.sediment) : (drop.sediment - capacity) * config.depositSpeed;

				depositAmount = std::clamp(depositAmount, 0.0f, config.maxDepositPerStep);
				drop.sediment -= depositAmount;
				heightmap.addBilinear(x, z, fx, fz, depositAmount);
			}
			else
			{
				float erodeAmount = (capacity - drop.sediment) * config.erodeSpeed;

				erodeAmount = std::clamp(erodeAmount, 0.0f, config.maxErodePerStep);
				drop.sediment += erodeAmount;
				heightmap.addBilinear(x, z, fx, fz, -erodeAmount);
			}
            drop.water *= (1.0f - config.evaporation);
        }
        currentDrop++;
    }
}

void HydraulicErosion::Reset()
{
	currentDrop = 0;
	rng.seed(this->seed);
}

bool HydraulicErosion::Finished() const
{
	return currentDrop >= config.numberOfDrops;
}