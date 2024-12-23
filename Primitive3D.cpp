#include "config.h"

Primitive3D::Primitive3D(const std::string& type, const Shader& shader)
	: m_shader(shader)  // Use member initializer list to set the shader
{
	m_Type = type;
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_RotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_RotationAngle = 0.0f;

	Initialize();
}

void Primitive3D::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}

void Primitive3D::SetRotation(float angle, const glm::vec3 axis)
{
	m_RotationAngle = angle;
	m_RotationAxis = axis;
}

void Primitive3D::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
}

glm::mat4 Primitive3D::LocalToWorldmatrix() const
{
	// For the object (transforms from local to world coordinates)
	glm::mat4 model = glm::mat4(1.0f); // Start with identity

	model = glm::translate(model, m_Position);
	model = glm::rotate(model, glm::radians(m_RotationAngle), m_RotationAxis);
	model = glm::scale(model, m_Scale);
	return model;
}

void Primitive3D::Draw(const Camera& camera) const
{
	m_shader.Activate();

	m_shader.SetVec3("viewPos", camera.Position());

	// For the object (transforms from local to world coordinates)
	glm::mat4 model = LocalToWorldmatrix();

	// For transforming vertices in world coordinates to the cameras view coordinates
	glm::mat4 view = camera.ViewMatrix();

	// For projecting the view coordinates to the frustum, which gets turned to fragments
	glm::mat4 projection = projection = glm::perspective(glm::radians(camera.FOV()), 800.0f / 600.0f, 0.1f, 100.0f);

	m_shader.SetMat4("model", model);
	m_shader.SetMat4("view", view);
	m_shader.SetMat4("projection", projection);

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Primitive3D::Initialize()
{
	if (m_Type == "CUBE")
	{	
		float vertices[] = {
			// Position            // Normal               // UV
			// Back face
			-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,    0.0f, 0.0f, // Bottom-left
			 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,    1.0f, 0.0f, // Bottom-right
			 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,    1.0f, 1.0f, // Top-right
			-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,    0.0f, 1.0f, // Top-left

			// Front face
			-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,    0.0f, 0.0f, // Bottom-left
			 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,    1.0f, 0.0f, // Bottom-right
			 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,    1.0f, 1.0f, // Top-right
			-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,    0.0f, 1.0f, // Top-left

			// Left face
			-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,    0.0f, 0.0f, // Bottom-left
			-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,    1.0f, 0.0f, // Bottom-right
			-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,    1.0f, 1.0f, // Top-right
			-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,    0.0f, 1.0f, // Top-left

			// Right face
			 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,    0.0f, 0.0f, // Bottom-left
			 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,    1.0f, 0.0f, // Bottom-right
			 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,    1.0f, 1.0f, // Top-right
			 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,    0.0f, 1.0f, // Top-left

			 // Bottom face
			 -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,    0.0f, 0.0f, // Bottom-left
			  0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f, // Bottom-right
			  0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,    1.0f, 1.0f, // Top-right
			 -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f, // Top-left

			 // Top face
			 -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,    0.0f, 0.0f, // Bottom-left
			  0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,    1.0f, 0.0f, // Bottom-right
			  0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,    1.0f, 1.0f, // Top-right
			 -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,    0.0f, 1.0f  // Top-left
		};
		unsigned int indices[] = {
			// Back face
			0, 1, 2,
			2, 3, 0,

			// Front face
			4, 5, 6,
			6, 7, 4,

			// Left face
			8, 9, 10,
			10, 11, 8,

			// Right face
			12, 13, 14,
			14, 15, 12,

			// Bottom face
			16, 17, 18,
			18, 19, 16,

			// Top face
			20, 21, 22,
			22, 23, 20
		};
		
		numVertices = 36;

		VAO VAO;
		VBO VBO(vertices, sizeof(vertices));
		EBO EBO(indices, sizeof(indices));
		vaoID = VAO.ID;
		vboID = VBO.ID;
		eboID = EBO.ID;

		VAO.Bind();
		VBO.Bind();
		VAO.LinkVBO(VBO, 0);
		EBO.Bind();

		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
	}
	else if (m_Type == "UV_SPHERE")
	{
		std::vector<float> vertices;
		std::vector<GLuint> indices;

		unsigned int stacks = 18;
		unsigned int slices = 36;
		float r = 0.5f;

		for (unsigned int i = 0; i <= stacks; ++i)
		{
			float theta = glm::pi<float>() * (float)i / (float)stacks;

			for (unsigned int j = 0; j <= slices; ++j)
			{
				float phi = 2.0f * glm::pi<float>() * (float)j / (float)slices;
				float x = r * glm::sin(theta) * glm::cos(phi);
				float y = r * glm::cos(theta);
				float z = r * glm::sin(theta) * glm::sin(phi);

				float u = (float)j / (float)slices; 
				float v = (float)i / (float)stacks;

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
				vertices.push_back(1.0f);
				vertices.push_back(1.0f);
				vertices.push_back(1.0f);
				vertices.push_back(u);
				vertices.push_back(v);
			}
		}

		for (int i = 0; i <= stacks; ++i)
		{
			for (int j = 0; j <= slices; ++j)
			{
				int first = (i * (slices + 1)) + j;
				int second = first + slices + 1;

				// Two triangles per quad
				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(first + 1);

				indices.push_back(second);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}
		numVertices = indices.size();

		VAO VAO;
		VBO VBO(&vertices[0], vertices.size() * sizeof(float));
		EBO EBO(&indices[0], indices.size() * sizeof(GLuint));
		vaoID = VAO.ID;
		vboID = VBO.ID;
		eboID = EBO.ID;

		VAO.Bind();
		VBO.Bind();
		VAO.LinkVBO(VBO, 0);
		EBO.Bind();

		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
	}
}