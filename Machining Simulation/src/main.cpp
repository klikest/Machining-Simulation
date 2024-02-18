#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "App.h"

int main()
{
	std::unique_ptr<App> ptr_App(new App);

	ptr_App->InitApp();
	ptr_App->Run();
	ptr_App->Close();

	//system("pause");

	return 0;
}