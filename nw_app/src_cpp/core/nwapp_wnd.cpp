#include "nwapp_pch.hpp"
#include "nwapp_wnd.h"
#if (defined NW_WAPI)
namespace NWAPP
{
	window_info::window_info(cstr window_title, v1u width, v1u height) :
		title(window_title),
		coord_x(0), coord_y(0), size_x(width), size_y(height),
		aspect_ratio(static_cast<v1f>(width) / static_cast<v1f>(height)) { }
	// --operators
	stm_out& window_info::operator<<(stm_out& stm) const {
		return stm <<
			"--==<window_info>==--" << std::endl <<
			"title: " << &title[0] << std::endl <<
			"coord_x/coord_y: " << coord_x << "/" << coord_y << std::endl <<
			"size_w/size_h: " << size_x << "/" << size_y << std::endl <<
			"aspect_ratio: " << aspect_ratio << std::endl <<
			"api_version: " << &api_version[0] << std::endl <<
			"--==</window_info>==--";
	}
	stm_in& window_info::operator>>(stm_in& stm) {
		return stm >>
			title >> api_version >>
			coord_x >> coord_y >> size_x >> size_y >>
			aspect_ratio >>
			opacity >>
			is_hovered >> is_hovered >> is_hovered;
	}
}
#	if (NW_WAPI & NW_WAPI_WIN)
namespace NWAPP
{
	app_wnd::app_wnd(cinfo& information) :
		a_mem_user(),
		m_info(information), m_handle{ 0 },
		m_class{ 0 }, m_msg{ 0 }
	{
		static TCHAR class_name[256];
		OemToChar(&information.title[0], class_name);
		m_class.lpszClassName = class_name;
		// register a window class to create a window; ModuleHandle is the current application;
		m_class.lpszMenuName = NULL;
		m_class.lpfnWndProc = NULL;
		m_class.hInstance = GetModuleHandle(NULL);
		m_class.hIcon = LoadIcon(m_class.hInstance, MAKEINTRESOURCE(NWAPP_ICON1));
		m_class.hIconSm = LoadIcon(m_class.hInstance, MAKEINTRESOURCE(NWAPP_ICON1));
		m_class.hCursor = LoadCursor(m_class.hInstance, MAKEINTRESOURCE(NWAPP_CURSOR0));
		m_class.hbrBackground = NULL;
		m_class.cbClsExtra = 0;
		m_class.cbWndExtra = 0;
		m_class.cbSize = sizeof(WNDCLASSEX);
	}
	app_wnd::~app_wnd()
	{
		if (m_handle != NULL) { ::DestroyWindow(m_handle); m_handle = { 0 }; }
		if (m_class.lpfnWndProc != NULL) { ::UnregisterClass(m_class.lpszClassName, m_class.hInstance); m_class = { 0 }; }
	}
	// --setters
	void app_wnd::set_title(cstr window_title) {
		m_info.title = window_title;
		::SetWindowTextA(m_handle, window_title);
	}
	void app_wnd::set_opacity(v1f opacity) {
		opacity = opacity > 1.0f ? 1.0f : opacity < 0.1f ? 0.1f : opacity;
		m_info.opacity = opacity;
		DWORD style_flags = ::GetWindowLongW(m_handle, GWL_EXSTYLE);
		if (opacity < 1.0f) {	// get window style and add "layered attribute" to it
			style_flags |= WS_EX_LAYERED;
			::SetWindowLongW(m_handle, GWL_EXSTYLE, style_flags);
			::SetLayeredWindowAttributes(m_handle, 0u, static_cast<ubyte>(255 * opacity), LWA_ALPHA);
		}
		else {  // get rid of transparency
			style_flags &= ~WS_EX_LAYERED;	// "&~0b0010" == get all bits except this one
			::SetWindowLongW(m_handle, GWL_EXSTYLE, style_flags);
		}
	}
	void app_wnd::set_focused(v1b is_focused) {
		if (m_info.is_focused == is_focused) { return; }
		if (is_focused) { SetFocus(m_handle); }
	}
	void app_wnd::set_enabled(v1b is_enabled) {
		if (m_info.is_enabled == is_enabled) { return; }
		m_info.is_enabled = is_enabled;
		::EnableWindow(m_handle, is_enabled);
	}
	void app_wnd::set_icon(const a_img_cmp& img) {
	}
	void app_wnd::set_callback(const event_callback& event_proc) {
		m_info.event_proc = event_proc;
	}
	// --==<core_methods>==--
	void app_wnd::update()
	{
		// if the message queue is empty - thread can be blocked by GetMessage()
		// if there is 0 - we've got a quit; can be also -1
		if (m_msg.message != WM_QUIT) {
			// if there is false - we don't have a message
			if (::PeekMessage(&m_msg, m_handle, 0u, 0u, PM_REMOVE)) {
				::TranslateMessage(&m_msg);	// make WM_CHAR/WM_SYSCHAR messages
				::DispatchMessage(&m_msg);	// send all the messages into window procedure
			}
		}
	}
	// --==</core_methods>==--
	// --==<impl_methods>==--
	LRESULT __stdcall app_wnd::event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg != WM_NCCREATE) { return ::DefWindowProc(hwnd, msg, wparam, lparam); }

		CREATESTRUCT* crtst = reinterpret_cast<CREATESTRUCT*>(lparam);
		app_wnd* nwapp_window = reinterpret_cast<app_wnd*>(crtst->lpCreateParams);

		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nwapp_window));
		::SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(app_wnd::event_proc_static));

		return app_wnd::event_proc_static(hwnd, msg, wparam, lparam);
	}
	LRESULT __stdcall app_wnd::event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return reinterpret_cast<app_wnd*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA))->event_proc(hwnd, msg, wparam, lparam);
	}
	LRESULT inline __stdcall app_wnd::event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		switch (msg) {
		// events_of_application
		case WM_CREATE: {
			return 0l;
			break;
		}
		case WM_DESTROY: {
			return 0l;
			break;
		}
		case WM_QUIT: {
			return 0l;
			break;
		}
		case WM_PAINT: {
			return 0l;
			break;
		}
		case WM_ERASEBKGND: {
			return 0l;
			break;
		}
		// events_of_keyboard
		// events_of_mouse
		// events_of_window
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
		default: break;
		}
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	// --==</impl_methods>==--
}
#	endif	// WAPI_WIN
#endif	// NW_WAPI