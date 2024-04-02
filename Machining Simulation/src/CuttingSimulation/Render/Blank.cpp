#include "Blank.h"

void Blank::CreateCylBlank(float D_, float H_, float resolution_)
{
	resolution = resolution_;
	D = D_;
	H = H_;

	X_grid_size = MyMath::Get_Grid_size(D, resolution);
	Y_grid_size = MyMath::Get_Grid_size(D, resolution);
	

	Grid_size = (X_grid_size * Y_grid_size);

	ClearArrays();

	Grid = new std::vector<Dexel>[Grid_size];

	for (int i = 0; i < Grid_size; i++)
	{

		float x = resolution * ((i % X_grid_size) - (X_grid_size / 2));
		float y = resolution * (((i / X_grid_size) % Y_grid_size) - (Y_grid_size / 2));

		glm::vec3 normal = glm::vec3(x, y, 0);

		if (MyMath::Scalar_cyl(D / 2, x, y))
		{
			
			Grid[i].push_back(Dexel(0, H, 0, normal));
			Num_of_Dexels += 1;
		}
		else
		{
			Grid[i].push_back(Dexel(0, H, -1, normal));
		}

	}

}

void Blank::ClearArrays()
{
	if (Grid != nullptr)
	{
		delete[] Grid;
		Grid = nullptr;
	}

	Num_of_Dexels = 0;
}

float Blank::Get_X_From_Grid_By_i(int i)
{
	return ((i % X_grid_size) - X_grid_size / 2) - resolution/2;
}

float Blank::Get_Y_From_Grid_By_i(int i)
{
	return ((i / X_grid_size) % Y_grid_size - Y_grid_size / 2) - resolution / 2;
}

glm::vec4 Blank::Get_Dexel_To_Draw(int i, int j)
{
	return glm::vec4(Get_X_From_Grid_By_i(i), Get_Y_From_Grid_By_i(i), Grid[i][j].start, (Grid[i][j].end - Grid[i][j].start));
}