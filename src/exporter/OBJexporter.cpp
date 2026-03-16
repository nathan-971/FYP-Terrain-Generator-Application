#include "exporter/OBJexporter.h"
#include <fstream>

bool OBJExporter::Export(const TerrainMesh& mesh, std::string outputPath)
{
	const std::vector<Vertex>& vertices = mesh.GetVertices();
	const std::vector<unsigned int>& indices = mesh.GetIndices();

	std::ofstream objFile(outputPath);
	if (objFile.is_open())
	{
		objFile << "#Terrain OBJ Export\n";
		objFile << "o Terrain\n";
		
		for (const Vertex& vertex : vertices) //Write Vertices
		{
			objFile << "v " << vertex.position.x << " "
				<< vertex.position.y << " " 
				<< vertex.position.z << "\n";
		}

		for (const Vertex& vertex : vertices) //Write Normals
		{
			objFile << "vn " << vertex.normal.x << " "
				<< vertex.normal.y << " " 
				<< vertex.normal.z << "\n";
		}

		objFile << "s 1\n"; //Use Smoothing

		for (unsigned int i = 0; i < indices.size(); i += 3) //Write Faces From Indices
		{
			objFile << "f " << indices[i] + 1 << "//" << indices[i] + 1 << " "
				<< indices[i + 1] + 1 << "//" << indices[i + 1] + 1 << " "
				<< indices[i + 2] + 1 << "//" << indices[i + 2] + 1 << "\n";
		}
		objFile.close();
		return true;
	}
	return false;
}