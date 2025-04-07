#include "config.h"

void PollEvents();
void ProcessInput(GLFWwindow* window);
void CursorCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void RenderScene();

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

Camera mainCamera = Camera(cameraPos, fov, pitch, yaw);
/*---------------------------------*/

int main()
{
	GLFWwindow* window = UI::CreateWindow(1280, 720, "SGE");
	UI::InitImGui(window);

	#pragma region INITIALIZE SCENE
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
	//SpotLight spotLight = SpotLight(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.0, 0.0, 0.0)-glm::vec3(-2.0f, 0.0f, 0.0f) , Color::White(), 1.0f, 12.5f, 17.5f);

	lightManager.directionalLights.push_back(dirLight);
	//lightManager.spotLights.push_back(spotLight);
	
	Mesh cube("CUBE", ds);
	Mesh sphere("UV_SPHERE", ds);
	//Model backpack("../OpenGL/assets/backpack/backpack.obj", ds);
	Model dragon("../OpenGL/assets/dragon.obj", ds);


	sphere.SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
	cube.SetPosition(glm::vec3(1.0, 0.0, 0.0));
	cube.SetRotation(30.0, glm::vec3(0.0, 1.0, 0.0));

	#pragma endregion  

	while (!glfwWindowShouldClose(window))
	{
		PollEvents();

		UI::BeginFrame();

		ProcessInput(window);

		UI::RenderUI();	// Render ImGui windows
		
		glEnable(GL_DEPTH_TEST);
		/*-------- Render Scene --------*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightManager.ApplyLightsToShader(shader);
		
		//dragon.Draw(mainCamera);

		//cube.Draw(mainCamera);
		//sphere.Draw(mainCamera);
		for (int i = 0; i < UI::sceneObjects.size(); ++i)
		{
			UI::sceneObjects[i]->Render(mainCamera);
		}

		/*--------- End Render ---------*/
		UI::EndFrame();


		glfwSwapBuffers(window);
	}

	UI::Cleanup(window);
	return 0;
}

void PollEvents()
{
	glfwPollEvents();

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void RenderScene()
{
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



}

void ProcessInput(GLFWwindow* window)
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
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwGetFramebufferSize(window, &mainCamera.screenWidth, &mainCamera.screenHeight);
	glfwSetCursorPosCallback(window, CursorCallback);
	glfwSetScrollCallback(window, ScrollCallback);
}

void CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	// Forward event to ImGui's handler
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;  // Let ImGui handle it
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
	}
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Forward event to ImGui's handler
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;  // Let ImGui handle it

	mainCamera.SetFOV(mainCamera.FOV() - (float)yoffset);
	if (mainCamera.FOV() < 1.0f)
		mainCamera.SetFOV(1.0f);
	if (mainCamera.FOV() > 45.0f)
		mainCamera.SetFOV(45.0f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}