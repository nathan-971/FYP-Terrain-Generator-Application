#ifndef _EXPORTER_H_
#define _EXPORTER_H_

#include "scene/terrain/terrainmesh.h"
#include <string>

enum class FileType
{
    FBX,
    OBJ
};

class IExporter
{
public:
    virtual ~IExporter() = default;
    virtual bool Export(const TerrainMesh& mesh, std::string outputPath) = 0;
};

#endif