#include "stdafx.h"
#include "node.h"

void Node::AddInput(ID3D11Resource *resource)
{
   // auto res = shared_ptr<ID3D11Resource>(resource);
    _inputs.push_back(resource);
}

void Node::AddOutput(ID3D11Resource *resource)
{
   // auto res = shared_ptr<ID3D11Resource>(resource);
    _outputs.push_back(resource);
}