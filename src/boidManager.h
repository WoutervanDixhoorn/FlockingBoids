#pragma once

#include <vector>
#include <optional>

#include "boid.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"

class BoidManager {
public:
	BoidManager(int boidCount, float worldHeight, float worldWidth);

	void Update(float deltaTime);
	void Draw(DG::Camera& camera);
private:
	void initializeBoidMesh();
	void spawnBoids(int boidCount);

private:
	float m_worldHeight, m_worldWidth;

	std::vector<Boid> m_boids;

	std::optional<DG::Mesh> m_boidMesh;
	DG::Shader m_boidShader;
};