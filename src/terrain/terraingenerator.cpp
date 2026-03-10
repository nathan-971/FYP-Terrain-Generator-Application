#include "terrain/terraingenerator.h"
#include "renderer/terrainmesh.h"
#include "terrain/heightmap.h"

TerrainGenerator::TerrainGenerator(
	std::unique_ptr<IHeightGenerator> heightGenerator,
	std::unique_ptr<IErosionHandler> erosionHandler
) :  heightGenerator(std::move(heightGenerator)), erosionHandler(std::move(erosionHandler)) { }

TerrainGenerator::~TerrainGenerator() { }

void TerrainGenerator::Generate(TerrainMesh& mesh)
{
	HeightMap map(mesh.GetVertexXCount(), mesh.GetVertexZCount());
	heightGenerator->Generate(map);

	if (erosionHandler)
	{
		erosionHandler->SetOriginalMap(map);
	}

	mesh.ApplyHeightMap(map);
	mesh.RecalculateNormals();
	mesh.UpdateBuffers();
}

void TerrainGenerator::Update(TerrainMesh& mesh)
{
	if (!erosionHandler)
	{
		return;
	}

	erosionHandler->Update();

	mesh.ApplyHeightMap(erosionHandler->GetHeightMap());
	mesh.RecalculateNormals();
	mesh.UpdateBuffers();
}

void TerrainGenerator::UpdateParameters(const TerrainConfig& config)
{
	if (heightGenerator)
	{
		heightGenerator->SetConfig(config);
	}
}

void TerrainGenerator::StartErosion()
{
	if (erosionHandler)
	{
		erosionHandler->Start();
	}
}

void TerrainGenerator::StopErosion()
{
	if (erosionHandler)
	{
		erosionHandler->Stop();
	}
}

void TerrainGenerator::ResetErosion()
{
	if (erosionHandler)
	{
		erosionHandler->Reset();
	}
}