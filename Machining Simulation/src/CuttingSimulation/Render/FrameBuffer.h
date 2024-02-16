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
	FrameBuffer();
	~FrameBuffer();
	void Init();
};

