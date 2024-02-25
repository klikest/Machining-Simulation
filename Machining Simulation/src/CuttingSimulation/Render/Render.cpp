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
    dexel_shader.CreateShaderProgram((char*)"Shaders/dexel.vert", (char*)"Shaders/dexel.frag");

    Cube_VertexPositions = {
    0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
     1.0f, 0.0f, 0.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
     1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  1.0f,  1.0f,  1.0f, 0.0f,
     1.0f, 0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
     1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    0.0f, 0.0f,  1.0f, 0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
    0.0f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f,  1.0f,
    0.0f, 0.0f,  1.0f,  1.0f, 0.0f,  1.0f,  1.0f, 0.0f, 0.0f,
     1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f,
    0.0f,  1.0f, 0.0f,  1.0f,  1.0f, 0.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f, 0.0f,  1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAO_line);
    glGenBuffers(1, &VBO_vert_line);
    glGenBuffers(1, &VBO_color_line);


    glGenVertexArrays(1, &VAO_dexel);
    glGenBuffers(1, &VBO_vert_cube_for_dexels);
    glGenBuffers(1, &VBO_offsets_dexel);
    glGenBuffers(1, &VBO_color_dexel);


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


void Render::AddRectangle(glm::vec3 min_rect, glm::vec3 max_rect, glm::vec3 color)
{
    std::vector<float>vertices = {
        min_rect.x, min_rect.y, min_rect.z,
        max_rect.x, min_rect.y, min_rect.z,
        min_rect.x, min_rect.y, min_rect.z,
        min_rect.x, max_rect.y, min_rect.z,
        min_rect.x, min_rect.y, min_rect.z,
        min_rect.x, min_rect.y, max_rect.z,
        max_rect.x, max_rect.y, max_rect.z,
        min_rect.x, max_rect.y, max_rect.z,
        max_rect.x, max_rect.y, max_rect.z,
        max_rect.x, min_rect.y, max_rect.z,
        max_rect.x, max_rect.y, max_rect.z,
        max_rect.x, max_rect.y, min_rect.z,
        max_rect.x, min_rect.y, min_rect.z,
        max_rect.x, min_rect.y, max_rect.z,
        min_rect.x, min_rect.y, max_rect.z,
        max_rect.x, min_rect.y, max_rect.x,
        min_rect.x, max_rect.y, min_rect.z,
        max_rect.x, max_rect.y, min_rect.z,
        min_rect.z, max_rect.y, min_rect.z,
        min_rect.z, max_rect.y, max_rect.z,
        min_rect.x, min_rect.y, max_rect.z,
        min_rect.x, max_rect.y, max_rect.z,
        max_rect.z, min_rect.y, min_rect.z,
        max_rect.z, max_rect.y, min_rect.z
    };
    std::vector<float>colors = {
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z
    };

    line_vertices.insert(line_vertices.end(), vertices.begin(), vertices.end());
    line_colors.insert(line_colors.end(), colors.begin(), colors.end());
}




void Render::DrawScene(DexelScene* scene)
{
    glUseProgram(dexel_shader.ID);
    glBindVertexArray(VAO_dexel);


    GLuint x_size_loc = glGetUniformLocation(dexel_shader.ID, "x_size");
    glUniform1f(x_size_loc, 2.0f);

    GLuint y_size_loc = glGetUniformLocation(dexel_shader.ID, "y_size");
    glUniform1f(y_size_loc, 2.0f);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vert_cube_for_dexels);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Cube_VertexPositions.size(), Cube_VertexPositions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    std::vector<glm::vec3> dexel_data = { glm::vec3(0, 10, 1), glm::vec3(1, 10, 2), glm::vec3(2, 10, 3), glm::vec3(3, 10, 1) };

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_offsets_dexel);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * dexel_data.size(), dexel_data.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1);


    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 4);

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

void Render::Draw(GLFWwindow* window, DexelScene* scene, float aspect)
{
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    CalcDeltaTime();
    camera.UpdateAspectRate(aspect);
    camera.MoveCamera(window, deltaTime);
    camera.SetCamMatrixToShader(line_shader.ID);
    

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);




    AddCoords(glm::vec3(0, 0, 0));
    AddLines({ 0, 0, 0, 10, 10, 10 }, { 1, 1, 1, 1, 0.5, 0.5 });

    AddRectangle(glm::vec3(-10, -10, -10), glm::vec3(10, 10, 10), glm::vec3(1, 1, 0));
    DrawLines();

    camera.SetCamMatrixToShader(dexel_shader.ID);
    //DrawScene(scene);
}

void Render::CalcDeltaTime()
{
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
