#include "stdafx.h"
#include "overlayrenderer.h"


FpsTextRenderer::FpsTextRenderer(DeviceResources* pResources)
{
	m_pDeviceResources = pResources;
}


FpsTextRenderer::~FpsTextRenderer()
{
}

HRESULT FpsTextRenderer::Initialize()
{
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(Resources()->RenderQuality->TextureFormat, D2D1_ALPHA_MODE_PREMULTIPLIED),
		Resources()->Dpi,
		Resources()->Dpi);

	IDXGISurface2* dxgiBackBuffer;
	Resources()->SwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

	ID2D1Bitmap1* targetBitmap;
	
	// DeviceContext()->CreateBitmapFromDxgiSurface(dxgiBackBuffer, &bitmapProperties, &targetBitmap);
	// DeviceContext()->SetTarget(targetBitmap);
	// DeviceContext()->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	return 0;
}
