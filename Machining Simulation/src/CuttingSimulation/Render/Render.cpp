#include "Render.h"



void Render::Init(GLFWwindow* window)
{
	line_shader.CreateShaderProgram((char*)"Shaders/lines.vert", (char*)"Shaders/lines.frag");

    float vertices[] = {
    -100, -100, -100,
     100, 100, 100,
    -100, 100, -100
    };
    glGenVertexArrays(1, &VAO_line);
    glGenBuffers(1, &VBO_vert_line);

    glBindVertexArray(VAO_line);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_vert_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    camera.Init(window, glm::vec3(-100.0f, 0.0f, 3.0f), -40, -40);

}

void Render::Draw(GLFWwindow* window)
{
    CalcDeltaTime();

    camera.MoveCamera(window, deltaTime);
    camera.SetCamMatrixToShader(line_shader.ID);

    glUseProgram(line_shader.ID);
    glBindVertexArray(VAO_line); 
    glDrawArrays(GL_LINE_STRIP, 0, 3);

}

void Render::CalcDeltaTime()
{
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
