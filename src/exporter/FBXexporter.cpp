#include <exporter/FBXexporter.h>
#include <iostream>

bool FBXExporter::Export(const ITerrainMesh& mesh, std::string outputPath)
{
	const std::vector<Vertex>& vertices = mesh.GetVertices();
	const std::vector<unsigned int>& indicies = mesh.GetIndices();

	FbxManager* manager = FbxManager::Create();
	if (!manager)
	{
		return false;
	}

	FbxIOSettings* IOSettings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(IOSettings);

	FbxScene* scene = FbxScene::Create(manager, "TerrainScene");
	FbxAxisSystem::OpenGL.ConvertScene(scene);
	FbxSystemUnit::m.ConvertScene(scene);

	FbxMesh* fbxMesh = FbxMesh::Create(scene, "TerrainMesh");

	FbxNode* meshNode = FbxNode::Create(scene, "TerrainMeshNode");
	meshNode->SetNodeAttribute(fbxMesh);
	scene->GetRootNode()->AddChild(meshNode);

	int vertexCount = vertices.size();

	for (unsigned int i : indicies)
	{
		if (i >= vertexCount)
		{
			std::cout << "Index out of bounds: " << i << std::endl;
			return false;
		}
	}

	fbxMesh->InitControlPoints(vertexCount);

	FbxVector4* controlPoints = fbxMesh->GetControlPoints();

	for (int i = 0; i < vertexCount; i++)
	{
		const Vertex& v = vertices[i];
		controlPoints[i].Set(
			v.position.x, 
			v.position.y, 
			v.position.z
		);
	}

	for (int i = 0; i < indicies.size(); i += 3)
	{
		fbxMesh->BeginPolygon();
		fbxMesh->AddPolygon(indicies[i + 0]);
		fbxMesh->AddPolygon(indicies[i + 1]);
		fbxMesh->AddPolygon(indicies[i + 2]);
		fbxMesh->EndPolygon();
	}

	FbxLayer* layer = fbxMesh->GetLayer(0);
	if (!layer)
	{
		fbxMesh->CreateLayer();
		layer = fbxMesh->GetLayer(0);
	}

	auto* normals = FbxLayerElementNormal::Create(fbxMesh, "Normals");
	normals->SetMappingMode(FbxLayerElement::eByControlPoint);
	normals->SetReferenceMode(FbxLayerElement::eDirect);
	for (const Vertex& v : vertices)
	{
		normals->GetDirectArray().Add(
			FbxVector4(
				v.normal.x,
				v.normal.y, 
				v.normal.z
			)
		);
	}
	layer->SetNormals(normals);

	auto* uvs = FbxLayerElementUV::Create(fbxMesh, "UVs");
	uvs->SetMappingMode(FbxLayerElement::eByControlPoint);
	uvs->SetReferenceMode(FbxLayerElement::eDirect);
	for (const Vertex& v : vertices)
	{
		uvs->GetDirectArray().Add(
			FbxVector2(
				v.uv.x, 
				v.uv.y
			)
		);
	}

	FbxExporter* exporter = FbxExporter::Create(manager, "");
	if (!exporter->Initialize(outputPath.c_str(), -1, manager->GetIOSettings()))
	{
		exporter->Destroy();
		manager->Destroy();
		return false;
	}

	bool success = exporter->Export(scene);
	if (!success)
	{
		FbxStatus& status = exporter->GetStatus();

		std::cout << "FBX Export failed\n";
		std::cout << "Error Code: " << status.GetCode() << "\n";
		std::cout << "Error Msg : " << status.GetErrorString() << std::endl;
	}
	
	exporter->Destroy();
	manager->Destroy();

	return success;
}