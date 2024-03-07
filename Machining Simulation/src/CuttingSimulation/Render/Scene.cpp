#include "Scene.h"

Scene::Scene()
{
}

void Scene::Init()
{

    shader.CreateShaderProgram((char*)"Shaders/dexel.vert", (char*)"Shaders/dexel.frag");

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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_main_dexel_vertices);
    glGenBuffers(1, &VBO_offsets);
    glGenBuffers(1, &VBO_colors);

}

void Scene::Draw()
{

    std::vector<glm::vec3> dexel_data = { glm::vec3(0, 10, 1), glm::vec3(1, 10, 2), glm::vec3(2, 10, 3), glm::vec3(3, 10, 1) };


    glUseProgram(shader.ID);
    glBindVertexArray(VAO);


    GLuint x_size_loc = glGetUniformLocation(shader.ID, "x_size");
    glUniform1f(x_size_loc, 2.0f);

    GLuint y_size_loc = glGetUniformLocation(shader.ID, "y_size");
    glUniform1f(y_size_loc, 2.0f);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_main_dexel_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Cube_VertexPositions.size(), Cube_VertexPositions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_offsets);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * dexel_data.size(), dexel_data.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1);


    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 4);
}
