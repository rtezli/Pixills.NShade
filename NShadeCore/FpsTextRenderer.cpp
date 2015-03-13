#include "stdafx.h"
#include "FpsTextRenderer.h"

HRESULT FpsTextRenderer::Initialize()
{
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(Res::Get()->RenderQuality->TextureFormat, D2D1_ALPHA_MODE_PREMULTIPLIED),
		Res::Get()->Dpi,
		Res::Get()->Dpi);

	IDXGISurface2* dxgiBackBuffer;
	Res::Get()->SwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

	//ID2D1Bitmap1* targetBitmap;
	// DeviceContext()->CreateBitmapFromDxgiSurface(dxgiBackBuffer, &bitmapProperties, &targetBitmap);
	// DeviceContext()->SetTarget(targetBitmap);
	// DeviceContext()->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	return 0;
}
