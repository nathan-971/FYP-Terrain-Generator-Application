#include "renderer/renderer.h"

#define SHADOWMAP_HEIGHT 2048
#define SHADOWMAP_WIDTH 2048

Renderer::Renderer()
	: terrainShader(),
	depthShader(), 
	skyboxShader(), 
	shadowMap(0),
	shadowMapFBO(0), 
	shadowMapHeight(SHADOWMAP_HEIGHT),
	shadowMapWidth(SHADOWMAP_WIDTH),
	orthogonalProjection(glm::ortho(-75.0f, 75.0f, -75.0f, 75.0f, 10.0f, 500.0f)) 
{ 
    terrainShader.Load("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
    depthShader.Load("assets/shaders/depthVertex.glsl", "assets/shaders/depthFragment.glsl");
    skyboxShader.Load("assets/shaders/skyboxVertex.glsl", "assets/shaders/skyboxFragment.glsl");
    generateShadowMap();
}

Renderer::~Renderer() 
{ 
    glDeleteTextures(1, &shadowMap);
    glDeleteFramebuffers(1, &shadowMapFBO);
}

void Renderer::RenderScene(Window& window, Camera& camera, Scene& scene)
{
    FrameData frameData = scene.getFrameData(camera);
    window.updateViewport(window.getWidth(), window.getHeight());
    camera.onResize(window.getWidth(), window.getHeight());

    renderDepthPass(scene, frameData);
    glViewport(0, 0, window.getWidth(), window.getHeight());
    renderLightPass(scene, frameData);

    if (!scene.getSkybox().isDisabled())
    {
        renderSkyboxPass(scene, frameData);
    }
}

void Renderer::renderDepthPass(Scene& scene, FrameData& frameData)
{
    glViewport(0, 0, shadowMapWidth, shadowMapHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    depthShader.Activate();

    glm::mat4 lightSpace = orthogonalProjection * frameData.lightSpaceMatrix;
    depthShader.setUniformMat("lightSpaceMatrix", lightSpace);

    glm::mat4 model = scene.getTerrainTransform().getMatrix();
    depthShader.setUniformMat("model", model);
    scene.getTerrainMesh().Draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderLightPass(Scene& scene, FrameData& frameData)
{
    terrainShader.Activate();
    terrainShader.setUniformMat("lightSpaceMatrix", frameData.lightSpaceMatrix);
    terrainShader.setUniformMat("view", frameData.viewMatrix);
    terrainShader.setUniformMat("projection", frameData.projectionMatrix);
    terrainShader.setUniformVec3("viewPos", frameData.cameraPosition);

    terrainShader.setUniformVec3("lightPos", frameData.lightPosition);
    terrainShader.setUniformVec3("lightColor", frameData.lightColor);
    terrainShader.setUniformFloat("ambientStrength", frameData.ambientStrength);
    terrainShader.setUniformFloat("specularStrength", frameData.specularStrength);
    terrainShader.setUniformInt("shininess", frameData.shininess);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    terrainShader.setUniformInt("shadowMap", 0);

    glm::mat4 model = scene.getTerrainTransform().getMatrix();
    terrainShader.setUniformMat("model", model);
    scene.getTerrainMesh().Draw();
}

void Renderer::renderSkyboxPass(Scene& scene, FrameData& frameData)
{
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    skyboxShader.Activate();

    glm::mat4 view = glm::lookAt(frameData.cameraPosition, frameData.cameraPosition + frameData.cameraOrientation, frameData.cameraUp);
    view = glm::mat4(glm::mat3(view));

    skyboxShader.setUniformMat("view", view);
    skyboxShader.setUniformMat("projection", frameData.projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, scene.getSkybox().getActiveTextureId());

    glDisable(GL_CULL_FACE);
    scene.getSkyboxMesh().Draw();
    glEnable(GL_CULL_FACE);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void Renderer::generateShadowMap()
{
    glGenFramebuffers(1, &shadowMapFBO);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    int swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}