#pragma once

#include "D3D11Common.h"
#include "D3D11VertexBuffer.h"
#include "D3D11Shaders.h"
#include "D3D11IndexBuffer.h"

class D3D11Core
{
private:
	ID3D11RenderTargetView* RenderTargetView = NULL;
	IDXGISwapChain* SwapChain = NULL;
	ID3D11Device* Device = NULL;
	ID3D11DeviceContext* DeviceContext = NULL;
	ID3D11DepthStencilView* DepthStencilView = NULL;

	bool CreateRenderTargetView()
	{
		ID3D11Resource* backBuffer = NULL;
		HRESULT res = SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (LPVOID*)&backBuffer);
		if (FAILED(res))
			return false;
		res = Device->CreateRenderTargetView(backBuffer, NULL, &RenderTargetView);
		ReleaseAndNullify(backBuffer);
		if (FAILED(res))
			return false;
		return true;
	}
	bool CreateDepthStencil(const unsigned width, const unsigned height, const unsigned sampleCount)
	{
		ID3D11DepthStencilState* state = NULL;
		D3D11_TEXTURE2D_DESC desc{};

		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = sampleCount;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

		ID3D11Texture2D* tmp = NULL;
		HRESULT res = Device->CreateTexture2D(&desc, NULL, &tmp);
		if (FAILED(res))
			return false;

		D3D11_DEPTH_STENCIL_DESC stencilDesc{};
		stencilDesc.DepthEnable = TRUE;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;

		res = Device->CreateDepthStencilState(&stencilDesc, &state);
		if (FAILED(res))
		{
			ReleaseAndNullify(tmp);
			return false;
		}
		DeviceContext->OMSetDepthStencilState(state, 1);

		D3D11_DEPTH_STENCIL_VIEW_DESC stencilViewDesc{};
		stencilViewDesc.Format = desc.Format;
		stencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
		res = Device->CreateDepthStencilView(tmp, &stencilViewDesc, &DepthStencilView);
		if (FAILED(res))
		{
			ReleaseAndNullify(tmp);
			ReleaseAndNullify(state);
			return false;
		}
		return true;
	}
public:
	D3D11Core() {}
	bool Initialize(HWND window, const unsigned width, const unsigned height)
	{
		DXGI_SWAP_CHAIN_DESC scd{};
		scd.BufferCount = 1;
		scd.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
		scd.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = window;
		scd.SampleDesc.Count = 1;
		scd.Windowed = TRUE;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT res = D3D11CreateDeviceAndSwapChain
		(
			NULL,
			D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_VIDEO_SUPPORT,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&scd,
			&SwapChain,
			&Device,
			NULL,
			&DeviceContext
		);

		if (FAILED(res))
			return false;

		if (!CreateRenderTargetView())
			return false;

		CreateDepthStencil(width, height, scd.SampleDesc.Count);

		DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

		D3D11_VIEWPORT viewport{};
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)height;
		DeviceContext->RSSetViewports(1, &viewport);
		return true;
	}

	const D3D11VertexBuffer CreateVertexBuffer(
		const UINT bufferSizeBytes,
		const D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC,
		const UINT accessFlagsCPU = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE)
	{
		ID3D11Buffer* vertexBuffer = NULL;
		D3D11_BUFFER_DESC descriptor{ bufferSizeBytes, usage, D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER, accessFlagsCPU, 0, 0 };
		Device->CreateBuffer(&descriptor, NULL, &vertexBuffer);
		return D3D11VertexBuffer(vertexBuffer, DeviceContext);
	}

	const D3D11IndexBuffer CreateIndexBuffer(
		const UINT bufferSizeBytes,
		const D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC,
		const UINT accessFlagsCPU = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE)
	{
		ID3D11Buffer* indexBuffer = NULL;
		D3D11_BUFFER_DESC descriptor{ bufferSizeBytes, usage, D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER, accessFlagsCPU, 0, 0 };
		Device->CreateBuffer(&descriptor, NULL, &indexBuffer);
		return D3D11IndexBuffer(indexBuffer, DeviceContext);
	}

	const D3D11Shaders CreateShaders(LPCWSTR fileVShader, LPCWSTR filePShader, LPCSTR entryVpoint, LPCSTR entryPpoint, vector<D3D11_INPUT_ELEMENT_DESC> descriptors =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		})
	{
		ID3DBlob* VS = NULL;
		ID3DBlob* PS = NULL;
		ID3D11VertexShader* VertexShader = NULL;
		ID3D11PixelShader* PixelShader = NULL;
		ID3D11InputLayout* Layout = NULL;
		ID3DBlob* errorMSG = NULL;

		HRESULT res = D3DCompileFromFile(fileVShader, 0, 0, entryVpoint, "vs_5_0", 0, 0, &VS, &errorMSG);
		if (FAILED(res))
			return D3D11Shaders(errorMSG);
		if (filePShader == NULL)
			filePShader = fileVShader;
		res = D3DCompileFromFile(filePShader, 0, 0, entryPpoint, "ps_5_0", 0, 0, &PS, &errorMSG);
		if (FAILED(res))
		{
			ReleaseAndNullify(VS);
			return D3D11Shaders(errorMSG);
		}
		res = Device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &VertexShader);
		if (FAILED(res))
		{
			ReleaseAndNullify(VS);
			ReleaseAndNullify(PS);
			return D3D11Shaders("CreateVertexShader failed");
		}
		res = Device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &PixelShader);
		if (FAILED(res))
		{
			ReleaseAndNullify(VS);
			ReleaseAndNullify(PS);
			ReleaseAndNullify(VertexShader);
			return D3D11Shaders("CreatePixelShader failed");
		}
		res = Device->CreateInputLayout(descriptors.data(), (UINT)descriptors.size(), VS->GetBufferPointer(), VS->GetBufferSize(), &Layout);
		if (FAILED(res))
		{
			ReleaseAndNullify(VS);
			ReleaseAndNullify(PS);
			ReleaseAndNullify(VertexShader);
			ReleaseAndNullify(PixelShader);
			return D3D11Shaders("CreateInputLayout failed");
		}
		ReleaseAndNullify(VS);
		ReleaseAndNullify(PS);
		return D3D11Shaders(VertexShader, PixelShader, Layout);
	}

	bool SetShaders(const D3D11Shaders& shaders)
	{
		if (!shaders.Valid())
			return false;

		DeviceContext->VSSetShader(shaders.GetVS(), 0, 0);
		DeviceContext->PSSetShader(shaders.GetPS(), 0, 0);
		DeviceContext->IASetInputLayout(shaders.GetLayout());
		return true;
	}

	bool SetVertexBuffer(const D3D11VertexBuffer& buffer)
	{
		if (!buffer.Valid())
			return false;

		UINT stride = buffer.GetStride();
		UINT offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, buffer.GetBufferPointer(), &stride, &offset);
		return true;
	}

	bool SetIndexBuffer(const D3D11IndexBuffer& buffer)
	{
		if (!buffer.Valid())
			return false;
		DeviceContext->IASetIndexBuffer(buffer.GetBuffer(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		return true;
	}

	void BeginDraw(ColorRGBA color = { 0.0f, 0.0f, 0.0f, 0.0f })
	{
		DeviceContext->ClearRenderTargetView(RenderTargetView, color.rgba);
		DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		DeviceContext->IASetPrimitiveTopology(topology);
	}

	void VDraw(const UINT count)
	{
		DeviceContext->Draw(count, 0);
	}

	void IDraw(const UINT count)
	{
		DeviceContext->DrawIndexed(count, 0, 0);
	}

	void EndDraw()
	{
		SwapChain->Present(0, 0);
	}

	~D3D11Core()
	{
		ReleaseAndNullify(SwapChain);
		ReleaseAndNullify(Device);
		ReleaseAndNullify(DeviceContext);
		ReleaseAndNullify(RenderTargetView);
		ReleaseAndNullify(DepthStencilView);
	}
};
