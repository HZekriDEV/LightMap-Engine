#include "config.h"

GLFWwindow* createWindow();

int main()
{
	GLFWwindow* window = createWindow();
	Shader shader("../OpenGL/shaders/vertex.vert", "../OpenGL/shaders/fragment.frag");

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f    // top 
	};

	float texCoords[] = {
		 0.0f, 0.0f,  // lower-left corner  
		 1.0f, 0.0f,  // lower-right corner
		 0.5f, 1.0f   // top-center corner
	};

	VAO VAO;
	VBO VBO(vertices, sizeof(vertices));
	
	VAO.Bind();
	VBO.Bind();
	VAO.LinkVBO(VBO, 0);
	
	VAO.Unbind();
	VBO.Unbind();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		shader.Activate();

		VAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	
	return 0;
}

GLFWwindow* createWindow()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cout << "GLFW failed" << std::endl;
		return NULL;
	}

	window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		return NULL;
	}

	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	return window;
}