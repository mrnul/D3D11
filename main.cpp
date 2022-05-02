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

	D3D11VertexBuffer buffer = test.CreateVertexBuffer(1024); // 1024 is big enough for now
	test.SetVertexBuffer(buffer);

	while (win)
	{
		vector<VertexPC> vertices;

		const float time = (float)GetTickCount64();
		vertices.push_back({ -0.25f, 0.1f, 0.2f, {1.0f, 0.0f, 0.0f, time} });
		vertices.push_back({ 1.0f, 0.0f, 0.2f, {0.0f, 1.0f, 0.0f, time} });
		vertices.push_back({ -0.25f, -0.1f, 0.2f, {0.0f, 0.0f, 1.0f, time} });

		vertices.push_back({ -0.5f, 1.0f, 0.5f, {1.0f, 0.0f, 0.0f, time} });
		vertices.push_back({ 1.0f, 0.0f, 0.5f, {0.0f, 1.0f, 0.0f, time} });
		vertices.push_back({ -0.5f, -1.0f, 0.5f, {0.0f, 0.0f, 1.0f, time} });

		buffer.SetData(vertices);

		if (win.MessageAvailable())
		{
			win.getMessage();
			win.sendMessage();
		}
		test.Clear();
		test.Render(vertices.size());

		// Sleep(1);
	}
}