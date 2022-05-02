#pragma once

#include "D3D11Common.h"
#include <vector>
using std::vector;

class D3D11VertexBuffer
{
private:
	ID3D11Buffer* VertexBuffer = NULL;
	ID3D11DeviceContext* Context = NULL;
	size_t ElementCount = 0;
	size_t Stride = 0;
public:
	D3D11VertexBuffer(ID3D11Buffer* buffer, ID3D11DeviceContext* context)
		: VertexBuffer(buffer), Context(context) {}
	bool SetData(const vector<VertexPC>& data)
	{
		if (VertexBuffer == NULL)
			return false;

		D3D11_MAPPED_SUBRESOURCE ms{};
		HRESULT res = Context->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		if (FAILED(res))
			return false;
		ElementCount = data.size();
		if (ElementCount > 0)
		{
			Stride = sizeof(data[0]);
			memcpy(ms.pData, data.data(), ElementCount * Stride);
		}
		Context->Unmap(VertexBuffer, NULL);

		return true;
	}
	UINT GetStride() const
	{
		return sizeof(VertexPC);
	}
	size_t GetElementCount() const
	{
		return ElementCount;
	}
	ID3D11Buffer* const* GetBufferPointer() const
	{
		return &VertexBuffer;
	}
	bool Valid() const
	{
		return VertexBuffer != NULL;
	}
	~D3D11VertexBuffer()
	{
		ReleaseAndNullify(VertexBuffer);
		ElementCount = Stride = 0;
	}
};
