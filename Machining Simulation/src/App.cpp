#include "App.h"
#include "CuttingSimulation/CuttingSimulation.h"


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


void App::InitApp()
{
	glfwSetErrorCallback(glfw_error_callback);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);  // comment this line in a release build!!!!!!!!!!!!!!!!!!!!!!!!!!!! 


}

void App::Run()
{

	ptr_CuttingSimulationApp = std::make_unique<CuttingSimulation>();

	ptr_CuttingSimulationApp->CreateSimWindow(1280, 920, "Machining Simulation");
	ptr_CuttingSimulationApp->Run();
}

void App::Close()
{
	ptr_CuttingSimulationApp->Close();
}

