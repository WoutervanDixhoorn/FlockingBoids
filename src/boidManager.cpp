#include "boidManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cmath>
#include <random>
#include <print>

static float boidVertices[8] = {
   -0.5f,  0.5f,
    0.0f,  0.3f,
    0.5f,  0.5f,

    0.0f, -0.5f
};

static uint32_t boidIndices[6] = {
    0, 1, 3,

    2, 1, 3
};

BoidManager::BoidManager(int boidCount, float worldHeight, float worldWidth) 
    : m_boidShader(DG::Shader::LoadFromFile("assets/shaders/basic.glsl")), m_worldHeight(worldHeight), m_worldWidth(worldWidth)
{
    initializeBoidMesh();
	spawnBoids(boidCount);
}

void BoidManager::Update(float deltaTime) {
    for (auto& b : m_boids) {
        b.Update(deltaTime, m_worldHeight, m_worldWidth);
    }
}

void BoidManager::Draw(DG::Camera& camera) {
    //NOTE: Setting uniforms automaticallty binds the shader too
    m_boidShader.SetMat4("camera", camera.GetCameraMatrix());
    
    for (auto& b : m_boids) {
        b.Draw(camera);
    }
}

void BoidManager::initializeBoidMesh() {
    DG::VertexLayout layout;
    layout.AddAttribute(DG::FLOAT_ATTRIB, 2);

    DG::MeshData meshData {
       .vertexData = boidVertices,
       .vertexSize = 2 * sizeof(float),
       .vertexCount = 4,

       .indexData = boidIndices,
       .indexCount = sizeof(boidIndices),

       .vertexLayout = layout
    };

    m_boidMesh = DG::Mesh::Create(meshData);
    if (!m_boidMesh) {
        std::println(stderr, "Creating boid mesh failed...");
    }
}

void BoidManager::spawnBoids(int boidCount) {
    m_boids.reserve(boidCount);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> posDist(-20.0f, 20.0f);
    std::uniform_real_distribution<float> angleDist(0.0f, glm::two_pi<float>());

    float speed = 15.0f;

    for (int i = 0; i < boidCount; ++i) {
        glm::vec2 pos(posDist(gen), posDist(gen));
        float angle = angleDist(gen);

        glm::vec2 vel(cos(angle) * speed, sin(angle) * speed);

        m_boids.push_back({ &m_boidMesh.value(), &m_boidShader, pos, vel, 2.0f});
    }
}