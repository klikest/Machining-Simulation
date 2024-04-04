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
    glGenBuffers(1, &VBO_normals);

    scene_grid = new DexelGrid;
    scene_grid->blank->CreateCylBlank(scene_grid->blank_D, scene_grid->blank_H, scene_grid->resolution);
    //scene_grid->Generate_Draw_Arrays(scene_grid->blank, scene_grid->tool);
    //blank->CreateBlankCyl(16, 60, 0.8);
    //blank->GenerateToolGrid();
    //blank->GenerateDrawArrays();

}

void Scene::Draw()
{
    int size = scene_grid->blank->Num_of_Dexels + scene_grid->tool->Num_of_Dexels;

    glUseProgram(shader.ID);
    glBindVertexArray(VAO);


    glUniform1f(glGetUniformLocation(shader.ID, "Machine_X"), scene_grid->machine_coords.X);
    glUniform1f(glGetUniformLocation(shader.ID, "Machine_offset"), scene_grid->machine_coords.offset);
    glUniform1f(glGetUniformLocation(shader.ID, "Machine_A"), scene_grid->machine_coords.A);
    glUniform1f(glGetUniformLocation(shader.ID, "Machine_C"), scene_grid->machine_coords.C);
    glUniform1i(glGetUniformLocation(shader.ID, "Machine_is_transform"), scene_grid->machine_coords.is_transform);

    GLfloat colors_to_choose[9] = {0.8, 0.8, 0.8,   0.29, 0.72, 0.33,  0.47, 0.15, 0.89 };

    glUniform3fv(glGetUniformLocation(shader.ID, "colors_choose"), 9, colors_to_choose );


    glUniform1f(glGetUniformLocation(shader.ID, "acc"), scene_grid->blank->resolution);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_main_dexel_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Cube_VertexPositions.size(), Cube_VertexPositions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_offsets);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)* size, scene_grid->dexel_draw_data, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1);


    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, scene_grid->colors_dexels, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    glVertexAttribDivisor(3, 1);


    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * size, scene_grid->normals_dexels, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(4, 1);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, size);
}

void Scene::Close()
{
    scene_grid->DeleteDrawArrays();
}