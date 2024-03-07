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

    glGenVertexArrays(1, &VAO_mesh);
    glGenBuffers(1, &VBO_vert_mesh);
    glGenBuffers(1, &VBO_color_mesh);


    glGenVertexArrays(1, &VAO_dexel);
    glGenBuffers(1, &VBO_vert_cube_for_dexels);
    glGenBuffers(1, &VBO_offsets_dexel);
    glGenBuffers(1, &VBO_color_dexel);


    camera.Init(window, glm::vec3(-51.0f, 19.0f, 31.0f), -25, -13);


    ReadMesh((char*)"C:/Users/User/source/repos/Machining Simulation/Machining Simulation/Models/Box1x1x1.stl");


    lines = std::make_unique<Lines>();
    lines->Init();

}


void Render::ReadMesh(std::string stl_file_name)
{
    mesh_vertices.clear();
    mesh_colors.clear();

    std::replace(stl_file_name.begin(), stl_file_name.end(), L'\\', L'/');

    auto info = stl::parse_stl(stl_file_name);

    std::vector<stl::triangle> triangles = info.triangles;

    for (int i = 0; i < info.triangles.size(); i++)
    {
        mesh_vertices.push_back((float)info.triangles[i].v1.x);
        mesh_vertices.push_back((float)info.triangles[i].v1.y);
        mesh_vertices.push_back((float)info.triangles[i].v1.z);

        mesh_vertices.push_back((float)info.triangles[i].v3.x);
        mesh_vertices.push_back((float)info.triangles[i].v3.y);
        mesh_vertices.push_back((float)info.triangles[i].v3.z);

        mesh_vertices.push_back((float)info.triangles[i].v2.x);
        mesh_vertices.push_back((float)info.triangles[i].v2.y);
        mesh_vertices.push_back((float)info.triangles[i].v2.z);



        mesh_colors.push_back(1);
        mesh_colors.push_back(0);
        mesh_colors.push_back(0);

        mesh_colors.push_back(0);
        mesh_colors.push_back(1);
        mesh_colors.push_back(0);

        mesh_colors.push_back(0);
        mesh_colors.push_back(0);
        mesh_colors.push_back(1);
    }

}

void Render::DrawMesh(GUI* gui)
{
    glUseProgram(line_shader.ID);
    glBindVertexArray(VAO_mesh);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vert_mesh);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_vertices.size(), mesh_vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_mesh);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_colors.size(), mesh_colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, mesh_vertices.size() * 3);

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



void Render::Draw(GLFWwindow* window, DexelScene* scene, float aspect, GUI* gui)
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
   
    camera.SetCamMatrixToShader(lines->shader.ID);


    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    
    lines->AddRectangle(glm::vec3(-10, -10, -10), glm::vec3(10, 10, 10), glm::vec3(0.5, 0.8, 0.2));
    lines->AddCoords(glm::vec3(0, 0, 0));
    lines->AddLines({ 0, 0, 0, 10, 10, 10 }, { 1, 1, 1, 1, 0.5, 0.5 });
    lines->Draw();


    DrawMesh(gui);

    camera.SetCamMatrixToShader(dexel_shader.ID);
    //DrawScene(scene);
}

void Render::CalcDeltaTime()
{
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
