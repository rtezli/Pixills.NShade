#pragma once

#include "common.h"

class RenderTarget
{
public:
    ID3D11Buffer* const GetBuffer(){ return _targetBuffer; };
private:
    ID3D11Buffer* _targetBuffer;
};

