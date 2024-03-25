#include "Profiler.h"

void Profiler::CreateArrays()
{
	Boolean_op_time.clear();
	Generate_draw_array_time.clear();
	Render_scene_time.clear();
	Generate_tool_time.clear();


	Boolean_op_time.resize(500);
	Generate_draw_array_time.resize(500);
	Render_scene_time.resize(500);
	Generate_tool_time.resize(500);

	for (int i = 0; i < 500; i++)
	{
		Boolean_op_time[i] = 0;
		Generate_draw_array_time[i] = 0;
		Render_scene_time[i] = 0;
		Generate_tool_time[i] = 0;
	}
}

void Profiler::AddDataToArray(std::vector<float>& array, float new_data)
{
	array.erase(array.begin());
	array.push_back(new_data);
}
