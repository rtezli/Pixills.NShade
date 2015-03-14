#pragma once

#include "d3d11.h"

class RenderInput
{
public:
    ID3D11Buffer* const GetBuffer(){ return _targetBuffer; };
private:
    ID3D11Buffer* _targetBuffer;
};

