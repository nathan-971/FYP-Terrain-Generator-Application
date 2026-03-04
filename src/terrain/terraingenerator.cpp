#include "terrain/terraingenerator.h"
#include "renderer/terrainmesh.h"
#include "terrain/heightmap.h"

TerrainGenerator::TerrainGenerator(
	std::unique_ptr<IHeightGenerator> heightGenerator,
	std::unique_ptr<IErosion> erosion
) :  heightGenerator(std::move(heightGenerator)), erosion(std::move(erosion)) { }

TerrainGenerator::~TerrainGenerator() { }

void TerrainGenerator::Generate(TerrainMesh& mesh)
{
	HeightMap map(mesh.GetVertexXCount(), mesh.GetVertexZCount());
	heightGenerator->Generate(map);

	if (erosion)
	{
		erosion->Apply(map);
	}

	mesh.ApplyHeightMap(map);
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