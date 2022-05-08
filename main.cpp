#include <iostream>
#include <string>
#include <vector>
#include "Window.h"
#include "WindowClass.h"
#include "D3D11Core.h"

using std::vector;
using std::cout;
using std::endl;

int main()
{
	const wchar_t* name = L"D3DTest";

	WindowClass c(name);
	Window win(name, name, window::style::PopUp | window::style::Visible, 100, 100, 800, 600);

	D3D11Core test = D3D11Core();
	if (!test.Initialize(win, 800, 600))
		cout << "Error: " << GetLastError() << endl;

	D3D11Shaders shaders = test.CreateShaders(L"shaders.shader", L"shaders.shader", "VShader", "PShader");

	if (!test.SetShaders(shaders))
		cout << "Error: " << shaders.GetErrorMSG() << endl;

	test.SetPrimitiveTopology();

	D3D11VertexBuffer vBuffer = test.CreateVertexBuffer(1024); // 1024 is big enough for now
	D3D11IndexBuffer iBuffer = test.CreateIndexBuffer(1024); // 1024 is big enough for now

	iBuffer.SetData({ 0, 1, 2, 3, 4, 5 });
	test.SetVertexBuffer(vBuffer);
	test.SetIndexBuffer(iBuffer);
	while (win)
	{
		if (win.MessageAvailable())
		{
			win.getMessage();
			win.sendMessage();
		}

		const float time = (float)GetTickCount64();
		vBuffer.SetData({
			{{ -0.25f, 0.1f, 0.2f }, { 1.0f, 0.0f, 0.0f, time }},
			{{ 1.0f, 0.0f, 0.2f}, {0.0f, 1.0f, 0.0f, time}},
			{{ -0.25f, -0.1f, 0.2f }, { 0.0f, 0.0f, 1.0f, time }},
			{{ -0.5f, 1.0f, 0.5f }, { 1.0f, 0.0f, 0.0f, time }},
			{{ 1.0f, 0.0f, 0.5f }, { 0.0f, 1.0f, 0.0f, time }},
			{{ -0.5f, -1.0f, 0.5f }, { 0.0f, 0.0f, 1.0f, time }}
			});

		ColorRGBA color = { abs(sin(time / 1000.f)), abs(sin(time / 2000.f)), abs(sin(time / 500.f)) };
		test.BeginDraw(color);
		test.IDraw(iBuffer.GetElementCount());
		test.EndDraw();
		// Sleep(1);
	}
}