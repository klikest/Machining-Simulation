#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
class Dexel
{
public:

	Dexel();
	Dexel(float start_, float end_, float color_);
	Dexel(float start_, float end_, float color_, glm::vec3 normal_);
	~Dexel();

	float start = 0;
	float end = 0;
	float color = 0;

	glm::vec3 normal = glm::vec3(0.f,0.f,0.f);
};

