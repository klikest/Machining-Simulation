#include "Dexel.h"

Dexel::Dexel()
{
}

Dexel::Dexel(float start_, float end_, float color_)
{
	start = start_;
	end = end_;
	color = color_;
}

Dexel::Dexel(float start_, float end_, float color_, glm::vec3 normal_)
{
	start = start_;
	end = end_;
	color = color_;
	normal = normal_;
}

Dexel::~Dexel()
{
}
