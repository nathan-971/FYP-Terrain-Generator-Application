#ifndef _FBX_EXPORTER_H_
#define _FBX_EXPORTER_H_

#include "scene/terrain/terrainmesh.h"
#include "scene/mesh/vertex.h"

#include "exporter/iexporter.h"

#include <fbxsdk.h>

class FBXExporter : public IExporter
{
public:
	bool Export(const ITerrainMesh& mesh, std::string outputPath) override;
};

#endif