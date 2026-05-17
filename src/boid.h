#pragma once

#include "glm/glm.hpp"

#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "renderer/camera.h"

class Boid {
public:
	Boid(DG::Mesh* boidMesh, DG::Shader* drawingShader, glm::vec2 pos, glm::vec2 vel);

	void Update(float deltatime, float worldHeight, float worldWidth);
	void Draw(DG::Camera& camera);

	inline glm::vec2 GetPosition() const { return m_position; };
	inline glm::vec2 GetVelocity() const { return m_velocity; };

	void ApplyForce(glm::vec2 force);

private:
	glm::vec2 m_position;
	glm::vec2 m_velocity;

	DG::Mesh* m_mesh;
	DG::Shader* m_shader;
};