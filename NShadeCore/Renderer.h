#pragma once

#include "memory"
#include "d3d11.h"

using namespace std;

namespace NShade
{
	class Renderer
	{
	public:
		NShade::Renderer::Renderer(const shared_ptr<NShade::Renderer>& deviceResources);
		void NShade::Renderer::CreateDeviceDependentResources();
		void NShade::Renderer::CreateWindowSizeDependentResources();
		void NShade::Renderer::ReleaseDeviceDependentResources();
	    //void Update(DX::StepTimer const& timer);
		void NShade::Renderer::Render();
		void NShade::Renderer::StopRendering();
		void NShade::Renderer::StartTracking();
		void NShade::Renderer::TrackingUpdate(float positionX);
		void NShade::Renderer::StopTracking();
		bool NShade::Renderer::IsTracking() { return m_tracking; }
		bool NShade::Renderer::IsRendering(){ return m_isRendering; }

	private:
		void Rotate(float radians);

	private:
		// Zeiger in den Geräteressourcen zwischengespeichert.
		shared_ptr<NShade::Renderer> m_deviceResources;

		// Direct3D-Ressourcen für Würfelgeometrie.
		shared_ptr<ID3D11InputLayout>	m_inputLayout;
		shared_ptr<ID3D11Buffer>		m_vertexBuffer;
		shared_ptr<ID3D11Buffer>		m_indexBuffer;
		shared_ptr<ID3D11VertexShader>	m_vertexShader;
		shared_ptr<ID3D11PixelShader>	m_pixelShader;
		shared_ptr<ID3D11Buffer>		m_constantBuffer;

		// Systemressourcen für Würfelgeometrie.
		//ModelViewProjectionConstantBuffer	m_constantBufferData;
		//uint32	m_indexCount;

		// Für die Renderschleife verwendete Variablen.
		bool	m_isRendering;
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;
	};
}