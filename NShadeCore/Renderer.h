#pragma once

#include "export.h"
#include "memory"
#include "d3d11.h"
#include "dxgi.h"

class Renderer
{
public:
	Renderer();
	~Renderer();
public:
	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
	void ReleaseDeviceDependentResources();

	void Render(IDXGISwapChain* swapChain);
	void RenderDeferred(IDXGISwapChain* swapChain);
	void StopRendering();
	void StartTracking();
	void StopTracking();
	void TrackingUpdate(float positionX);

	bool IsTracking() { return m_tracking; }
	bool IsRendering(){ return m_isRendering; }
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
