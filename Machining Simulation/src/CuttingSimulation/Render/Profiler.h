#pragma once
#include <iostream> 
#include <vector>

class Profiler
{
public:
	std::vector<float> Boolean_op_time;
	std::vector<float> Generate_draw_array_time;
	std::vector<float> Generate_tool_time;
	std::vector<float> Render_scene_time;

	void CreateArrays();
	void AddDataToArray(std::vector<float>& array, float new_data);
};


