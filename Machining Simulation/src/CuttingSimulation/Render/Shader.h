#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	GLuint ID;
	Shader();
	~Shader();
	void CreateShaderProgram(char* vert, char* frag);
};

