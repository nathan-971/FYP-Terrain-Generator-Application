#ifndef _OBJ_EXPORTER_H_
#define _OBJ_EXPORTER_H_

#include "exporter/iexporter.h"
#include "scene/mesh/vertex.h"

class OBJExporter : public IExporter
{
public:
	bool Export(const TerrainMesh& mesh, std::string outputPath) override;
};

#endif