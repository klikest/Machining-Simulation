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


}

void DexelScene::Init()
{

}

