#include "Object.h"

Object::Object(const std::string name) : name(name)
{
	ID = 1;
}

void Object::SetMesh(const Mesh& mesh)
{
	m_mesh = mesh;
}
void Object::SetShader(const Shader& shader)
{
	m_shader = shader;
}
Mesh Object::GetMesh()
{
	return m_mesh;
}
void Object::UpdatePosition(const glm::vec3& position)
{
	m_mesh.SetPosition(position);
}
void Object::UpdateRotation(float angle, const glm::vec3& axis)
{
	m_mesh.SetRotation(angle, axis);
}
void Object::UpdateScale(const glm::vec3& scale)
{
	m_mesh.SetScale(scale);
}

void Object::Render(const Camera& camera)
{
	m_mesh.Draw(camera);
}