#include "renderer/renderer.h"

#include <utility>
#include <iostream>

#define SHADOWMAP_HEIGHT 2048
#define SHADOWMAP_WIDTH 2048

Renderer::Renderer(
    std::unique_ptr<IShader> depthShader,
    std::unique_ptr<IShader> terrainShader,
    std::unique_ptr<IShader> skyboxShader,
    std::unique_ptr<IShader> bakeAlbedoShader,
    int initiaWidth,
    int initiaHeight
) :
	depthShader(std::move(depthShader)),
	terrainShader(std::move(terrainShader)),
	skyboxShader(std::move(skyboxShader)),
    bakeAlbedoShader(std::move(bakeAlbedoShader))
{ 
    createShadowFrameBuffer(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
	createViewportFrameBuffer(initiaWidth, initiaHeight);
    createBakeAlbedoFrameBuffer(2048, 2048);
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

FrameData Renderer::getFrameData(const ICamera& camera, const IScene& scene)
{
    FrameData frame;
    auto& light = scene.Lighting().getLight();
    auto& terrain = scene.Terrain();
    auto& skybox = scene.Skybox();

    frame.viewMatrix = camera.getView();
    frame.projectionMatrix = camera.getProjection();
    frame.cameraPosition = camera.getPosition();

    glm::mat4 lightProjection = glm::ortho(
        -50.0f, 50.0f,
        -50.0f, 50.0f,
        5.0f, 80.0f
    );

    glm::mat4 lightView = glm::lookAt(
        light.position,
        glm::vec3(terrain.getMeshTransform().getMatrix()[3]),
        glm::vec3(0, 1, 0)
    );

    frame.lightSpaceMatrix = lightProjection * lightView;
    frame.lightPosition = light.position;
    frame.lightColor = light.color;
    frame.ambientStrength = light.ambient;
    frame.specularStrength = light.specular;
    frame.shininess = light.shininess;

    frame.terrain.modelMatrix = terrain.getMeshTransform().getMatrix();
    frame.terrain.terrainMesh = &terrain.getMesh();
    frame.terrain.grass = &terrain.getGrassMaterial();
    frame.terrain.stone = &terrain.getStoneMaterial();

    frame.skybox.skyboxMesh = &skybox.getMesh();
    frame.skybox.enabled = !skybox.isDisabled();
    frame.skybox.skyboxTexture = skybox.getActiveTexture();

    return frame;
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

void Renderer::RenderFinishedAlbedoToTexture(const IScene& scene)
{
    auto& terrain = scene.Terrain();

    glBindFramebuffer(GL_FRAMEBUFFER, bakeAlbedoFrameBuffer.fbo);
    glViewport(0, 0, bakeAlbedoFrameBuffer.width, bakeAlbedoFrameBuffer.height);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float size = 50.0f;
    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 200, 0),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, 1)
    );

    glm::mat4 projection = glm::ortho(
        -size, size,
        -size, size,
        0.1f, 500.0f
    );

    bakeAlbedoShader->Activate();
    bakeAlbedoShader->setUniformMat("model", glm::mat4(1.0f));
    bakeAlbedoShader->setUniformMat("view", view);
    bakeAlbedoShader->setUniformMat("projection", projection);
    
    terrain.getGrassMaterial().albedo->Bind(0);
    bakeAlbedoShader->setUniformInt("grassAlbedo", 0);

    terrain.getStoneMaterial().albedo->Bind(1);
    bakeAlbedoShader->setUniformInt("stoneAlbedo", 1);

    terrain.getMesh().Draw();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderDepthPass(const FrameData& frameData)
{
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer.fbo);
    glViewport(0, 0, shadowMapFramebuffer.width, shadowMapFramebuffer.height);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    depthShader->Activate();
    depthShader->setUniformMat("lightSpaceMatrix", frameData.lightSpaceMatrix);
    depthShader->setUniformMat("model", frameData.terrain.modelMatrix);

    frameData.terrain.terrainMesh->Draw();
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderLightPass(const FrameData& frameData)
{
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

    frameData.terrain.grass->albedo->Bind(1);
    terrainShader->setUniformInt("grassAlbedo", 1);

    frameData.terrain.stone->albedo->Bind(2);
    terrainShader->setUniformInt("stoneAlbedo", 2);

    terrainShader->setUniformMat("model", frameData.terrain.modelMatrix);
    frameData.terrain.terrainMesh->Draw();
}

void Renderer::renderSkyboxPass(const FrameData& frameData)
{
    glm::mat4 view = glm::mat4(glm::mat3(frameData.viewMatrix));

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    
    skyboxShader->Activate();
    skyboxShader->setUniformMat("view", view);
    skyboxShader->setUniformMat("projection", frameData.projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, frameData.skybox.skyboxTexture);

    glDisable(GL_CULL_FACE);
    frameData.skybox.skyboxMesh->Draw();
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowMapFramebuffer.width, shadowMapFramebuffer.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

void Renderer::createBakeAlbedoFrameBuffer(int width, int height)
{
    bakeAlbedoFrameBuffer.width = width;
    bakeAlbedoFrameBuffer.height = height;

    glGenFramebuffers(1, &bakeAlbedoFrameBuffer.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, bakeAlbedoFrameBuffer.fbo);

    glGenTextures(1, &bakeAlbedoFrameBuffer.color);
    glBindTexture(GL_TEXTURE_2D, bakeAlbedoFrameBuffer.color);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bakeAlbedoFrameBuffer.color, 0);
    glGenRenderbuffers(1, &bakeAlbedoFrameBuffer.depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, bakeAlbedoFrameBuffer.depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
    {
        std::cout << "Framebuffer is not complete!" << std::endl;
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, bakeAlbedoFrameBuffer.depthRenderBuffer);
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

unsigned int Renderer::getBakedAlbedoTexture() const
{
    return bakeAlbedoFrameBuffer.fbo;
}

int Renderer::getBakedAlbedoWidth() const
{
    return bakeAlbedoFrameBuffer.width;
}

int Renderer::getBakedAlbedoHeight() const
{
    return bakeAlbedoFrameBuffer.height;
}