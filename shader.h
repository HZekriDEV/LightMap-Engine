#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
	public:
		// Shader ID
		unsigned int ID;

		Shader(const char* vertex_filepath, const char* fragment_filepath);

		void Activate();
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;

	private:
		unsigned int create_module(const std::string& filepath, unsigned int module_type);
};

#endif