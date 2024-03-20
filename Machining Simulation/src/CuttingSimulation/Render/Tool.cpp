#include "Tool.h"


void Tool::Generate_Tool_lines(float D, float H, Coordinates coords)
{
    tool_lines.clear();

    float pi = 2 * asin(1.0);

    for (float angle = 0; angle < 2 * pi; angle += pi / 20)
    {
        tool_lines.push_back(glm::vec3( sinf(angle) * D, cosf(angle) * D, 0));
        tool_lines.push_back(glm::vec3(sinf(angle) * D, cosf(angle) * D, H));

        tool_lines.push_back(glm::vec3(sinf(angle + pi / 20) * D, cosf(angle + pi / 20) * D, H));
        tool_lines.push_back(glm::vec3(sinf(angle) * D, cosf(angle) * D, H));

        tool_lines.push_back(glm::vec3(sinf(angle + pi / 20) * D, cosf(angle + pi / 20) * D, 0));
        tool_lines.push_back(glm::vec3(sinf(angle) * D, cosf(angle) * D, 0));
    }


    //MyMath::transformArray(tool_lines, coords);

    for (int i = 0; i < tool_lines.size(); i ++)
    {
        tool_lines[i] = MyMath::transform(tool_lines[i], coords);
    }

    tool_offset_line.clear();

    tool_offset_line.push_back(MyMath::transform(glm::vec3(0, 0, -coords.offset), coords));
    tool_offset_line.push_back(MyMath::transform(glm::vec3(0, 0, 0), coords));
}

void Tool::Generate_Toool_Dexels(float D_, float H_, float resolution_, Coordinates mashine_coords)
{
	resolution = resolution_;
	D = D_;
	H = H_;

	X_grid_size = MyMath::Get_Grid_size(D, resolution);
	Y_grid_size = MyMath::Get_Grid_size(D, resolution);

}

void Tool::Clear_Arrays()
{
	if (Grid != nullptr)
	{
		delete[] Grid;
		Grid = nullptr;
	}

	Num_of_Dexels = 0;
}