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




void DexelGrid::CreateBlankCyl(float diam, float h, float acc_)
{
    acc = acc_;




    if ( (int)ceil(diam / acc)%2 == 1)
    {
        X_size = ceil(diam / acc + 1);
        Y_size = ceil(diam / acc + 1);
    }
    else
    {
        X_size = ceil(diam / acc);
        Y_size = ceil(diam / acc);
    }

    std::cout << X_size << std::endl;

    rect_min = glm::vec3(acc * -X_size / 2, acc * -Y_size / 2, 0);
    rect_max = glm::vec3(acc * X_size / 2, acc * Y_size / 2, h);


    grid = new Dexel[X_size * Y_size];
    summ_num_of_dexels = 0;

    for (int i = 0; i < X_size * Y_size; i++)
    {
        float x = (i % X_size) - (X_size / 2) + (acc/2);
        float y = (i / X_size) % Y_size - (Y_size / 2) + (acc/2);

        if (Scalar_cyl(diam / (2 * acc), x, y))
        {
            grid[i].start = 0;
            grid[i].end = h;
            grid[i].color = 0;

            summ_num_of_dexels += 1;
        }
        else
        {
            grid[i].start = 0;
            grid[i].end = 0;
            grid[i].color = 0;
        }

    }

}


void DexelGrid::GenerateDrawArrays()
{
    dexel_draw_data = new glm::vec4[summ_num_of_dexels];
    colors_dexels = new float[summ_num_of_dexels];

    int count = 0;

    for (int i = 0; i < X_size * Y_size; i++)
    {
        float x = (i % X_size) - X_size/2;
        float y = (i / X_size) % Y_size  - Y_size/2;
        float z = grid[i].start;
        float len = grid[i].end - grid[i].start;

        if (len != 0)
        {
            dexel_draw_data[count] = glm::vec4(x, y, z, len);
            colors_dexels[count] = grid->color;
            count += 1;
        }

    }
}


void DexelGrid::DeleteArrays()
{
    delete[] grid;
    grid = nullptr;

    delete[] dexel_draw_data;
    dexel_draw_data = nullptr;

    delete[] colors_dexels;
    colors_dexels = nullptr;

}


