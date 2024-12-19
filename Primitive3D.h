#ifndef PRIMITIVE3D_H
#define PRIMITIVE3D_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "shader.h"

class Primitive3D
{
	public:
		Primitive3D(const std::string& type, const Shader& shader = Shader("../OpenGL/shaders/default_vertex.vert", "../OpenGL/shaders/default_fragment.frag"));

		// Setters for transformations
		void SetPosition(const glm::vec3& position);
		void SetRotation(float angle, const glm::vec3 axis);
		void SetScale(const glm::vec3& scale);

		glm::mat4 LocalToWorldmatrix() const;

		void Draw(const Camera& camera) const;

	private:
		std::string m_Type;
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::vec3 m_RotationAxis;
		float m_RotationAngle;

		Shader m_shader;

		GLuint vaoID;
		GLuint vboID;
		GLuint eboID;
		
		unsigned int numVertices;
		
		void Initialize();
};

#endif