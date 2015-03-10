#include "stdafx.h"
#include "d3d.h"

VOID D3D::SetRenderTarget(DeviceResources *resources, ID3D11Buffer *buffer, TextureSettings *settings)
{
	auto renderTarget = new BufferTarget(resources);
	//auto view = renderTarget->GetView();
	//resources->DeviceContext->OMSetRenderTargets(1, &view, NULL);
}

VOID D3D::SetRenderTarget(DeviceResources *resources, ID3D11Texture2D *texture)
{
	TextureSettings settings;
	auto renderTarget = new TextureTarget(resources, &settings);
	//auto view = renderTarget->GetView();
	//resources->DeviceContext->OMSetRenderTargets(1, &view, NULL);
}

