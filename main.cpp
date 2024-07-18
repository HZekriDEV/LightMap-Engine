#include "config.h"
#include "Triangle.cpp"

GLFWwindow* createWindow();
unsigned int create_module(const std::string& filepath, unsigned int module_type);
unsigned int create_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);


int main()
{
	GLFWwindow* window = createWindow();
	unsigned int orange_shader = create_shader("../OpenGL/shaders/vertex.txt", "../OpenGL/shaders/fragment.txt");
	unsigned int blue_shader = create_shader("../OpenGL/shaders/vertex.txt", "../OpenGL/shaders/fragment2.txt");

	std::vector<float> pointA = { 0.5f,  0.5f, 0.0f };
	std::vector<float> pointB = { 0.5f, -0.5f, 0.0f };
	std::vector<float> pointC = { -0.5f,  0.5f, 0.0f };

	std::vector<float> pointD = { 0.5f, -0.5f, 0.0f };
	std::vector<float> pointE = { -0.5f, -0.5f, 0.0f };
	std::vector<float> pointF = { -0.5f, 0.5f, 0.0f };

	Triangle* triangleA = new Triangle(pointA, pointB, pointC);
	Triangle* triangleB = new Triangle(pointD, pointE, pointF);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(blue_shader);
		triangleA->draw();

		glUseProgram(orange_shader);
		triangleB->draw();

		glfwSwapBuffers(window);
	}


	glDeleteProgram(blue_shader);
	glDeleteProgram(orange_shader);
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

unsigned int create_shader(const std::string& vertex_filepath, const std::string& fragment_filepath)
{
	std::vector<unsigned int> modules;
	modules.push_back(create_module(vertex_filepath, GL_VERTEX_SHADER));
	modules.push_back(create_module(fragment_filepath, GL_FRAGMENT_SHADER));

	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules)
	{
		glAttachShader(shader, shaderModule);
	}

	glLinkProgram(shader);

	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking failed: \n" << errorLog << std::endl;
	}

	for (unsigned int shaderModule : modules)
	{
		glDeleteShader(shaderModule);
	}

	return shader;
}

unsigned int create_module(const std::string& filepath, unsigned int module_type)
{
	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filepath);
	while (std::getline(file, line))
	{
		bufferedLines << line << "\n";
	}

	std::string shaderSource = bufferedLines.str();
	const char* shaderSrc = shaderSource.c_str();
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
		std::cout << "Shader Module compilation failed: \n" << errorLog << std::endl;
	}

	return shaderModule;
}

