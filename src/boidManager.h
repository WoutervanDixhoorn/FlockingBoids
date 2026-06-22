#pragma once

#include <vector>
#include <optional>

#include "physics/grid.h"

#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "ecs/registry.h"

struct FlockingSettings {
	float seperationCoefficient = 0.5f;
	float cohesionCoefficient = 0.5f;
	float alignmentCoefficient = 0.5f;

	float visibilityArea = 2.5f;

	bool drawGrid = false;
	bool drawViewFrustum = false;
	bool drawViewCircle = false;
};

class BoidManager {
public:
	BoidManager(int boidCount, float worldHeight, float worldWidth, FlockingSettings* settings);

	void Update(float deltaTime);
	void Draw();
private:
	void initializeBoidMesh();
	void spawnBoids(int boidCount);

	void updateBoids(float deltaTime);
	void updateFlocking();
	glm::vec2 calculateSeperation(const DG::Entity boid, std::vector<DG::Entity> neighbours);
	glm::vec2 calculateCohesion(const DG::Entity boid, std::vector<DG::Entity> neighbours);
	glm::vec2 calculateAlignment(const DG::Entity boid, std::vector<DG::Entity> neighbours);

	void updateGrid();

	void drawBoids();
	//void drawDebugFrustum();
	
	void getBoidNeighbours(const DG::Entity boid, std::vector<DG::Entity>& outNeighbours);

private:
	float m_worldHeight, m_worldWidth;
	FlockingSettings* m_settings;

	DG::EntityRegistry m_registry;
	DG::Grid m_grid;

	std::vector<DG::Entity> m_boids;
	
	std::vector<DG::Entity> m_neighbourBuffer;
	std::vector<DG::Entity> m_gridBuffer;

	std::optional<DG::Mesh> m_boidMesh;
};