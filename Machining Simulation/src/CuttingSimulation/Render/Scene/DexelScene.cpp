#include "DexelScene.h"

DexelScene::DexelScene()
{
}

DexelScene::~DexelScene()
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


void DexelScene::CreateCylBlank(float d, float h, float acc)
{
    X_grid_size = d + 5;
    Y_grid_size = d + 5;

    blank_grid = new Dexel[X_grid_size * Y_grid_size];

    for (int i = 0; i < X_grid_size * Y_grid_size; i++)
    {
        blank_grid[i].start = 0;
        blank_grid[i].end = h;
        blank_grid[i].color = 1;
    }

}

void DexelScene::Init()
{

}

