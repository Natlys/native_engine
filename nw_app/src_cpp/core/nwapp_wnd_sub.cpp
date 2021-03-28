#include "nwapp_pch.hpp"
#include "nwapp_wnd_sub.h"
#if (defined NW_WAPI)
#include "nwapp_wnd_core.h"
#	if (NW_WAPI & NW_WAPI_WIN)
namespace NWAPP
{
	app_wnd_sub::app_wnd_sub(app_wnd_core& core, cinfo& information) :
		app_wnd(information),
		m_core(&core)
	{
		// register a window class to create a window; ModuleHandle is the current application;
		m_class.lpfnWndProc = app_wnd_sub::event_proc_init;
		m_class.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
		if (::RegisterClassEx(&m_class) == FALSE) { throw init_error(__FILE__, __LINE__); return; }
		DWORD style = 0;
		// control styles;
		style |= WS_CAPTION;		// title bar;
		style |= WS_SYSMENU;		// must be used with WS_CAPTION;
		style |= WS_MINIMIZEBOX;	// button sizing;
		style |= WS_MAXIMIZEBOX;	// button sizing;
		style |= WS_SIZEBOX;		// arrow-border sizing;
		// view styles;
		style |= WS_OVERLAPPED;		// title bar and border; same as WS_TILED;
		style |= WS_TILED;			// title bar and border; same as WS_OVERLAPPED;
		style |= WS_BORDER;			// thin-line border;
		style |= WS_THICKFRAME;		// ;
		DWORD style_ex = 0;
		style_ex |= WS_EX_APPWINDOW;	// force top window onto the taskbar whe it is visible;
		style_ex |= WS_EX_ACCEPTFILES;	// drag-drop files;
		style_ex |= WS_EX_LEFT;			// generic left-aligned properties;
		style_ex |= WS_EX_WINDOWEDGE;	// the window has a border with a raised edge
		
		RECT wnd_rect = { 100, 100, 100 + static_cast<LONG>(get_size_x()), 100 + static_cast<LONG>(get_size_y()) };
		::AdjustWindowRect(&wnd_rect, style, FALSE);
		
		m_handle = CreateWindowEx(
			style_ex,
			m_class.lpszClassName, m_class.lpszClassName,
			style,
			wnd_rect.left, wnd_rect.top,
			wnd_rect.right - wnd_rect.left, wnd_rect.bottom - wnd_rect.top,
			NULL, NULL,
			m_class.hInstance,
			this
		);
		if (m_handle == NULL) { throw init_error(__FILE__, __LINE__); return; }
		
		::ShowWindow(m_handle, SW_SHOWDEFAULT);
		::UpdateWindow(m_handle);
		set_title(get_title());
	}
	app_wnd_sub::~app_wnd_sub()
	{
	}
	// --setters
	// --==<core_methods>==--
	void app_wnd_sub::update()
	{
		app_wnd::update();
	}
	// --==</core_methods>==--
	// --==<impl_methods>==--
	LRESULT inline __stdcall app_wnd_sub::event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg != WM_NCCREATE) { return ::DefWindowProc(hwnd, msg, wparam, lparam); }

		CREATESTRUCT* crtst = reinterpret_cast<CREATESTRUCT*>(lparam);
		app_wnd_sub* nwapp_window = reinterpret_cast<app_wnd_sub*>(crtst->lpCreateParams);

		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nwapp_window));
		::SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(app_wnd_sub::event_proc_static));

		return app_wnd_sub::event_proc_static(hwnd, msg, wparam, lparam);
	}
	LRESULT inline __stdcall app_wnd_sub::event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return reinterpret_cast<app_wnd_sub*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA))->event_proc(hwnd, msg, wparam, lparam);
	}
	inline LRESULT __stdcall app_wnd_sub::event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		switch (msg) {
		// events_of_window
		{
		case WM_SIZE: {
			m_info.size_x = LOWORD(lparam);
			m_info.size_y = HIWORD(lparam);
			//SetWindowPos(m_handle, NULL, 0, 0, GetSizeW(), GetSizeH(), SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
			wnd_event wnd_evt = wnd_event(EVT_WND_RESIZE, get_size_x(), get_size_y());
			m_info.event_proc(wnd_evt);
			PostMessage(m_handle, WM_PAINT, 0, 0);
			return 0l;
			break;
		}
		case WM_MOVE: {
			m_info.coord_y = LOWORD(lparam);
			m_info.coord_x = HIWORD(lparam);
			//SetWindowPos(m_handle, NULL, GetCoordX(), GetCoordY(), 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
			wnd_event wnd_evt = wnd_event(EVT_WND_MOVE, get_coord_x(), get_coord_y());
			m_info.event_proc(wnd_evt);
			PostMessage(m_handle, WM_PAINT, 0, 0);
			return 0l;
			break;
		}
		case WM_SETFOCUS: {		// wparam is the last window was focused, lParam is not used
			wnd_event wnd_evt = wnd_event(EVT_WND_FOCUS);
			m_info.event_proc(wnd_evt);
			m_info.is_focused = true;
			return 0l;
			break;
		}
		case WM_CLOSE: {
			m_info.event_proc(wnd_event(EVT_WND_CLOSE));
			::PostQuitMessage(0);
			return 0l;
			break;
		}
		case WM_KILLFOCUS: {	// wparam is the next window will be focused, lParam is not used
			m_info.is_focused = false;
			return 0l;
			break;
		}
		}
		default: break;
		}
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
}
#	endif	// WAPI_WIN
#endif	// NW_WAPI