#pragma once

#include "D3D11Common.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class D3D11Shaders
{
private:
	ID3D11VertexShader* VertexShader = NULL;
	ID3D11PixelShader* PixelShader = NULL;
	ID3D11InputLayout* Layout = NULL;
	string ErrorMSG = "No errors";
public:
	D3D11Shaders() {}
	D3D11Shaders(ID3DBlob* errors)
	{
		if (errors == NULL)
			return;
		ErrorMSG.clear();
		const size_t Size = errors->GetBufferSize();
		const char* pointer = (char*)errors->GetBufferPointer();
		for (size_t i = 0; i < Size; i++)
			ErrorMSG.push_back(pointer[i]);
		ReleaseAndNullify(errors);
	}
	D3D11Shaders(string errorMSG)
		:ErrorMSG(errorMSG) {}
	D3D11Shaders(ID3D11VertexShader* vs, ID3D11PixelShader* ps, ID3D11InputLayout* il)
		: VertexShader(vs), PixelShader(ps), Layout(il) {}
	D3D11Shaders& operator=(const D3D11Shaders& s)
	{
		VertexShader = s.VertexShader;
		PixelShader = s.PixelShader;
		Layout = s.Layout;
	}
	ID3D11VertexShader* GetVS() const
	{
		return VertexShader;
	}
	ID3D11PixelShader* GetPS() const
	{
		return PixelShader;
	}
	ID3D11InputLayout* GetLayout() const
	{
		return Layout;
	}
	string GetErrorMSG() const
	{
		return ErrorMSG;
	}
	bool Valid() const
	{
		return VertexShader != NULL && PixelShader != NULL && Layout != NULL;
	}
	~D3D11Shaders()
	{
		ReleaseAndNullify(VertexShader);
		ReleaseAndNullify(PixelShader);
		ReleaseAndNullify(Layout);
	}
};
