#pragma once

#include "glm/glm.hpp"

#include "mesh.h"
#include "shader.h"
#include "camera.h"

class Boid {
public:
	Boid(DG::Mesh* boidMesh, DG::Shader* drawingShader, glm::vec2 pos, glm::vec2 vel, float visibilityRadius);

	void Update(float deltatime, float worldHeight, float worldWidth);
	void Draw(DG::Camera& camera);

private:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_visibilityRadius;

	DG::Mesh* m_mesh;
	DG::Shader* m_shader;
};