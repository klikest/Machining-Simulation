#include "DexelGrid.h"


DexelGrid::DexelGrid()
{

}

bool Scalar_cyl(float r, float x, float y)
{
    if ((x * x) + (y * y) <= (r * r))
    {
        return true;
    }
    else
    {
        return false;
    }
}


void DexelGrid::DeleteDexelBlank()
{
    for (int i = 0; i < sizeof(grid) / sizeof(Dexel); i++)
    {
        delete[] grid[i];
        grid[i] = nullptr;
    }

    delete[] grid;
    grid = nullptr;

    delete[] num_dexels;
    num_dexels = nullptr;
}

void DexelGrid::CreateBlankCyl(float diam, float h, float acc)
{

	X_size = (diam / acc);
	Y_size = (diam / acc);
    Z_size = h/acc;


	grid = new Dexel * [X_size * Y_size];
	num_dexels = new int[X_size * Y_size];

    summ_num_of_dexels = 0;

    for (int i = 0; i < X_size * Y_size; i++)
    {
        grid[i] = new Dexel[1];
        num_dexels[i] = 1;

        if (Scalar_cyl(diam/2, i % X_size - X_size / 2, (i / X_size) % Y_size - Y_size / 2) )
        {
            grid[i][0].start = 0;
            grid[i][0].end = h;
            grid[i][0].color = 0;

            summ_num_of_dexels += 1;
        }
        else
        {
            grid[i][0].start = 0;
            grid[i][0].end = 0;
            grid[i][0].color = 0;
        }
    }


}


void DexelGrid::GenerateDrawables()
{
    
    dexel_draw_data = new glm::vec4[summ_num_of_dexels];
    colors_dexels = new float[summ_num_of_dexels];

    for (int i = 0; i < X_size * Y_size; i++)
    {
        if (grid[i][0].end - grid[i][0].start != 0)
        {
            dexel_draw_data[i] = glm::vec4( (i % X_size - X_size / 2), (i / X_size) % Y_size - Y_size / 2), (float)grid[i]->start, (float)(grid[i]->end - grid[i]->start) );
        }
    }

}