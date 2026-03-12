#ifndef _OBJ_EXPORTER_H_
#define _OBJ_EXPORTER_H_

#include "exporter/iexporter.h"
#include "renderer/vertex.h"

class OBJExporter : public IExporter
{
public:
	bool Export(TerrainMesh& mesh, std::string outputPath) override;
};

#endif