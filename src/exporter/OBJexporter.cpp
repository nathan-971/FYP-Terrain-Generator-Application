#include "exporter/OBJexporter.h"

bool OBJExporter::Export(TerrainMesh& mesh, std::string outputPath)
{
	std::vector<Vertex>& vertices = mesh.GetVertices();
	std::vector<unsigned int>& indices = mesh.GetIndices();

	return false;
}