#pragma once

#include "D3D11Common.h"
#include <vector>
using std::vector;

class D3D11IndexBuffer
{
private:
	ID3D11Buffer* IndexBuffer = NULL;
	ID3D11DeviceContext* Context = NULL;
	size_t ElementCount = 0;
	size_t Stride = 0;
public:
	D3D11IndexBuffer(ID3D11Buffer* buffer, ID3D11DeviceContext* context)
		: IndexBuffer(buffer), Context(context) {}
	bool SetData(const vector<unsigned int>& data)
	{
		if (IndexBuffer == NULL)
			return false;

		D3D11_MAPPED_SUBRESOURCE ms{};
		HRESULT res = Context->Map(IndexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		if (FAILED(res))
			return false;
		ElementCount = data.size();
		if (ElementCount > 0)
		{
			Stride = sizeof(unsigned int);
			memcpy(ms.pData, data.data(), ElementCount * Stride);
		}
		Context->Unmap(IndexBuffer, NULL);

		return true;
	}
	UINT GetStride() const
	{
		return sizeof(unsigned int);
	}
	size_t GetElementCount() const
	{
		return ElementCount;
	}
	ID3D11Buffer* GetBuffer() const
	{
		return IndexBuffer;
	}
	bool Valid() const
	{
		return IndexBuffer != NULL;
	}
	~D3D11IndexBuffer()
	{
		ReleaseAndNullify(IndexBuffer);
		ElementCount = Stride = 0;
	}
};
