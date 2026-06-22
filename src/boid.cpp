//#include "renderer/renderer.h"
//#include "renderer/debugRenderer.h"
//
//#include "glm/glm.hpp"
//#include "glm/gtc/type_ptr.hpp"
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/rotate_vector.hpp>
//
//Boid::Boid(DG::Mesh* boidMesh, glm::vec2 pos, glm::vec2 vel)
//{
//	m_mesh = boidMesh;
//	m_position = pos;
//	m_velocity = vel;
//}
//
//void Boid::Update(float deltatime, float worldHeight, float worldWidth) {
//	m_position += m_velocity * deltatime;
//
//	if (m_position.x > worldWidth) m_position.x = 0.0f;
//	else if (m_position.x < 0.0f) m_position.x = worldWidth;
//
//	if (m_position.y > worldHeight) m_position.y = 0.0f;
//	else if (m_position.y < 0.0f) m_position.y = worldHeight;
//}
//
//void Boid::Draw() {
//	glm::mat4 transform = glm::mat4(1.0f);
//	transform = glm::translate(transform, glm::vec3(m_position, 0.0f));
//
//	float angle = atan2(m_velocity.y, m_velocity.x) + glm::radians(90.0f);
//	transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
//
//	DG::Renderer2D::DrawMesh(m_mesh, transform);
//}
//
