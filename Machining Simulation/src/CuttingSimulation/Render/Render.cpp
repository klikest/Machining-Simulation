#include "Render.h"



Render::Render()
{

}


Render::~Render()
{
}

void Render::Init(GLFWwindow* window)
{
	line_shader.CreateShaderProgram((char*)"Shaders/lines.vert", (char*)"Shaders/lines.frag");
   
    float vertices[] = {
    0, 0, 0,
    10, 0, 0,

    0, 0, 0,
    0, 10, 0,

    0, 0, 0,
    0, 0, 10
    };


    float colors[] = {
    1, 0, 0,
    1, 0, 0,

    0, 1, 0,
    0, 1, 0,

    0, 0, 1,
    0, 0, 1,
        };

    glGenVertexArrays(1, &VAO_line);
    glGenBuffers(1, &VBO_vert_line);
    glGenBuffers(1, &VBO_color_line);

    glBindVertexArray(VAO_line);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vert_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    camera.Init(window, glm::vec3(-51.0f, 19.0f, 31.0f), -25, -13);

}

void Render::AddCoords(glm::vec3 Pos)
{
    float offset = 10;

    std::vector<float> vertices = {
    Pos.x, Pos.y, Pos.z,
    Pos.x + offset, Pos.y, Pos.z,

    Pos.x, Pos.y, Pos.z,
    Pos.x, Pos.y + offset, Pos.z,

    Pos.x, Pos.y, Pos.z,
    Pos.x, Pos.y, Pos.z + offset,
    };


    std::vector<float> colors = {
    1, 0, 0,
    1, 0, 0,

    0, 1, 0,
    0, 1, 0,

    0, 0, 1,
    0, 0, 1,
    };

    line_vertices.insert(line_vertices.end(), vertices.begin(), vertices.end());
    line_colors.insert(line_colors.end(), colors.begin(), colors.end());

}

void Render::AddLines(std::vector<float> vertices, std::vector<float> colors)
{
    line_vertices.insert(line_vertices.end(), vertices.begin(), vertices.end());
    line_colors.insert(line_colors.end(), colors.begin(), colors.end());
}

void Render::DrawLines()
{
    glUseProgram(line_shader.ID);
    glBindVertexArray(VAO_line);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vert_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * line_vertices.size(), line_vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * line_colors.size(), line_colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnable(GL_LINE_SMOOTH);
    glDrawArrays(GL_LINES, 0, line_colors.size()*3);

    line_vertices.clear();
    line_colors.clear();

}

void Render::Draw(GLFWwindow* window)
{
    CalcDeltaTime();

    camera.MoveCamera(window, deltaTime);
    camera.SetCamMatrixToShader(line_shader.ID);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    AddCoords(glm::vec3(0, 0, 0));
    AddLines({ 0, 0, 0, 10, 10, 10 }, { 1, 1, 1, 1, 0.5, 0.5 });
    DrawLines();
}

void Render::CalcDeltaTime()
{
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
