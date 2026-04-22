#include "exporter/terrainexporterservice.h"
#include "exporter/iexporter.h"

#include "glad/glad.h"
#include "stb/stb_image_write.h"

#include <iostream>

TerrainExporterService::TerrainExporterService(std::unique_ptr<IExporterFactory> factory)
	: factory(std::move(factory)) 
{
	exporter = this->factory->Create(FileType::FBX);
}

TerrainExporterService::~TerrainExporterService() { }

bool TerrainExporterService::ExportTerrain(const ITerrainMesh& mesh, const FileType& type, const std::string& path)
{
    try
    {
        exporter = factory->Create(type);
        if (exporter->Export(mesh, path))
        {
            std::cout << "Saved model file!";
            return true;
        }
        std::cout << "Failed to save Model File!";
        return false;
    }
    catch (const std::exception& e)
    {
        std::cout << "Error Exporting Terrain to File!";
        return false;
    }
}

void TerrainExporterService::ExportAlbedoTexture(const std::string& path, int framebufferWidth, int framebufferHeight, unsigned int framebuffer)
{
    std::vector<unsigned char> imageData(framebufferWidth * framebufferHeight * 3);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glReadPixels(0, 0, framebufferWidth, framebufferHeight, GL_RGB, GL_UNSIGNED_BYTE, imageData.data());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    stbi_flip_vertically_on_write(true);
    stbi_write_png(path.c_str(), framebufferWidth, framebufferHeight, 3, imageData.data(), framebufferWidth * 3);
}