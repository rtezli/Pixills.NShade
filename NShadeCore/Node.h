#pragma once

#include "vector"
#include "memory"
#include "res.h"
#include "export.h"

using namespace std;

EXTERN class API Node
{
public:
    void AddInput(ID3D11Resource *resource);
    void AddOutput(ID3D11Resource *resource);
private:
    vector<ID3D11Resource*> _inputs;
    vector<ID3D11Resource*> _outputs;
};