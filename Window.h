#pragma once

#include "macros.h"
#include <Windows.h>

#ifdef _UNICODE
#define WINMAIN(hInstance, pCmdLine, nCmdShow)  int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#else
#define WINMAIN(hInstance, pCmdLine, nCmdShow)  int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#endif

CREATE_NEW_DATA_TYPE(WStyle);
CREATE_NEW_DATA_TYPE(WExStyle);

namespace window
{
	namespace position
	{
		const unsigned int Default = CW_USEDEFAULT;
	}
	namespace style
	{
		const WStyle Border1(WS_BORDER);
		const WStyle Border2(WS_DLGFRAME);
		const WStyle Child(WS_CHILD);
		const WStyle HorizontalScrollBar(WS_HSCROLL);
		const WStyle VerticalScrollBar(WS_VSCROLL);
		const WStyle ScrollBars(VerticalScrollBar | HorizontalScrollBar);
		const WStyle Maximized(WS_MAXIMIZE);
		const WStyle Minimized(WS_MINIMIZE);
		const WStyle MaximizeButton(WS_MAXIMIZEBOX);
		const WStyle MinimizeButton(WS_MINIMIZEBOX);
		const WStyle Overlapped(WS_OVERLAPPED);
		const WStyle OverlappedWindow(WS_OVERLAPPEDWINDOW);
		const WStyle Visible(WS_VISIBLE);
		const WStyle PopUp(WS_POPUPWINDOW);
		const WStyle SysMenu(WS_SYSMENU);
		const WStyle Resizable(WS_THICKFRAME); 
		const WStyle Default(WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	}
	namespace exstyle
	{
		const WExStyle DragDrop(WS_EX_ACCEPTFILES);
		const WExStyle AppWindow(WS_EX_APPWINDOW);
		const WExStyle ClientEdge(WS_EX_CLIENTEDGE);
		const WExStyle QuestionMark(WS_EX_CONTEXTHELP);
		const WExStyle DoubleBorder(WS_EX_DLGMODALFRAME);
		const WExStyle AlphaBlend(WS_EX_LAYERED);
		const WExStyle LeftAlign(WS_EX_LEFT);
		const WExStyle LeftScrollBar(WS_EX_LEFTSCROLLBAR);
		const WExStyle Overlapped(WS_EX_OVERLAPPEDWINDOW);
		const WExStyle Palette(WS_EX_PALETTEWINDOW);
		const WExStyle RightAlign(WS_EX_RIGHT);
		const WExStyle RightScrollBar(WS_EX_RIGHTSCROLLBAR);
		const WExStyle StaticEdge(WS_EX_STATICEDGE);
		const WExStyle ToolWindow(WS_EX_TOOLWINDOW);
		const WExStyle TopMost(WS_EX_TOPMOST);
		const WExStyle Transparent(WS_EX_TRANSPARENT);
		const WExStyle RaisedEdge(WS_EX_WINDOWEDGE);
		const WExStyle Default(0);
	}
}


class Window
{
	private:
		MSG msg = {};
		HWND Handle = {};
	public:
		Window(){}

		Window(const TCHAR* classname, const TCHAR* windowname,
			const WStyle style,
			const int x, const int y,
			const int width, const int height)
		{
			Create(classname, windowname, style, x, y, width, height);
		}

		Window(const TCHAR* classname, const TCHAR* windowname,
			const WStyle style,
			const int x, const int y,
			const int width, const int height,
			const HWND parent,
			const HMENU menuID)
		{
			Create(classname, windowname, style, x, y, width, height);
		}

		void SetAppData(const void* data)
		{
			SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG_PTR)data);
		}

		void* GetAppData()
		{
			return (void*)GetWindowLongPtr(Handle, GWLP_USERDATA);
		}

		void SetTitle(const TCHAR* title)
		{
			SetWindowText(Handle, title);
		}

		bool MessageAvailable()
		{
			return PeekMessage(&msg, Handle, 0, 0, PM_NOREMOVE) != 0;
		}

		bool getMessageNoWait()
		{
			return PeekMessage(&msg, Handle, 0, 0, PM_REMOVE) != 0;
		}

		LRESULT sendMessage()
		{
			TranslateMessage(&msg);
			return DispatchMessage(&msg);
		}

		bool getMessage()
		{
			return GetMessage(&msg, Handle, 0, 0) > 0;
		}

		bool Destroy()
		{
			if (!Handle)
				return true;
			if (!DestroyWindow(Handle))
				return false;

			Handle = 0;
			return true;;
		}

		operator HWND()
		{
			if (IsWindow(Handle))
				return Handle;
			return Handle = 0;
		}

		bool Create(const TCHAR* classname, const TCHAR* windowname,
			const WStyle style,
			const int x, const int y,
			const int width, const int height,
			const HWND parent,
			const HMENU menuID)
		{
			RECT rect = { 0, 0, width, height };
			AdjustWindowRectEx(&rect, style, menuID != 0, 0);

			return (Handle = CreateWindowEx(0, classname, windowname, style, x, y, rect.right - rect.left, rect.bottom - rect.top, parent, menuID, GetModuleHandle(0), 0)) != 0;
		}

		bool Create(const TCHAR* classname, const TCHAR* windowname,
			const WStyle style,
			const int x, const int y,
			const int width, const int height)
		{
			RECT rect = { 0, 0, width, height };
			AdjustWindowRectEx(&rect, style, false, 0);

			return (Handle = CreateWindowEx(0, classname, windowname, style, x, y, rect.right - rect.left, rect.bottom - rect.top, 0, 0, GetModuleHandle(0), 0)) != 0;
		}

		~Window()
		{
			Destroy();
		}
};