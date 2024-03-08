#include "Render.h"



Render::Render()
{

}


Render::~Render()
{
}

void Render::Init(GLFWwindow* window)
{
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


    glGenVertexArrays(1, &VAO_dexel);
    glGenBuffers(1, &VBO_vert_cube_for_dexels);
    glGenBuffers(1, &VBO_offsets_dexel);
    glGenBuffers(1, &VBO_color_dexel);


    camera.Init(window, glm::vec3(-51.0f, 19.0f, 31.0f), -25, -13);


    lines = std::make_unique<Lines>();
    lines->Init();

    tool_mesh = std::make_unique<Mesh>();
    tool_mesh->Init();
    tool_mesh->ReadMeshSTL("C://Users//User//source//repos//Machining Simulation//Machining Simulation//Models//Box1x1x1.stl");

    main_scene = std::make_unique<Scene>();
    main_scene->Init();

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
   


    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    camera.SetCamMatrixToShader(lines->shader.ID);
    lines->AddRectangle(glm::vec3(-10, -10, -10), glm::vec3(10, 10, 10), glm::vec3(0.5, 0.8, 0.2));
    lines->AddCoords(glm::vec3(0, 0, 0));
    lines->AddLines({ 0, 0, 0, 10, 10, 10 }, { 1, 1, 1, 1, 0.5, 0.5 });
    lines->Draw();


    camera.SetCamMatrixToShader(tool_mesh->shader.ID);
    tool_mesh->DrawMesh();
    
    camera.SetCamMatrixToShader(main_scene->shader.ID);
    camera.SetCamPosToShader(main_scene->shader.ID);
    main_scene->Draw();
}

void Render::CalcDeltaTime()
{
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
