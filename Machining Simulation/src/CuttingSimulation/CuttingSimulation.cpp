#include "CuttingSimulation.h"


CuttingSimulation::CuttingSimulation()
{
	SimulationWindow = NULL;
	width = NULL;
	height = NULL;
}


void CuttingSimulation::CreateSimWindow(int width_, int height_, std::string title_)
{
	width = width_;
	height = height_;
	title = title_;

	SimulationWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (SimulationWindow == NULL)
	{
		std::cout << "GLFW window create - failed" << std::endl;
		glfwTerminate();
	}
	else
	{
		std::cout << "GLFW window create - done" << std::endl;
	}

	glfwMakeContextCurrent(SimulationWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD initialize - failed" << std::endl;
	}
	else
	{
		std::cout << "GLAD initialize - done" << std::endl;
	}



	ptr_render_scene = std::make_unique<Render>();
	ptr_render_scene->Init(SimulationWindow);

	ptr_gui = std::make_unique<GUI>();
	ptr_gui->SetCurretWindow(SimulationWindow);
	ptr_gui->Init();

	ptr_fbo = std::make_unique<FrameBuffer>();
	ptr_fbo->Init();
}


void CuttingSimulation::Run()
{


	while (glfwGetKey(SimulationWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(SimulationWindow) == 0)
	{
		glfwPollEvents();


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ptr_gui->RenderGUI(*ptr_fbo, ptr_render_scene.get());

		ptr_fbo->Bind();

		ptr_render_scene->Draw(SimulationWindow);

		ptr_fbo->Unbind();

		glfwSwapBuffers(SimulationWindow);


	}
}

void CuttingSimulation::Close()
{

	glfwDestroyWindow(SimulationWindow);
	glfwTerminate();
}