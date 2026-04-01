#include "gtest/gtest.h"

#include "terrain/warp/iwarp.h"

#include "terrain/noise/inoise.h"
#include "terrain/noise/perlinnoise.h"

#include "terrain/erosion/isimulatederosion.h"
#include "terrain/erosion/ierosionhandler.h"
#include "terrain/erosion/erosionhandler.h"
#include "terrain/erosion/hydraulicerosion.h"
#include "terrain/erosion/hydraulicconfig.h"

#include "terrain/height/iheightgenerator.h"
#include "terrain/height/basenoisegenerator.h"
#include "terrain/height/ridgednoisegenerator.h"

#include "terrain/terrainconfig.h"

#include "terrain/heightmap.h"

#pragma region Stub / Fake Setup
class MockNoise : public INoise 
{
public:
    float returnValue = 0.5f;

    float Get(float x, float y) override 
    {
        return returnValue;
    }
    void ApplySeed(int seed) override { }
};
#pragma endregion

TEST(PerlinNoiseTests, Perlin_Noise_Is_Deterministic)
{
    PerlinNoise noise1;
    PerlinNoise noise2;

    int seed = 12345;
    noise1.ApplySeed(seed);
    noise2.ApplySeed(seed);

    float val1 = noise1.Get(10.5f, 20.5f);
    float val2 = noise2.Get(10.5f, 20.5f);

    EXPECT_FLOAT_EQ(val1, val2);
}

TEST(BaseNoiseGeneratorTests, BaseNoiseGenerator_Populates_Heightmap)
{
    TerrainConfig mockConfig;
    mockConfig.amplitude = 1.0f;
    mockConfig.frequency = 1.0f;
    mockConfig.octaves = 1;
    mockConfig.scale = 1.0f;
    mockConfig.lacunarity = 2.0f;
    mockConfig.persistence = 0.5f;

    std::shared_ptr<INoise> mockPerlinNoise = std::make_shared<MockNoise>();
    std::unique_ptr<IWarp> mockWarp = nullptr;
    std::unique_ptr<IHeightGenerator> generator = std::make_unique<BaseNoiseGenerator>(
        mockConfig, mockPerlinNoise, std::move(mockWarp)
    );

    HeightMap map(4, 4);
    generator->Generate(map);

    for (int x = 0; x < map.getWidth(); ++x)
    {
        for (int z = 0; z < map.getDepth(); ++z)
        {
            EXPECT_FLOAT_EQ(map.getHeight(x, z), 0.5f * mockConfig.amplitude);
        }
    }
}

TEST(RidgedNoiseGeneratorTests, RidgedNoiseGenerator_Produced_Height_Within_Bounds)
{
    TerrainConfig mockConfig;
    mockConfig.amplitude = 1.0f;
    mockConfig.frequency = 1.0f;
    mockConfig.octaves = 3;
    mockConfig.scale = 1.0f;
    mockConfig.lacunarity = 2.0f;
    mockConfig.persistence = 0.5f;

    std::shared_ptr<INoise> mockPerlinNoise = std::make_shared<MockNoise>();
    std::unique_ptr<IWarp> mockWarp = nullptr;
    std::unique_ptr<IHeightGenerator> generator = std::make_unique<RidgedNoiseGenerator>(
        mockConfig, mockPerlinNoise, std::move(mockWarp)
    );

    HeightMap map(10, 10);
    generator->Generate(map);

    for (int x = 0; x < map.getWidth(); ++x)
    {
        for (int z = 0; z < map.getDepth(); ++z)
        {
            float height = map.getHeight(x, z);
            EXPECT_GE(height, 0.0f);
            EXPECT_LE(height, 4.0f);
        }
    }
}

TEST(RidgedNoiseGeneratorTests, RidgedNoiseGenerator_Fails_When_Height_Is_Minus)
{
    TerrainConfig mockConfig;
    mockConfig.amplitude = 1.0f;
    mockConfig.frequency = 1.0f;
    mockConfig.octaves = 1;
    mockConfig.scale = 1.0f;
    mockConfig.lacunarity = 2.0f;
    mockConfig.persistence = 0.5f;

    std::shared_ptr<INoise> mockPerlinNoise = std::make_shared<MockNoise>();
    std::unique_ptr<IWarp> mockWarp = nullptr;
    std::unique_ptr<IHeightGenerator> generator = std::make_unique<RidgedNoiseGenerator>(
        mockConfig, mockPerlinNoise, std::move(mockWarp)
    );

    HeightMap map(2, 2);
    generator->Generate(map);

    for (int x = 0; x < map.getWidth(); ++x)
    {
        for (int z = 0; z < map.getDepth(); ++z)
        {
            EXPECT_GE(map.getHeight(x, z), 0.0f);
        }
    }
}

TEST(ErosionTests, Erosion_Step_Modfies_Heightmap)
{
    HydraulicConfig mockConfig;
    mockConfig.numberOfDrops = 50;
    mockConfig.dropLifetime = 10;
    mockConfig.erodeSpeed = 0.3f;
    mockConfig.depositSpeed = 0.3f;
    int seed = 234324;

    std::unique_ptr<ISimulatedErosion> erosion = std::make_unique<HydraulicErosion>(mockConfig, seed);
    HeightMap map(8, 8);

    for (int x = 0; x < map.getWidth(); ++x)
    {
        for (int z = 0; z < map.getDepth(); ++z)
        {
            map.getHeight(x, z) = 1.0f;
        }
    }
    erosion->Step(map, 50);

    bool changed = false;
    for (int x = 0; x < map.getWidth(); ++x)
    {
        for (int z = 0; z < map.getDepth(); ++z)
        {
            if (map.getHeight(x, z) != 1.0f)
            {
                changed = true;
            }
        }
    }
    EXPECT_TRUE(changed);
}

TEST(ErosionTests, Finished_Becomes_True_After_All_Drops)
{
    HydraulicConfig mockConfig;
    mockConfig.numberOfDrops = 10;
    mockConfig.dropLifetime = 5;
    int seed = 12345;

    std::unique_ptr<ISimulatedErosion> erosion = std::make_unique<HydraulicErosion>(mockConfig, seed);
    HeightMap map(8, 8);

    erosion->Step(map, 5);
    EXPECT_FALSE(erosion->Finished());

    erosion->Step(map, 5);
    EXPECT_TRUE(erosion->Finished());
}

TEST(ErosionTests, HydraulicErosion_Is_Deterministic)
{
    HydraulicConfig mockConfig;
    mockConfig.numberOfDrops = 30;
    mockConfig.dropLifetime = 10;
    int seed = 12345;

    HeightMap mapA(16, 16);
    HeightMap mapB(16, 16);

    std::unique_ptr<ISimulatedErosion> erosionA = std::make_unique<HydraulicErosion>(mockConfig, seed);
    std::unique_ptr<ISimulatedErosion> erosionB = std::make_unique<HydraulicErosion>(mockConfig, seed);

    erosionA->Step(mapA, 30);
    erosionB->Step(mapB, 30);

    for (int x = 0; x < mapA.getWidth(); ++x)
    {
        for (int z = 0; z < mapA.getDepth(); ++z)
        {
            EXPECT_FLOAT_EQ(mapA.getHeight(x, z), mapB.getHeight(x, z));
        }
    }
}

TEST(ErosionTests, Reset_Erosion_Restores_Original_Map)
{
    HydraulicConfig mockConfig;
    mockConfig.numberOfDrops = 20;
    mockConfig.dropLifetime = 5;

    std::unique_ptr<ISimulatedErosion> erosion = std::make_unique<HydraulicErosion>(mockConfig, 42);
    std::unique_ptr<IErosionHandler> handler = std::make_unique<ErosionHandler>(std::move(erosion));

    HeightMap original(8, 8);
    for (int x = 0; x < original.getWidth(); ++x)
    {
        for (int z = 0; z < original.getDepth(); ++z)
        {
            original.getHeight(x, z) = 1.0f;
        }
    }

    handler->SetOriginalMap(original);

    handler->Start();
    handler->Update();
    handler->Update();
    handler->Stop();

    handler->Reset();

    HeightMap& result = handler->GetHeightMap();

    for (int x = 0; x < result.getWidth(); ++x)
    {
        for (int z = 0; z < result.getDepth(); ++z)
        {
            EXPECT_NEAR(result.getHeight(x, z), 1.0f, 1e-2f);
        }
    }
}