#ifndef _I_TERRAIN_EXPORTER_SERVICE_H_
#define _I_TERRAIN_EXPORTER_SERVICE_H_

#include "exporter/iexporter.h"

#include "scene/mesh/iterrainmesh.h"

class ITerrainExporterService
{
public:
	virtual ~ITerrainExporterService() = default;
	virtual bool ExportTerrain(const ITerrainMesh& mesh, const FileType& type, const std::string& path) = 0;
	virtual void ExportAlbedoTexture(const std::string& path, int framebufferWidth, int framebufferHeight, unsigned int framebuffer) = 0;
};

#endif