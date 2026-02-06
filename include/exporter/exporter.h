#ifndef _EXPORTER_H_
#define _EXPORTER_H_

#include "renderer/terrainmesh.h"
#include <string>

enum class FileType
{
    FBX,
    OBJ
};

class Exporter
{
public:
    virtual ~Exporter() = default;
    virtual bool Export(TerrainMesh& mesh, std::string outputPath) = 0;
};

#endif