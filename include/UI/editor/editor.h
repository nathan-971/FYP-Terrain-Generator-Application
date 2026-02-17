#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "core/camera.h"

#include "UI/panels/ipanel.h"
#include "UI/editor/editorcontext.h"
#include "UI/editor/editorstate.h"

#include "renderer/scene.h"

class Editor
{
public:
	Editor(Scene& scene, Camera& camera);

	void Render();
	void ApplyCommands();
private:
	Scene& scene;
	Camera& camera;

	EditorContext ctx;
	std::vector<std::unique_ptr<IPanel>> panels;
};

#endif