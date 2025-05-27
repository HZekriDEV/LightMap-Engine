#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "gl_buffers.h"
#include "stb_image.h"
#include "shader.h"
#include "texture.h"
#include "Camera.h"
#include "Light.h"
#include "Color.h"
#include "mesh.h"
#include "model.h"
#include "Object.h"
#include "UI.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>