#ifndef _I_TERRAIN_EXPORTER_SERVICE_H_
#define _I_TERRAIN_EXPORTER_SERVICE_H_

#include "exporter/iexporter.h"

#include "scene/terrain/terrainmesh.h"

class ITerrainExporterService
{
public:
	virtual ~ITerrainExporterService() = default;
	virtual bool ExportTerrain(const TerrainMesh& mesh, const FileType& type, const std::string& path) = 0;
};

#endif