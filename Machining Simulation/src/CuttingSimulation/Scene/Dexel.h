#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
class Dexel
{
public:
	unsigned int start;
	unsigned int end;
	glm::vec3 normal;
	glm::vec3 top_normal;
	glm::vec3 bot_normal;

	glm::vec3 color;
	glm::vec3 top_color;
	glm::vec3 bot_color;
};

