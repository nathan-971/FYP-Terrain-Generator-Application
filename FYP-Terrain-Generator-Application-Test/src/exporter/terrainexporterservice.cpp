#include "exporter/terrainexporterservice.h"
#include "exporter/iexporter.h"

#include "utils/iexporterfactory.h"
#include "scene/mesh/iterrainmesh.h"

#include <iostream>

TerrainExporterService::TerrainExporterService(std::unique_ptr<IExporterFactory> factory)
	: factory(std::move(factory)) 
{
	exporter = this->factory->Create(FileType::FBX);
}

TerrainExporterService::~TerrainExporterService() { }

bool TerrainExporterService::ExportTerrain(const ITerrainMesh& mesh, const FileType& type, const std::string& path)
{
    try
    {
        exporter = factory->Create(type);
        if (exporter->Export(mesh, path))
        {
            std::cout << "Saved model file!";
            return true;
        }
        std::cout << "Failed to save Model File!";
        return false;
    }
    catch (const std::exception& e)
    {
        std::cout << "Error Exporting Terrain to File!";
        return false;
    }
}