#pragma once

#include "dxgi.h"
#include "d2d1.h"
#include "res.h"
#include "export.h"

EXTERN class API Overlay
{
public:
    static Overlay* Create(IDXGISwapChain *swapChain);
protected:
    IDXGISurface    *_surface;
    RECT            *_poistion;
    RECT            *_size;
private:
    Overlay();
};