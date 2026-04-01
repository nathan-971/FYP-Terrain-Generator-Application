#include "gtest/gtest.h"

#include "exporter/iexporter.h"
#include "exporter/terrainexporterservice.h"
#include "exporter/objexporter.h"

#include "utils/iexporterfactory.h"

#include "scene/mesh/iterrainmesh.h"

#include <string>
#include <fstream>

#pragma region Stub / Fake Setup
class MockTerrainMesh : public ITerrainMesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Transform transform;

    void UpdateBuffers() override { }
    void ApplyHeightMap(HeightMap& map) override { }
    void Create(unsigned int width, unsigned int depth, float resolution) override { }
    void RecalculateNormals() override { }

    const std::vector<Vertex>& GetVertices() const override { return vertices; }
    const std::vector<unsigned int>& GetIndices() const override { return indices; }
    const Transform& getTransform() const override { return transform; }
    Transform& getTransform() override { return transform; }

    int GetVertexXCount() override { return 0; }
    const int GetVertexXCount() const override { return 0; }
    int GetVertexZCount() override { return 0; }
    const int GetVertexZCount() const override { return 0; }
};

class MockExporter : public IExporter
{
public:
    bool shouldSucceed = false;
    bool wasCalled = false;

    bool Export(const ITerrainMesh& mesh, std::string outputPath) override
    {
        wasCalled = true;
        return shouldSucceed;
    }
};

class MockExporterFactory : public IExporterFactory
{
public:
    MockExporter* exporterPtr = nullptr;
    bool exporterShouldSucceed = false;

    std::unique_ptr<IExporter> Create(const FileType& type) override
    {
        auto exporter = std::make_unique<MockExporter>();
        exporter->shouldSucceed = exporterShouldSucceed;
        exporterPtr = exporter.get();
        return exporter;
    }
};
#pragma endregion

TEST(TerrainExporterServiceTests, Export_Terrain_Success)
{
    auto factory = std::make_unique<MockExporterFactory>();
    factory->exporterShouldSucceed = true;
    MockExporterFactory* factoryPtr = factory.get();

    auto service = std::make_unique<TerrainExporterService>(std::move(factory));

    MockTerrainMesh mesh;
    FileType type = FileType::OBJ;
    std::string path = "test.obj";

    bool result = service->ExportTerrain(mesh, type, path);

    ASSERT_TRUE(result);
    ASSERT_TRUE(factoryPtr->exporterPtr->wasCalled);
}

TEST(OBJExporterTests, Exporter_Correctly_Writes_Vertices_Indices_Data)
{
    MockTerrainMesh mesh;
    mesh.vertices = {
        {{0,0,0}, {0,1,0}, {1,0,0}},
        {{1,0,0}, {0,1,0}, {0,1,0}},
        {{0,0,1}, {0,1,0}, {0,0,1}}
    };
    mesh.indices = { 0, 1, 2 };
    
    std::string path = "test.obj";

    std::unique_ptr<IExporter> exporter = std::make_unique<OBJExporter>();
    bool exportResult = exporter->Export(mesh, path);

    EXPECT_TRUE(exportResult);

    std::ifstream file(path);
    ASSERT_TRUE(file.is_open());

    std::string line;
    bool hasVertex = false, hasNormal = false, hasFace = false;
    while (std::getline(file, line))
    {
        if (line.starts_with("v "))
        {
            hasVertex = true;
        }
        if (line.starts_with("vn "))
        {
            hasNormal = true;
        }
        if (line.starts_with("f "))
        {
            hasFace = true;
        }
    }

    EXPECT_TRUE(hasVertex);
    EXPECT_TRUE(hasNormal);
    EXPECT_TRUE(hasFace);
    file.close();
}
