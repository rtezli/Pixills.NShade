#pragma once

#include "d3d11.h"
#include "export.h"

using namespace std;

EXTERN class API ShaderBuffer
{
public:
    ID3D11Buffer* GetBuffer(){ return _buffer; }
protected:
    ID3D11Buffer *_buffer;
};