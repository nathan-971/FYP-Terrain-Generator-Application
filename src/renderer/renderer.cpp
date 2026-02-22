#include "renderer/renderer.h"
#include <utility>

#define SHADOWMAP_HEIGHT 2048
#define SHADOWMAP_WIDTH 2048

Renderer::Renderer(
    std::unique_ptr<IShader> depthShader,
    std::unique_ptr<IShader> terrainShader,
    std::unique_ptr<IShader> skyboxShader,
    int initiaWidth,
    int initiaHeight
) :
	depthShader(std::move(depthShader)),
	terrainShader(std::move(terrainShader)),
	skyboxShader(std::move(skyboxShader))
{ 
    createShadowFrameBuffer(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
	createViewportFrameBuffer(initiaWidth, initiaHeight);
}

Renderer::~Renderer() 
{ 
	destroyFramebuffer(shadowMapFramebuffer);
	destroyFramebuffer(viewportFramebuffer);
}

void Renderer::RenderScene(const FrameData& frameData)
{
    renderDepthPass(frameData);

    glBindFramebuffer(GL_FRAMEBUFFER, viewportFramebuffer.fbo);
    glViewport(0, 0, viewportFramebuffer.width, viewportFramebuffer.height);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderLightPass(frameData);

    if (frameData.skybox.enabled)
    {
        renderSkyboxPass(frameData);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::ResizeViewport(int width, int height)
{
    if (width == 0 || height == 0)
    {
        return;
    }

	destroyFramebuffer(viewportFramebuffer);
	createViewportFrameBuffer(width, height);
}

void Renderer::renderDepthPass(const FrameData& frameData)
{
    TerrainMesh* mesh = frameData.terrain.terrainMesh;

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer.fbo);
    glViewport(0, 0, shadowMapFramebuffer.width, shadowMapFramebuffer.height);
    glClear(GL_DEPTH_BUFFER_BIT);

    depthShader->Activate();
    depthShader->setUniformMat("lightSpaceMatrix", frameData.lightSpaceMatrix);
    depthShader->setUniformMat("model", frameData.terrain.modelMatrix);

    mesh->Draw();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderLightPass(const FrameData& frameData)
{
    glm::mat4 model = frameData.terrain.modelMatrix;
    TerrainMesh* mesh = frameData.terrain.terrainMesh;

    terrainShader->Activate();
    terrainShader->setUniformMat("lightSpaceMatrix", frameData.lightSpaceMatrix);
    terrainShader->setUniformMat("view", frameData.viewMatrix);
    terrainShader->setUniformMat("projection", frameData.projectionMatrix);
    terrainShader->setUniformVec3("viewPos", frameData.cameraPosition);

    terrainShader->setUniformVec3("lightPos", frameData.lightPosition);
    terrainShader->setUniformVec3("lightColor", frameData.lightColor);
    terrainShader->setUniformFloat("ambientStrength", frameData.ambientStrength);
    terrainShader->setUniformFloat("specularStrength", frameData.specularStrength);
    terrainShader->setUniformInt("shininess", frameData.shininess);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMapFramebuffer.depth);
    terrainShader->setUniformInt("shadowMap", 0);

    terrainShader->setUniformMat("model", model);
	mesh->Draw();
}

void Renderer::renderSkyboxPass(const FrameData& frameData)
{
    glm::mat4 view = glm::mat4(glm::mat3(frameData.viewMatrix));
    SkyboxMesh* mesh = frameData.skybox.skyboxMesh;

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    
    skyboxShader->Activate();

    skyboxShader->setUniformMat("view", view);
    skyboxShader->setUniformMat("projection", frameData.projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, frameData.skybox.skyboxTexture);

    glDisable(GL_CULL_FACE);
    mesh->Draw();
    glEnable(GL_CULL_FACE);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}


void Renderer::createShadowFrameBuffer(int width, int height)
{
    shadowMapFramebuffer.width = width;
    shadowMapFramebuffer.height = height;

    glGenFramebuffers(1, &shadowMapFramebuffer.fbo);

    glGenTextures(1, &shadowMapFramebuffer.depth);
    glBindTexture(GL_TEXTURE_2D, shadowMapFramebuffer.depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapFramebuffer.width, shadowMapFramebuffer.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer.fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapFramebuffer.depth, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::createViewportFrameBuffer(int width, int height)
{
    viewportFramebuffer.width = width;
    viewportFramebuffer.height = height;

    glGenFramebuffers(1, &viewportFramebuffer.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, viewportFramebuffer.fbo);

    glGenTextures(1, &viewportFramebuffer.color);
    glBindTexture(GL_TEXTURE_2D, viewportFramebuffer.color);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportFramebuffer.width, viewportFramebuffer.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewportFramebuffer.color, 0);

    glGenRenderbuffers(1, &viewportFramebuffer.depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, viewportFramebuffer.depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, viewportFramebuffer.depthRenderBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::destroyFramebuffer(Framebuffer& framebuffer)
{
    if (framebuffer.depth)
    {
        glDeleteTextures(1, &framebuffer.depth);
    }

    if (framebuffer.depthRenderBuffer)
    {
        glDeleteRenderbuffers(1, &framebuffer.depthRenderBuffer);
    }

    if (framebuffer.color)
    {
        glDeleteTextures(1, &framebuffer.color);
    }

    if (framebuffer.fbo)
    {
        glDeleteFramebuffers(1, &framebuffer.fbo);
    }

    framebuffer = { };
}

unsigned int Renderer::getViewportTexture() const
{
    return viewportFramebuffer.color;
}

int Renderer::getViewportWidth() const
{
    return viewportFramebuffer.width;
}

int Renderer::getViewportHeight() const
{
    return viewportFramebuffer.height;
}