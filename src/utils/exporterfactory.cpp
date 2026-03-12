#include "utils/exporterfactory.h"
#include "exporter/FBXexporter.h"
#include "exporter/OBJexporter.h"

std::unique_ptr<IExporter> ExporterFactory::Create(const FileType& type)
{
	switch (type)
	{
		case FileType::OBJ:
		{
			return std::make_unique<OBJExporter>();
		}
		case FileType::FBX:
		default:
		{
			return std::make_unique<FBXExporter>();
		}
	}
}