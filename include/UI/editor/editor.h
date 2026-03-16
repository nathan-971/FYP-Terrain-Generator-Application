#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <vector>
#include <memory>
#include <filesystem>

#include "exporter/iexporter.h"
#include "exporter/iterrainexporterservice.h"

#include "UI/panels/ipanel.h"
#include "UI/editor/editorcontext.h"

#include "scene/iscene.h"

class Editor
{
public:
	Editor(IScene& scene, ITerrainExporterService& exporterService);

	void Render();
	void ApplyCommands();
private:
	void renderExportPopup();
	void renderFileDialog();
	bool canAccessDirectory(const std::filesystem::path& path);

	IScene& scene;
	ITerrainExporterService& exporterService;

	EditorContext ctx;
	std::vector<std::unique_ptr<IPanel>> panels;

	bool showExportPopup = false;
	bool exportConfirmed = false;
	bool exportPathMade = false;
	FileType selectedExportType = FileType::FBX;
	std::string exportPathString = "";
};

#endif