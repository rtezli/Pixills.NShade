#pragma once

#include "res.h"
#include "node.h"

namespace ColorShader
{
    struct InputLayout
    {
        XMFLOAT3 Position;
        XMFLOAT4 ColorWithAlpha;
    };

    class ColorVertexShader : public Node
    {

    };
}