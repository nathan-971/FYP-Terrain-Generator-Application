#include <exporter/FBXexporter.h>
#include <iostream>

bool FBXExporter::Export(Mesh& mesh, std::string outputPath)
{
	std::vector<Vertex>& vertices = mesh.GetVertices();
	std::vector<unsigned int>& indicies = mesh.GetIndices();

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

	for (unsigned int idx : indicies)
	{
		if (idx >= vertexCount)
		{
			std::cout << "Index out of bounds: " << idx << std::endl;
			return false;
		}
	}


	fbxMesh->InitControlPoints(vertexCount);

	FbxVector4* controlPoints = fbxMesh->GetControlPoints();

	for (int i = 0; i < vertexCount; i++)
	{
		Vertex& v = vertices[i];
		controlPoints[i].Set(v.position.x, v.position.y, v.position.z);
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

	for (Vertex& v : vertices)
	{
		normals->GetDirectArray().Add(
			FbxVector4(v.normal.x, v.normal.y, v.normal.z)
		);
	}
	layer->SetNormals(normals);

	auto* colors = FbxLayerElementVertexColor::Create(fbxMesh, "VertexColors");
	colors->SetMappingMode(FbxLayerElement::eByControlPoint);
	colors->SetReferenceMode(FbxLayerElement::eDirect);

	for (Vertex& v : vertices)
	{
		colors->GetDirectArray().Add(
			FbxVector4(v.color.x, v.color.y, v.color.z)
		);
	}
	layer->SetVertexColors(colors);

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