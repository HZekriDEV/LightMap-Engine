#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Camera.h"

class Shader
{
	public:
		// Shader ID
		unsigned int ID;

		Shader(const char* vertex_filepath, const char* fragment_filepath);

		void Activate() const;
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetMat4(const std::string& name, glm::mat4) const;

	private:
		unsigned int create_module(const std::string& filepath, unsigned int module_type);
};

#endif