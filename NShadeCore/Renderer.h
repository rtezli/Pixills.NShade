#pragma once

#include "export.h"
#include "memory"
#include "d3d11.h"
#include "dxgi.h"

class Renderer
{
public:
	Renderer::Renderer(const std::shared_ptr<Renderer>& deviceResources);
	void Renderer::CreateDeviceDependentResources();
	void Renderer::CreateWindowSizeDependentResources();
	void Renderer::ReleaseDeviceDependentResources();
	//void Update(DX::StepTimer const& timer);
	void Renderer::Render(IDXGISwapChain* swapChain);
	void Renderer::RenderDeferred(IDXGISwapChain* swapChain);
	void Renderer::StopRendering();
	void Renderer::StartTracking();
	void Renderer::TrackingUpdate(float positionX);
	void Renderer::StopTracking();
	bool Renderer::IsTracking() { return m_tracking; }
	bool Renderer::IsRendering(){ return m_isRendering; }
private:
	void Rotate(float radians);

private:
	// Zeiger in den Geräteressourcen zwischengespeichert.
	std::shared_ptr<Renderer> m_deviceResources;

	// Direct3D-Ressourcen für Würfelgeometrie.
	std::shared_ptr<ID3D11InputLayout>	m_inputLayout;
	std::shared_ptr<ID3D11Buffer>		m_vertexBuffer;
	std::shared_ptr<ID3D11Buffer>		m_indexBuffer;
	std::shared_ptr<ID3D11VertexShader>	m_vertexShader;
	std::shared_ptr<ID3D11PixelShader>	m_pixelShader;
	std::shared_ptr<ID3D11Buffer>		m_constantBuffer;

	// Systemressourcen für Würfelgeometrie.
	//ModelViewProjectionConstantBuffer	m_constantBufferData;
	//uint32	m_indexCount;

	// Für die Renderschleife verwendete Variablen.
	bool	m_isRendering;
	bool	m_loadingComplete;
	float	m_degreesPerSecond;
	bool	m_tracking;
};
