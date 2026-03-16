#ifndef _TERRAIN_EXPORTER_SERVICE_H_
#define _TERRAIN_EXPORTER_SERVICE_H_

#include "scene/terrain/terrainmesh.h"

#include "exporter/iterrainexporterservice.h"

#include "utils/iexporterfactory.h"

#include <memory>

class TerrainExporterService : public ITerrainExporterService
{
public:
	TerrainExporterService(std::unique_ptr<IExporterFactory> factory);
	~TerrainExporterService();

	bool ExportTerrain(const TerrainMesh& mesh, const FileType& type, const std::string& path) override;

private:
	std::unique_ptr<IExporterFactory> factory;
	std::unique_ptr<IExporter> exporter;
};

#endif