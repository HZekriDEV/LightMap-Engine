#include "config.h"

GLFWwindow* createWindow();
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

/*-------- Camera Settings --------*/
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;
float pitch = 0.0f, yaw = -90.0f;
float fov = 45.0;
bool firstMouse = true;

Camera mainCamera = Camera(cameraPos, fov, pitch, yaw);;
/*---------------------------------*/

int main()
{
	GLFWwindow* window = createWindow();

	Shader shader("../OpenGL/shaders/vertex.vert", "../OpenGL/shaders/fragment.frag");
	Shader ds("../OpenGL/shaders/default_vertex.vert", "../OpenGL/shaders/default_fragment.frag");
	Shader lightShader("../OpenGL/shaders/light_vertex.vert", "../OpenGL/shaders/light_fragment.frag");


	Texture diffuse("../OpenGL/textures/container2.png", true, true);
	Texture specular("../OpenGL/textures/container2_specular.png", true, true);

	shader.AddTexture(diffuse);
	shader.SetInt("material.diffuse", 0);
	shader.AddTexture(specular);
	shader.SetInt("material.specular", 1);
	shader.SetFloat("material.shininess", 32.0f);

	LightManager lightManager;

	DirectionalLight dirLight = DirectionalLight(glm::vec3(0.0f, 0.0f, 0.0f), Color::White(), 10.0f);
	SpotLight spotLight = SpotLight(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.0, 0.0, 0.0)-glm::vec3(-2.0f, 0.0f, 0.0f) , Color::White(), 1.0f, 12.5f, 17.5f);

	lightManager.directionalLights.push_back(dirLight);
	lightManager.spotLights.push_back(spotLight);
	
	Mesh cube("CUBE", shader);
	Mesh sphere("UV_SPHERE", lightShader);
	Model backpack("../OpenGL/backpack/backpack.obj", ds);

	sphere.SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
	cube.SetPosition(glm::vec3(1.0, 0.0, 0.0));
	cube.SetRotation(30.0, glm::vec3(0.0, 1.0, 0.0));

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*-------- Render Scene --------*/
		lightManager.ApplyLightsToShader(shader);
		
		backpack.Draw(mainCamera);

		cube.Draw(mainCamera);
		sphere.Draw(mainCamera);

		/*--------- End Render ---------*/

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

	glClearColor(0.025f, 0.025f, 0.05f, 1.0f);

	return window;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	const float cameraSpeed = 2.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mainCamera.SetPosition(mainCamera.Position() + (cameraSpeed * mainCamera.Front()));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mainCamera.SetPosition(mainCamera.Position() - (cameraSpeed * mainCamera.Front()));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mainCamera.SetPosition(mainCamera.Position() - (glm::normalize(glm::cross(mainCamera.Front(), mainCamera.Up())) * cameraSpeed));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mainCamera.SetPosition(mainCamera.Position() + (glm::normalize(glm::cross(mainCamera.Front(), mainCamera.Up())) * cameraSpeed));

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetCursorPosCallback(window, mouse_callback);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(window, NULL);
	}
	
	glfwSetScrollCallback(window, scroll_callback);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	mainCamera.SetYaw(mainCamera.Yaw() + xoffset);
	mainCamera.SetPitch(mainCamera.Pitch() + yoffset);

	if (mainCamera.Pitch() > 89.0f)
		mainCamera.SetPitch(89.0f);
	if (mainCamera.Pitch() < -89.0f)
		mainCamera.SetPitch(-89.0f);

	glm::vec3 direction;
	direction.x = cos(glm::radians(mainCamera.Yaw())) * cos(glm::radians(mainCamera.Pitch()));
	direction.y = sin(glm::radians(mainCamera.Pitch()));
	direction.z = sin(glm::radians(mainCamera.Yaw())) * cos(glm::radians(mainCamera.Pitch()));
	mainCamera.SetFrontVector(glm::normalize(direction));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mainCamera.SetFOV(mainCamera.FOV() - (float)yoffset);
	if (mainCamera.FOV() < 1.0f)
		mainCamera.SetFOV(1.0f);
	if (mainCamera.FOV() > 45.0f)
		mainCamera.SetFOV(45.0f);
}