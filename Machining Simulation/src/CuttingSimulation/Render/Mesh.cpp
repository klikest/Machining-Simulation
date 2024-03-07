#include "Mesh.h"

Mesh::Mesh()
{
}

void Mesh::Init()
{
    shader.CreateShaderProgram((char*)"Shaders/lines.vert", (char*)"Shaders/lines.frag");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_vertices);
    glGenBuffers(1, &VBO_colors);


}


void Mesh::ReadMeshSTL(std::string stl_file_name)
{
    vertices.clear();
    colors.clear();

    std::replace(stl_file_name.begin(), stl_file_name.end(), L'\\', L'/');

    auto info = stl::parse_stl(stl_file_name);

    std::vector<stl::triangle> triangles = info.triangles;

    for (int i = 0; i < info.triangles.size(); i++)
    {
        vertices.push_back((float)info.triangles[i].v1.x);
        vertices.push_back((float)info.triangles[i].v1.y);
        vertices.push_back((float)info.triangles[i].v1.z);

        vertices.push_back((float)info.triangles[i].v3.x);
        vertices.push_back((float)info.triangles[i].v3.y);
        vertices.push_back((float)info.triangles[i].v3.z);

        vertices.push_back((float)info.triangles[i].v2.x);
        vertices.push_back((float)info.triangles[i].v2.y);
        vertices.push_back((float)info.triangles[i].v2.z);


        colors.push_back(1);
        colors.push_back(0);
        colors.push_back(0);

        colors.push_back(0);
        colors.push_back(1);
        colors.push_back(0);

        colors.push_back(0);
        colors.push_back(0);
        colors.push_back(1);
    }

}

void Mesh::DrawMesh()
{
    glUseProgram(shader.ID);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);

}