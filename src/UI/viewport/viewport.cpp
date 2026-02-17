#include "ui/viewport/viewport.h"

Viewport::Viewport(Scene& scene, Camera& camera, Renderer& renderer) :
	scene(scene), 
	camera(camera),
    renderer(renderer) { }

Viewport::~Viewport() { }

void Viewport::Render()
{
    ImGui::Begin("Terrain Viewport");

    ImVec2 avail = ImGui::GetContentRegionAvail();
    int newWidth = static_cast<int>(avail.x);
    int newHeight = static_cast<int>(avail.y);

    if (newWidth != framebuffer.width || newHeight != framebuffer.height)
    {
        Resize(newWidth, newHeight);
    }

    RenderSceneToTexture();
    ImGui::Image(
        (ImTextureID)(int*)framebuffer.color,
        avail,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGui::End();
}

void Viewport::Resize(int width, int height)
{
    framebuffer.width = width;
    framebuffer.height = height;

    if (framebuffer.fbo == 0)
    {
        glGenFramebuffers(1, &framebuffer.fbo);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);

    if (framebuffer.color == 0)
    {
        glGenTextures(1, &framebuffer.color);
    }

    glBindTexture(GL_TEXTURE_2D, framebuffer.color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebuffer.width, framebuffer.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer.color, 0);

    if (framebuffer.depth == 0)
    {
        glGenRenderbuffers(1, &framebuffer.depth);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, framebuffer.depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer.depth);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    camera.onResize(framebuffer.width, framebuffer.height);
}

void Viewport::RenderSceneToTexture()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);
    glViewport(0, 0, framebuffer.width, framebuffer.height);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.updateCameraMatrix(FOV, NEAR_PLANE, FAR_PLANE);

    scene.Update();
    renderer.RenderScene(camera, scene);

    glViewport(0, 0, framebuffer.width, framebuffer.height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
