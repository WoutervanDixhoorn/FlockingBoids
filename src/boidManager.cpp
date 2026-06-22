#include "boidManager.h"

#include <cmath>
#include <random>
#include <print>

#include "renderer/rendererTypes.h"
#include "renderer/renderer.h"
#include "renderer/debugRenderer.h"
#include "ecs/components.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

static std::vector<DG::Vertex> boidVertices = {
   {{ -0.5f,  0.5f }},
   {{  0.0f,  0.3f }},
   {{  0.5f,  0.5f }},
   {{  0.0f, -0.5f }},
};

static std::vector<uint32_t> boidIndices = {
    0, 1, 3,
    1, 2, 3
};

BoidManager::BoidManager(int boidCount, float worldHeight, float worldWidth, FlockingSettings* settings)
    : m_worldHeight(worldHeight), m_worldWidth(worldWidth), m_settings(settings), m_grid(worldWidth, worldHeight, m_settings->visibilityArea)
{
    initializeBoidMesh();
	spawnBoids(boidCount);
}

void BoidManager::Update(float deltaTime) {
    updateGrid();
    updateFlocking();
    updateBoids(deltaTime);
}
void BoidManager::Draw() { 
    drawBoids();

    if (m_settings->drawGrid) {
        m_grid.DrawDebugLines();
    }
}

void BoidManager::drawBoids() 
{
    for (auto b : m_boids) {
        glm::vec2 boidPos = m_registry.Get<DG::TransformComponent>(b).position;
        glm::vec2& boidVel = m_registry.Get<DG::VelocityComponent>(b).velocity;

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(boidPos, 0.0f));

        float angle = atan2(boidVel.y, boidVel.x) + glm::radians(90.0f);
        transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));

        m_registry.Get<DG::MeshRenderComponent>(b).Draw(transform);

        if (m_settings->drawViewCircle) {
            DG::DebugRenderer::DrawCircle(boidPos, m_settings->visibilityArea / 2);
        }

        //NOTE: Maybe we want to add this back! We can draw if we like but the viewFrustum feature is not used at this moment
        /* if (m_settings->drawViewFrustum) {
             drawViewFrustum(90.0f, m_settings->visibilityArea/2.0f);
         }
         */
    }
}

//void BoidManager::drawDebugFrustum(float viewAngle, float viewRange) {
//	auto normalizedVelocity = glm::normalize(m_velocity);
//	auto halfAngeRad = glm::radians(viewAngle / 2.0f);
//	
//	glm::vec2 p1 = m_position + (glm::rotate(normalizedVelocity, halfAngeRad) * viewRange);
//	glm::vec2 p2 = m_position + (glm::rotate(normalizedVelocity, -halfAngeRad) * viewRange);
//
//	DG::DebugRenderer::DrawLine(m_position, p1);
//	DG::DebugRenderer::DrawLine(m_position, p2);
//}

void BoidManager::initializeBoidMesh() {
    DG::MeshData meshData{
        .Vertices = boidVertices,
        .Indices = boidIndices
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

    std::uniform_real_distribution<float> posDist(0.0f, m_worldWidth);
    std::uniform_real_distribution<float> angleDist(0.0f, glm::two_pi<float>());

    float speed = 8.0f;

    for (int i = 0; i < boidCount; ++i) {
        glm::vec2 pos(posDist(gen), posDist(gen));

        float angle = angleDist(gen);
        glm::vec2 vel(cos(angle) * speed, sin(angle) * speed);
        
        DG::Entity boidEntity = m_registry.CreateEntity();
        m_registry.Register<DG::TransformComponent>(boidEntity,  {pos});
        m_registry.Register<DG::VelocityComponent>(boidEntity,   {vel});
        m_registry.Register<DG::MeshRenderComponent>(boidEntity, {&m_boidMesh.value()});
        m_registry.Register<DG::AccelerationComponent>(boidEntity, { {0.0f, 0.0f} });

        m_boids.push_back(boidEntity);
    }
}

void BoidManager::getBoidNeighbours(const DG::Entity boid, std::vector<DG::Entity>& outNeighbours) {
    outNeighbours.clear();
    m_gridBuffer.clear();

    glm::vec2& boidPos = m_registry.Get<DG::TransformComponent>(boid).position;
    m_grid.GetCellInhabitants(boidPos, m_settings->visibilityArea, m_gridBuffer);

    glm::vec2 heading = glm::normalize(m_registry.Get<DG::VelocityComponent>(boid).velocity);

    for (auto b : m_gridBuffer) {
        if (b == boid) continue; 
        
        glm::vec2 toNeighbour = m_registry.Get<DG::TransformComponent>(b).position - boidPos;

        float distanceSq = glm::length2(toNeighbour);
        float viewAreaSq = m_settings->visibilityArea * m_settings->visibilityArea;
        if (distanceSq <= viewAreaSq) {
            outNeighbours.push_back(b);
        }
    }
}

void BoidManager::updateBoids(float deltaTime)
{
    for (DG::Entity b : m_boids) {
        glm::vec2& pos = m_registry.Get<DG::TransformComponent>(b).position;
        glm::vec2& vel = m_registry.Get<DG::VelocityComponent>(b).velocity;
        glm::vec2& acc = m_registry.Get<DG::AccelerationComponent>(b).acceleration;

        vel += acc;

        // TODO: Snelheidslimiet
        float speed = glm::length(vel);
        if (speed > 10.0f) {
            vel = (vel / speed) * 10.0f;
        }

        pos += vel * deltaTime;
        acc = { 0.0f, 0.0f };

        // Screen Wrap
        if (pos.x > m_worldWidth) pos.x = 0.0f;
        else if (pos.x < 0.0f) pos.x = m_worldWidth;

        if (pos.y > m_worldHeight) pos.y = 0.0f;
        else if (pos.y < 0.0f) pos.y = m_worldHeight;
    }
}

void BoidManager::updateGrid() {
    m_grid.Clear();

    for (auto b : m_boids) {
        m_grid.AddCellInhabitant(b, m_registry.Get<DG::TransformComponent>(b).position);
    }
}

void BoidManager::updateFlocking()
{
    for (DG::Entity b : m_boids) {
        glm::vec2 myPos = m_registry.Get<DG::TransformComponent>(b).position;

        getBoidNeighbours(b, m_neighbourBuffer);

        glm::vec2 seperation = calculateSeperation(b, m_neighbourBuffer);
        glm::vec2 cohesion = calculateCohesion(b, m_neighbourBuffer);
        glm::vec2 alignment = calculateAlignment(b, m_neighbourBuffer);

        glm::vec2 steer{ 0.0f };
        steer += (m_settings->seperationCoefficient * seperation);
        steer += (m_settings->cohesionCoefficient * cohesion);
        steer += (m_settings->alignmentCoefficient * alignment);

        m_registry.Get<DG::AccelerationComponent>(b).acceleration += steer;
    }
}

glm::vec2 BoidManager::calculateSeperation(const DG::Entity boid, std::vector<DG::Entity> neighbours) {
    if (neighbours.empty()) return glm::vec2{ 0.0f };

    glm::vec2 negativeSum{ 0.0f };
    glm::vec2 myPos = m_registry.Get<DG::TransformComponent>(boid).position;
    for (auto b : neighbours) {
        negativeSum += (myPos - m_registry.Get<DG::TransformComponent>(b).position);
    }

    return negativeSum;
}

glm::vec2 BoidManager::calculateCohesion(const DG::Entity boid, std::vector<DG::Entity> neighbours) {
    if (neighbours.empty()) return glm::vec2{ 0.0f };
    
    glm::vec2 sumPos{0.0f};
    for (auto b : neighbours) {
        sumPos += m_registry.Get<DG::TransformComponent>(b).position;
    }

    glm::vec2 center = sumPos / static_cast<float>(neighbours.size());

    return(center - m_registry.Get<DG::TransformComponent>(boid).position);
}

glm::vec2 BoidManager::calculateAlignment(const DG::Entity boid, std::vector<DG::Entity> neighbours) {
    if (neighbours.empty()) return glm::vec2{ 0.0f };

    glm::vec2 sumVel{ 0.0f };
    for (auto b : neighbours) {
        sumVel += m_registry.Get<DG::VelocityComponent>(b).velocity;
    }

    glm::vec2 averageVelocity = sumVel/ static_cast<float>(neighbours.size());

    return averageVelocity;
}