#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class FrameBuffer
{
public:
	GLuint fbo;
	GLuint rbo;
	GLuint texture_id;

	float width;
	float height;

	FrameBuffer();
	~FrameBuffer();
	void Init();
	void Bind();
	void Unbind();
	void Rescale(float width, float height);
};

