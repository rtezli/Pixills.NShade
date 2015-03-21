#include "stdafx.h"
#include "overlay.h"

Overlay::Overlay()
{

}

Overlay* Overlay::Create(IDXGISwapChain *swapChain)
{ 
    auto overlay = new Overlay();
    D2D1_PIXEL_FORMAT format;
    D2D1_BITMAP_PROPERTIES bitmapProperties =
        D2D1::BitmapProperties(
        D2D1::PixelFormat(Res::Get()->RenderQuality->TextureFormat, D2D1_ALPHA_MODE_PREMULTIPLIED),
        Res::Get()->DpiX,
        Res::Get()->DpiY);

    IDXGISurface* dxgiBackBuffer;
    swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

    ID2D1Bitmap* targetBitmap;
    ID2D1RenderTarget* renderTarget;
    //Res::Get()->DeviceContext-> CreateBitmapFromDxgiSurface(dxgiBackBuffer, &bitmapProperties, &targetBitmap);
    //Res::Get()->DeviceContext->SetTarget(targetBitmap);
    //Res::Get()->DeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
    return overlay;
}