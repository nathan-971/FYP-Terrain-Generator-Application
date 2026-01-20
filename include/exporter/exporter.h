#ifndef _EXPORTER_H_
#define _EXPORTER_H_

#include "renderer/mesh.h"
#include <string>

enum class FileType
{
    FBX
};

class Exporter
{
public:
    virtual ~Exporter() = default;
    virtual bool Export(Mesh& mesh, std::string outputPath) = 0;
};

#endif