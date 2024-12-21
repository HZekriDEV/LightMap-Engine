#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "shader.h"

class Light
{
	public:
		glm::vec3 color;
		float intensity;

		Light(const glm::vec3& color = glm::vec3(1.0f), float intensity = 1.0f)
			: color(color), intensity(intensity) {}

		virtual void ApplyToShader(Shader& shader, int index) const = 0;
};

class DirectionalLight : public Light
{
	public:
		glm::vec3 direction;

		DirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity)
			: Light(color, intensity), direction(direction) {}

		void ApplyToShader(Shader& shader, int index) const override
		{
			std::string prefix = "directionalLights[" + std::to_string(index) + "].";
			shader.SetVec3(prefix + "direction", direction.x, direction.y, direction.z);
			
			glm::vec3 cxi = color * intensity;
			shader.SetVec3(prefix + "color", cxi.x, cxi.y, cxi.z);
		}
};

class PointLight : public Light
{

};

class SpotLight : public Light
{

};

class LightManager
{
	public:
		std::vector<PointLight> pointLights;
		std::vector<DirectionalLight> directionalLights;
		std::vector<SpotLight> spotLights;

		void ApplyLightsToShader(Shader& shader) const
		{
			shader.Activate();

			// Apply point lights
			for (size_t i = 0; i < pointLights.size(); ++i) {
				pointLights[i].ApplyToShader(shader, static_cast<int>(i));
			}

			// Apply directional lights
			for (size_t i = 0; i < directionalLights.size(); ++i) {
				directionalLights[i].ApplyToShader(shader, static_cast<int>(i));
			}

			// Apply spotlights
			for (size_t i = 0; i < spotLights.size(); ++i) {
				spotLights[i].ApplyToShader(shader, static_cast<int>(i));
			}
		}
};

#endif