#pragma once

#include "macros.h"
#include <Windows.h>

#define WINPROC(WinProcName,HandleName,MsgName,WparamName,LparamName) LRESULT CALLBACK WinProcName(HWND HandleName,UINT MsgName,WPARAM WparamName,LPARAM LparamName)
#define WINPROCDEF(WinProcName) LRESULT CALLBACK WinProcName(HWND handle,UINT msg,WPARAM wp,LPARAM lp)

WINPROC(BasicProc, handle, msg, wp, lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(handle, msg, wp, lp);
}


CREATE_NEW_DATA_TYPE(WCStyle);
CREATE_NEW_DATA_TYPE(WCColor);

namespace windowclass
{
	namespace style
	{
		const WCStyle AlignClient(CS_BYTEALIGNCLIENT);
		const WCStyle AlignWindow(CS_BYTEALIGNWINDOW);
		const WCStyle DoubleClicks(CS_DBLCLKS);
		const WCStyle DropShadow(CS_DROPSHADOW);
		const WCStyle WidthReDraw(CS_HREDRAW);
		const WCStyle HeightReDraw(CS_VREDRAW);
		const WCStyle NoClose(CS_NOCLOSE);
		const WCStyle ReDraw(CS_VREDRAW | CS_HREDRAW);
		const WCStyle Default(ReDraw);
	}

	const WNDPROC BasicWindowProc = BasicProc;
}

class WindowClass
{
private:
	WNDCLASSEX WinClass = {};
	bool Registered = false;
public:
	WindowClass(){}

	const WNDCLASSEX GetClass()
	{
		return WinClass;
	}

	operator bool()
	{
		return Registered;
	}

	WindowClass(const TCHAR* name, const WNDPROC proc = BasicProc, 
		const WCStyle style = windowclass::style::Default, 
		const HCURSOR cursor = LoadCursor(NULL, IDC_ARROW), 
		const HICON icon = LoadIcon(NULL, IDI_APPLICATION))
	{
		Register(name, proc, style, cursor, icon);
	}

	~WindowClass()
	{
		if (Registered)
			Unregister();
	}

	bool Unregister()
	{
		if (UnregisterClass(WinClass.lpszClassName, GetModuleHandle(0)) != 0)
		{
			WinClass = {};
			Registered = false;
			return true;
		}
		return false;

	}

	bool Register(const TCHAR* name, const WNDPROC proc, const WCStyle style, const HCURSOR cursor, const HICON icon)
	{
		if (Registered)
			return false;
		WinClass = {};
		WinClass.style = style;
		WinClass.cbSize = sizeof(WinClass);


		WinClass.hCursor = cursor;
		WinClass.hIcon = icon;

		WinClass.lpfnWndProc = proc;
		WinClass.lpszClassName = name;
		WinClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		return Registered = (RegisterClassEx(&WinClass) != 0);
	}
};