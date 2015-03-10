#pragma once

#include "common.h"
#include "rendertarget.h"

class RenderTarget
{
public:
	ID3D11RenderTargetView* const GetView(){ return _targetView; }
private:
	ID3D11RenderTargetView* _targetView;
};

