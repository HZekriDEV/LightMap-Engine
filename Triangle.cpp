#include "config.h"

class Triangle
{
	private:
		float vertices[9];
		unsigned int VAO, VBO;

	public:
		Triangle(std::vector<float> a, std::vector<float> b, std::vector<float> c)
		{
			for(int i = 0; i < 3; i++)
				vertices[i] = a[i];

			for (int i = 0; i < 3; i++)
				vertices[i+3] = b[i];

			for (int i = 0; i < 3; i++)
				vertices[i+6] = c[i];

			// Create vertex array object and vertex buffer object
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			// Bind the vertex array object first
			glBindVertexArray(VAO);

			// Next bind the set vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// Then configure vertex attributes
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);


			// Unbind the VAO and VBO
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void draw()
		{
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
};