#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <vector>
#include <memory>
#include <filesystem>

#include "UI/panels/ipanel.h"
#include "UI/editor/editorcontext.h"

#include "scene/scene.h"

class Editor
{
public:
	Editor(Scene& scene);

	void Render();
	void ApplyCommands();
private:
	void renderExportPopup();
	void renderFileDialog();
	bool canAccessDirectory(const std::filesystem::path& path);

	Scene& scene;

	EditorContext ctx;
	std::vector<std::unique_ptr<IPanel>> panels;

	bool showExportPopup = false;
	bool exportConfirmed = false;
	bool exportPathMade = false;
	FileType selectedExportType = FileType::FBX;
	std::string exportPathString = "";
};

#endif