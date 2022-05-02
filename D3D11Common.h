#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib") 
#pragma comment (lib, "D3DCompiler.lib") 

template<class D3D11Type>
void ReleaseAndNullify(D3D11Type* var)
{
	if (var != NULL)
	{
		var->Release();
		var = NULL;
	}
}

struct PositionXYZ
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float xyz[3];
	};
};

struct ColorRGBA
{
	union
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		float rgba[4];
	};
};

struct VertexPC
{
	PositionXYZ XYZ;
	ColorRGBA RGBA;
};
