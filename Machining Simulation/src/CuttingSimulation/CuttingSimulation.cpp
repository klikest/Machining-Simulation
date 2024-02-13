#include "CuttingSimulation.h"


CuttingSimulation::CuttingSimulation()
{
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



}


void CuttingSimulation::Run()
{


	while (glfwGetKey(SimulationWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(SimulationWindow) == 0)
	{
		glfwPollEvents();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		glfwSwapBuffers(SimulationWindow);


	}
}

void CuttingSimulation::Close()
{

	glfwDestroyWindow(SimulationWindow);
	glfwTerminate();
}