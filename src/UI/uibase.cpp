#include "ui/uibase.h"

UIBase::UIBase(
	IScene& scene,
	ICamera& camera,
	IViewportProvider& viewportProvider,
	IBakedAlbedoProvider& bakedAlbedoProvider,
	ITerrainExporterService& exporterService
) :
	scene(scene), 
	camera(camera), 
	viewport(camera, viewportProvider),
	editor(scene, exporterService, bakedAlbedoProvider) { }

UIBase::~UIBase() { }

void UIBase::PreRender()
{
	StartRootFrame();
	LayoutRootFrame();

	viewport.UpdateSize();
}

void UIBase::Render()
{
	editor.ApplyCommands();
	editor.Render();
	viewport.Render();

	ImGui::End();
}

void UIBase::StartRootFrame()
{
	const ImGuiViewport* windowViewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(windowViewport->Pos);
	ImGui::SetNextWindowSize(windowViewport->Size);
	ImGui::SetNextWindowViewport(windowViewport->ID);

	ImGui::Begin("Root", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus
	);

	ImGui::DockSpace(ImGui::GetID("RootDockspace"), ImVec2(0, 0),
		ImGuiDockNodeFlags_NoWindowMenuButton |
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoTabBar
	);
}

void UIBase::LayoutRootFrame()
{
	static bool built = false;
	if (built)
	{
		return;
	}
	built = true;

	ImGuiID dockspaceID = ImGui::GetID("RootDockspace");

	ImGui::DockBuilderRemoveNode(dockspaceID);
	ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

	ImGuiID leftDock;
	ImGuiID viewportDock;

	ImGui::DockBuilderSplitNode(
		dockspaceID,
		ImGuiDir_Left,
		0.30f,
		&leftDock,
		&viewportDock
	);

	ImGui::DockBuilderDockWindow("Terrain Editor", leftDock);
	ImGui::DockBuilderDockWindow("Terrain Viewport", viewportDock);

	ImGui::DockBuilderFinish(dockspaceID);
}