#ifndef _OBJ_EXPORTER_H_
#define _OBJ_EXPORTER_H_

#include "exporter/exporter.h"
#include "renderer/vertex.h"

class OBJExporter : public Exporter
{
public:
	bool Export(TerrainMesh& mesh, std::string outputPath) override;
};

#endif