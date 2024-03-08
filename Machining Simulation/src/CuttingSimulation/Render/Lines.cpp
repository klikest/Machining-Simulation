#include "Lines.h"

Lines::Lines()
{

}

void Lines::Init()
{
    shader.CreateShaderProgram((char*)"Shaders/lines.vert", (char*)"Shaders/lines.frag");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_vertices);
    glGenBuffers(1, &VBO_colors);
}


void Lines::AddLines(std::vector<float> new_vertices, std::vector<float> new_colors)
{
    vertices.insert(vertices.end(), new_vertices.begin(), new_vertices.end());
    colors.insert(colors.end(), new_colors.begin(), new_colors.end());
}

void Lines::AddRectangle(glm::vec3 min_rect, glm::vec3 max_rect, glm::vec3 color)
{
    std::vector<float>rect_vertices = {
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
    std::vector<float>rect_colors = {
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

    vertices.insert(vertices.end(), rect_vertices.begin(), rect_vertices.end());
    colors.insert(colors.end(), rect_colors.begin(), rect_colors.end());
}

void Lines::AddCoords(glm::vec3 Pos)
{
    float offset = 10;

    std::vector<float> cords_vertices = {
    Pos.x, Pos.y, Pos.z,
    Pos.x + offset, Pos.y, Pos.z,

    Pos.x, Pos.y, Pos.z,
    Pos.x, Pos.y + offset, Pos.z,

    Pos.x, Pos.y, Pos.z,
    Pos.x, Pos.y, Pos.z + offset,
    };


    std::vector<float> cords_colors = {
    1, 0, 0,
    1, 0, 0,

    0, 1, 0,
    0, 1, 0,

    0, 0, 1,
    0, 0, 1,
    };

    vertices.insert(vertices.end(), cords_vertices.begin(), cords_vertices.end());
    colors.insert(colors.end(), cords_colors.begin(), cords_colors.end());

}

void Lines::Draw()
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

    glEnable(GL_LINE_SMOOTH);
    glDrawArrays(GL_LINES, 0, vertices.size() * 3);

    vertices.clear();
    colors.clear();

}