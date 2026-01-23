#ifndef _FBX_EXPORTER_H_
#define _FBX_EXPORTER_H_

#include "exporter/exporter.h"
#include "renderer/vertex.h"
#include <fbxsdk.h>

class FBXExporter : public Exporter
{
public:
	bool Export(Mesh& mesh, std::string outputPath) override;
};

#endif