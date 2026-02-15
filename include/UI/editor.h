#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "imgui/imgui.h"

#include "core/camera.h"
#include "UI/panels/ipanel.h"
#include "UI/editorcontext.h"
#include "UI/editorstate.h"
#include "renderer/scene.h"

class Editor
{
public:
	Editor(Scene& scene, Camera& camera);

	void Update();
	void Render();

private:
	Scene& scene;
	Camera& camera;

	EditorContext ctx;
	std::vector<std::unique_ptr<IPanel>> panels;
};

#endif