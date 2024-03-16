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


    blank = new DexelGrid;
    blank->CreateBlankCyl(16, 60, 0.8);
    blank->GenerateToolGrid();
    blank->GenerateDrawArrays();

}

void Scene::Draw()
{


    int size = blank->summ_num_of_dexels + blank->X_size * blank->Y_size;

    glUseProgram(shader.ID);
    glBindVertexArray(VAO);


    GLfloat colors_to_choose[9] = {0.8, 0.8, 0.8,   1, 1, 0,  1, 0, 1};

    glUniform3fv(glGetUniformLocation(shader.ID, "colors_choose"), 9, colors_to_choose );


    glUniform1f(glGetUniformLocation(shader.ID, "acc"), blank->acc);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_main_dexel_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Cube_VertexPositions.size(), Cube_VertexPositions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_offsets);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)* size, blank->dexel_draw_data, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1);


    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, blank->colors_dexels, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    glVertexAttribDivisor(3, 1);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, size);
}

void Scene::Close()
{
    blank->DeleteArrays();
}