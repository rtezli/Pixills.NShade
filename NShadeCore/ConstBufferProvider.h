#pragma once

#include "d3d11.h"
#include "memory"
#include "export.h"

using namespace std;

EXTERN class API ConstBufferProvider
{
public:
    ID3D11Buffer* GetBuffer(){ return _buffer.get(); }
protected:
    void SetBuffer(char *data, unsigned int size, D3D11_BIND_FLAG bindFlags);
private:
    unsigned int                _dataSize;
    shared_ptr<char>            _data;
    shared_ptr<ID3D11Buffer>    _buffer;
};