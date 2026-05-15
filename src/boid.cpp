#include "boid.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Boid::Boid(DG::Mesh* boidMesh, DG::Shader* drawingShader, glm::vec2 pos, glm::vec2 vel)
{
	m_mesh = boidMesh;
	m_shader = drawingShader;
	m_position = pos;
	m_velocity = vel;
}

void Boid::Update(float deltatime, float worldHeight, float worldWidth) {
	m_position += m_velocity * deltatime;

	if (m_position.x > worldWidth) m_position.x = -worldWidth;
	else if (m_position.x < -worldWidth) m_position.x = worldWidth;

	if (m_position.y > worldHeight) m_position.y = -worldHeight;
	else if (m_position.y < -worldHeight) m_position.y = worldHeight;
}

void Boid::Draw(DG::Camera& camera) {
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(m_position, 0.0f));

	float angle = atan2(m_velocity.y, m_velocity.x) + glm::radians(90.0f);
	transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));

	m_shader->SetMat4("transform", transform);
	m_mesh->Draw();
}

void Boid::ApplyForce(glm::vec2 force) {
	m_velocity += force;

	float speed = glm::length(m_velocity);
	if (speed > 10.0f) { //TODO: Change for maxSpeed
		m_velocity = (m_velocity / speed) * 10.0f;
	}
}