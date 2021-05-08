#include "nw_app_pch.hpp"
#include "nw_app_wnd_sub.h"
#if (defined NW_WAPI)
#	include "nw_app_wnd_core.h"
#	if (NW_WAPI & NW_WAPI_WIN)
namespace NW
{
	app_wnd_sub::app_wnd_sub(app_wnd_core_t& core) :
		app_wnd_t(),
		m_core(&core)
	{
	}
	app_wnd_sub::~app_wnd_sub()
	{
	}
	// setters //
	// commands //
	v1nil_t app_wnd_sub::update()
	{
		app_wnd_t::update();
	}
	v1bit_t app_wnd_sub::remake() {
		NW_CHECK(app_wnd_t::remake(), "remake error!", return NW_FALSE);

		// register a window class to create a window; ModuleHandle is the current application;
		m_class.lpfnWndProc = app_wnd_sub::event_proc_init;
		m_class.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
		NW_CHECK(::RegisterClassEx(&m_class), "failed register!", return NW_FALSE);
		DWORD style = NW_NULL;
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
		DWORD style_ex = NW_NULL;
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
			NW_NULL, NW_NULL,
			m_class.hInstance,
			this
		);
		NW_CHECK(m_handle, "no handle!", return NW_FALSE);

		::ShowWindow(m_handle, SW_SHOWDEFAULT);
		::UpdateWindow(m_handle);

		return NW_TRUE;
	}
	// --==</core_methods>==--
	// --==<impl_methods>==--
	inline LRESULT WINAPI app_wnd_sub::event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg != WM_NCCREATE) { return ::DefWindowProc(hwnd, msg, wparam, lparam); }

		CREATESTRUCT* crtst = reinterpret_cast<CREATESTRUCT*>(lparam);
		app_wnd_sub* nw_app_window = reinterpret_cast<app_wnd_sub*>(crtst->lpCreateParams);

		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nw_app_window));
		::SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(app_wnd_sub::event_proc_static));

		return app_wnd_sub::event_proc_static(hwnd, msg, wparam, lparam);
	}
	inline LRESULT WINAPI app_wnd_sub::event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return reinterpret_cast<app_wnd_sub*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA))->event_proc(hwnd, msg, wparam, lparam);
	}
	inline LRESULT WINAPI app_wnd_sub::event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		switch (msg) {
		// events_of_window
		{
		case WM_SIZE: {
			set_size_xy({ LOWORD(lparam), HIWORD(lparam) });
			nw_event_t wnd_evt(NW_EVTYPE_WINDOW_SIZED, get_size_x(), get_size_y());
			m_event_proc(wnd_evt);
			PostMessage(m_handle, WM_PAINT, NW_NULL, NW_NULL);
			return 0l;
			break;
		}
		case WM_MOVE: {
			set_coord_xy({ LOWORD(lparam), HIWORD(lparam) });
			nw_event_t wnd_evt(NW_EVTYPE_WINDOW_MOVED, get_coord_x(), get_coord_y());
			m_event_proc(wnd_evt);
			PostMessage(m_handle, WM_PAINT, NW_NULL, NW_NULL);
			return 0l;
			break;
		}
		case WM_SETFOCUS: {		// wparam is the last window was focused, lParam is not used
			set_focused(NW_TRUE);
			nw_event_t wnd_evt(NW_EVTYPE_WINDOW_FOCUS, NW_TRUE);
			m_event_proc(wnd_evt);
			return 0l;
			break;
		}
		case WM_KILLFOCUS: {	// wparam is the next window will be focused, lParam is not used
			set_focused(NW_FALSE);
			nw_event_t wnd_evt(NW_EVTYPE_WINDOW_FOCUS, NW_FALSE);
			m_event_proc(wnd_evt);
			return 0l;
			break;
		}
		case WM_CLOSE: {
			nw_event_t wnd_evt(NW_EVTYPE_WINDOW_CLOSE, NW_TRUE);
			m_event_proc(wnd_evt);
			::PostQuitMessage(NW_NULL);
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