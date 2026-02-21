#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <vector>
#include <memory>

#include "UI/panels/ipanel.h"
#include "UI/editor/editorcontext.h"

#include "renderer/scene.h"

class Editor
{
public:
	Editor(Scene& scene);

	void Render();
	void ApplyCommands();
private:
	Scene& scene;

	EditorContext ctx;
	std::vector<std::unique_ptr<IPanel>> panels;
};

#endif