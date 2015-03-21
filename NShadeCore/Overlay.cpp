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

void Overlay::Render()
{
    //ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
    //Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

    //context->SaveDrawingState(m_stateBlock.Get());
    //context->BeginDraw();

    //// Position on the bottom right corner
    //D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
    //    logicalSize.Width - m_textMetrics.layoutWidth,
    //    logicalSize.Height - m_textMetrics.height
    //    );

    //context->SetTransform(screenTranslation * m_deviceResources->GetOrientationTransform2D());

    //DX::ThrowIfFailed(
    //    m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING)
    //    );

    //context->DrawTextLayout(
    //    D2D1::Point2F(0.f, 0.f),
    //    m_textLayout.Get(),
    //    m_whiteBrush.Get()
    //    );

    //// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
    //// is lost. It will be handled during the next call to Present.
    //HRESULT hr = context->EndDraw();
    //if (hr != D2DERR_RECREATE_TARGET)
    //{
    //    DX::ThrowIfFailed(hr);
    //}

    //context->RestoreDrawingState(m_stateBlock.Get());
}