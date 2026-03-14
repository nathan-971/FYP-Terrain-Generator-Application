#include "UI/panels/heightmappanel.h"

void HeightMapPanel::Display(EditorContext& ctx)
{
	int heightMapTexture = 0;//generateHeightMapTexture(ctx);

    ImGui::SeparatorText("Height Map");
	ImVec2 imageSize(256, 256);
	float windowWidth = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX((windowWidth - imageSize.x) * 0.5f);
	ImGui::Image(
		(ImTextureID)(int*)heightMapTexture,
        imageSize,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
}

//int HeightMapPanel::generateHeightMapTexture(EditorContext& ctx)
//{
//    auto& scene = ctx.scene;
//    auto& mesh = ctx.scene.getTerrainMesh();
//    auto& config = ctx.terrainConfig;
//
//    float minHeight = 0.0f;
//	float maxHeight = 0.0f;
//
//    int vertCountX = mesh.GetVertexXCount();
//    int vertCountZ = mesh.GetVertexZCount();
//
//    auto& verts = scene.getTerrainMesh().GetVertices();
//    std::vector<float> heightMap(vertCountX * vertCountZ);
//    for (int i = 0; i < heightMap.size(); i++)
//    {
//        heightMap[i] = verts[i].position.y;
//		minHeight = std::min(minHeight, heightMap[i]);
//		maxHeight = std::max(maxHeight, heightMap[i]);
//        heightMap[i] = (heightMap[i] - minHeight) / (maxHeight - minHeight);
//    }
//
//    if (textureID == 0)
//    {
//        glGenTextures(1, &textureID);
//        glBindTexture(GL_TEXTURE_2D, textureID);
//
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, vertCountX, vertCountZ, 0, GL_RED, GL_FLOAT, nullptr);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//        int swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
//        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
//    }
//    else
//    {
//        glBindTexture(GL_TEXTURE_2D, textureID);
//    }
//
//    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, vertCountX, vertCountZ, GL_RED, GL_FLOAT, heightMap.data());
//    return textureID;
//}