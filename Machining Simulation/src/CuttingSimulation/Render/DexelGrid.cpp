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
        X_size = ceil(diam / acc) ;
        Y_size = ceil(diam / acc) ;
    }
    else
    {
        X_size = ceil(diam / acc) + 1;
        Y_size = ceil(diam / acc) + 1;
    }

    rect_min = glm::vec3(acc * -X_size / 2, acc * -Y_size / 2, 0);
    rect_max = glm::vec3(acc * X_size / 2, acc * Y_size / 2, h);


    num_dexels = new int[X_size * Y_size];
    grid_list = new Dexel* [X_size * Y_size];
    summ_num_of_dexels = 0;

    for (int i = 0; i < X_size * Y_size; i++)
    {
        float x = acc * ( (i % X_size) - (X_size / 2) );
        float y = acc * ( ((i / X_size) % Y_size) - (Y_size / 2) );

        if (i == 0)
        {
            test_rect_min = glm::vec3(x, y, 0.1);
            std::cout << "min x =" << x << std::endl;
        }
        if (i == (X_size * Y_size) - 1)
        {
            test_rect_max = glm::vec3(x, y, 1);
            std::cout << "max x =" << x << std::endl;
        }

        grid_list[i] = new Dexel[1];
        num_dexels[i] = 1;

        if (Scalar_cyl(diam / 2, x, y))
        {

            grid_list[i][0].start = 0;
            grid_list[i][0].end = h;
            grid_list[i][0].color = 0;

            summ_num_of_dexels += 1;
        }
        else
        {
            grid_list[i][0].start = 0;
            grid_list[i][0].end = 0;
            grid_list[i][0].color = 0;

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
        for (int num = 0; num < num_dexels[i]; num++)
        {
            float x = (i % X_size) - X_size / 2;
            float y = (i / X_size) % Y_size - Y_size / 2;
            float z = grid_list[i][num].start;
            float len = grid_list[i][num].end - grid_list[i][num].start;

            if (len != 0)
            {
                dexel_draw_data[count] = glm::vec4(x - acc/2, y - acc/2, z, len);
                colors_dexels[count] = grid_list[i][num].color;
                count += 1;
            }
        }


    }
}


void DexelGrid::DeleteArrays()
{
    for (int i = 0; i < X_size * Y_size; i++)
    {
        delete[] grid_list[i];
        grid_list[i] = nullptr;
    }

    delete[] grid_list;
    grid_list = nullptr;

    delete[] dexel_draw_data;
    dexel_draw_data = nullptr;

    delete[] colors_dexels;
    colors_dexels = nullptr;

}


