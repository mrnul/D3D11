#pragma once

template<typename ElementType>
class D3D11Buffer
{
private:
	ID3D11Buffer* Buffer = NULL;
	ID3D11DeviceContext* Context = NULL;
	size_t ElementCount = 0;
public:
	D3D11Buffer(ID3D11Buffer* buffer, ID3D11DeviceContext* context)
		: Buffer(buffer), Context(context) {}

	bool SetData(const vector<ElementType>& data)
	{
		if (Buffer == NULL)
			return false;

		D3D11_MAPPED_SUBRESOURCE ms{};
		HRESULT res = Context->Map(Buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		if (FAILED(res))
			return false;
		ElementCount = data.size();
		if (ElementCount > 0)
			memcpy(ms.pData, data.data(), ElementCount * sizeof(ElementType));

		Context->Unmap(Buffer, NULL);

		return true;
	}

	UINT GetStride() const
	{
		return sizeof(ElementType);
	}
	size_t GetElementCount() const
	{
		return ElementCount;
	}
	ID3D11Buffer* const* GetBufferPointer() const
	{
		return &Buffer;
	}
	ID3D11Buffer* GetBuffer() const
	{
		return Buffer;
	}
	bool Valid() const
	{
		return Buffer != NULL;
	}
	~D3D11Buffer()
	{
		ReleaseAndNullify(Buffer);
		ElementCount = 0;
	}
};