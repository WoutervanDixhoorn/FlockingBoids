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

BoidManager::BoidManager(int boidCount, float worldHeight, float worldWidth, FlockingSettings* settings) 
    : m_boidShader(DG::Shader::LoadFromFile("assets/shaders/basic.glsl")), m_worldHeight(worldHeight), m_worldWidth(worldWidth), m_settings(settings)
{
    initializeBoidMesh();
	spawnBoids(boidCount);
}

void BoidManager::Update(float deltaTime) {
    updateFlocking();

    for (auto& b : m_boids) {
        b.Update(deltaTime, m_worldHeight, m_worldWidth);
    }
}

void BoidManager::Draw(DG::Camera& camera) {
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

    float speed = 8.0f;

    for (int i = 0; i < boidCount; ++i) {
        glm::vec2 pos(posDist(gen), posDist(gen));
        float angle = angleDist(gen);

        glm::vec2 vel(cos(angle) * speed, sin(angle) * speed);

        m_boids.push_back({ &m_boidMesh.value(), &m_boidShader, pos, vel});
    }
}

std::vector<Boid*> BoidManager::getBoidNeighbours(const Boid& boid)
{
    std::vector<Boid*> neighbours;

    for (auto &b : m_boids) {
        if (&b == &boid) continue;

        float distance = glm::distance(b.GetPosition(), boid.GetPosition());
        if (distance <= m_settings->visibilityArea) {
            neighbours.push_back(&b);
        }
    }

    return neighbours;
}

void BoidManager::updateFlocking()
{
    //Copy currentBoids into the nextBoids. m_boids is what we draw, nextBoids is what we mutate
    m_nextBoids = m_boids;

    for (size_t i = 0; i < m_boids.size(); ++i) {
        auto& b_mutate = m_nextBoids[i];
        const auto& b_read = m_boids[i];

        auto neighbours = getBoidNeighbours(b_read);

        glm::vec2 seperation = calculateSeperation(b_read, neighbours);
        glm::vec2 cohesion   = calculateCohesion(b_read, neighbours);
        glm::vec2 alignment  = calculateAlignment(b_read, neighbours);

        glm::vec2 steer{ 0.0f }; 
        steer += (m_settings->seperationCoefficient * seperation);
        steer += (m_settings->cohesionCoefficient * cohesion);
        steer += (m_settings->alignmentCoefficient * alignment);

        b_mutate.ApplyForce(steer);
    }

    //Swap!!
    m_boids = m_nextBoids;
}

glm::vec2 BoidManager::calculateSeperation(const Boid& boid, std::vector<Boid*> neighbours) {
    if (neighbours.empty()) return glm::vec2{ 0.0f };

    glm::vec2 negativeSum{ 0.0f };
    for (auto b : neighbours) {
        negativeSum += (boid.GetPosition() - b->GetPosition());
    }

    return negativeSum;
}

glm::vec2 BoidManager::calculateCohesion(const Boid& boid, std::vector<Boid*> neighbours) {
    if (neighbours.empty()) return glm::vec2{ 0.0f };
    
    glm::vec2 sumPos{0.0f};
    for (auto b : neighbours) {
        sumPos += b->GetPosition();
    }

    glm::vec2 center = sumPos / static_cast<float>(neighbours.size());

    return(center - boid.GetPosition());
}

glm::vec2 BoidManager::calculateAlignment(const Boid& boid, std::vector<Boid*> neighbours) {
    if (neighbours.empty()) return glm::vec2{ 0.0f };

    glm::vec2 sumVel{ 0.0f };
    for (auto b : neighbours) {
        sumVel += b->GetVelocity();
    }

    glm::vec2 averageVelocity = sumVel/ static_cast<float>(neighbours.size());

    return averageVelocity;
}