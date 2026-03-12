#ifndef _FBX_EXPORTER_H_
#define _FBX_EXPORTER_H_

#include "exporter/iexporter.h"
#include "renderer/vertex.h"
#include <fbxsdk.h>

class FBXExporter : public IExporter
{
public:
	bool Export(TerrainMesh& mesh, std::string outputPath) override;
};

#endif