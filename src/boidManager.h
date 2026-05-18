#pragma once

#include <vector>
#include <optional>

#include "boid.h"
#include "renderer/mesh.h"
#include "renderer/shader.h"

struct FlockingSettings {
	float seperationCoefficient = 0.5f;
	float cohesionCoefficient = 0.5f;
	float alignmentCoefficient = 0.5f;

	float visibilityArea = 2.5f;
};

class BoidManager {
public:
	BoidManager(int boidCount, float worldHeight, float worldWidth, FlockingSettings* settings);

	void Update(float deltaTime);
	void Draw();
private:
	void initializeBoidMesh();
	void spawnBoids(int boidCount);

	void updateFlocking();
	glm::vec2 calculateSeperation(const Boid& boid, std::vector<Boid*> neighbours);
	glm::vec2 calculateCohesion(const Boid& boid, std::vector<Boid*> neighbours);
	glm::vec2 calculateAlignment(const Boid& boid, std::vector<Boid*> neighbours);

	std::vector<Boid*> getBoidNeighbours(const Boid& boid);

private:
	float m_worldHeight, m_worldWidth;
	FlockingSettings* m_settings;

	std::vector<Boid> m_boids;
	std::vector<Boid> m_nextBoids;

	std::optional<DG::Mesh> m_boidMesh;
};