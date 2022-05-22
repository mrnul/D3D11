#pragma once

#include "D3D11Common.h"
#include <vector>
using std::vector;

class D3D11ConstantBuffer
{
private:
	ID3D11Buffer* ConstantBuffer = NULL;
	ID3D11DeviceContext* Context = NULL;
	size_t ElementCount = 0;
	size_t Stride = 0;
public:
	D3D11ConstantBuffer(ID3D11Buffer* buffer, ID3D11DeviceContext* context)
		: ConstantBuffer(buffer), Context(context) {}
	bool SetData(const vector<ConstantBufferData>& data)
	{
		if (ConstantBuffer == NULL)
			return false;

		D3D11_MAPPED_SUBRESOURCE ms{};
		HRESULT res = Context->Map(ConstantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		if (FAILED(res))
			return false;
		ElementCount = data.size();
		if (ElementCount > 0)
		{
			Stride = sizeof(data[0]);
			memcpy(ms.pData, data.data(), ElementCount * Stride);
		}
		Context->Unmap(ConstantBuffer, NULL);

		return true;
	}
	UINT GetStride() const
	{
		return sizeof(ConstantBufferData);
	}
	size_t GetElementCount() const
	{
		return ElementCount;
	}
	ID3D11Buffer* const* GetBufferPointer() const
	{
		return &ConstantBuffer;
	}
	bool Valid() const
	{
		return ConstantBuffer != NULL;
	}
	~D3D11ConstantBuffer()
	{
		ReleaseAndNullify(ConstantBuffer);
		ElementCount = Stride = 0;
	}
};
