#include "nc_app_pch.hpp"
#include "nc_app_wnd.h"
#if (defined NC_WAPI)
#	if (NC_WAPI & NC_WAPI_WIN)
namespace NC
{
	app_wnd_t::app_wnd_t() :
		mem_user_t(),
		m_handle{ NC_NULL },
		m_class{ NC_NULL },
		m_msg{ NC_NULL },
		m_title(NC_DEFAULT_STR),
		m_ver_str(NC_DEFAULT_STR),
		m_viewp(NC_NULL),
		m_ratio(NC_NULL),
		m_opacity(NC_NULL),
		m_is_enabled(NC_NULL),
		m_is_hovered(NC_NULL),
		m_is_focused(NC_NULL),
		m_event_proc([](nc_event_t&)->v1bit_t { return NC_FALSE; })
	{
	}
	app_wnd_t::~app_wnd_t()
	{
		if (m_handle != NULL) { ::DestroyWindow(m_handle); m_handle = { NC_NULL }; }
		if (m_class.lpfnWndProc != NULL) { ::UnregisterClass(m_class.lpszClassName, m_class.hInstance); m_class = { NC_NULL }; }
	}
	// setters //
	app_wnd_t::wnd_t& app_wnd_t::set_size_xy(cv2u size_xy) {
		m_viewp[2] = size_xy[0];
		m_viewp[3] = size_xy[1];
		//SetWindowPos(m_handle, NULL, 0, 0, GetSizeW(), GetSizeH(), SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		PostMessage(m_handle, WM_PAINT, NC_NULL, NC_NULL);
		return *this;
	}
	app_wnd_t::wnd_t& app_wnd_t::set_coord_xy(cv2u coord_xy) {
		m_viewp[0] = coord_xy[0];
		m_viewp[1] = coord_xy[1];
		//SetWindowPos(m_handle, NULL, GetCoordX(), GetCoordY(), 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
		PostMessage(m_handle, WM_PAINT, NC_NULL, NC_NULL);
		return *this;
	}
	app_wnd_t::wnd_t& app_wnd_t::set_viewp_xywh(cv4u viewport) {
		m_viewp[0] = viewport[0];
		m_viewp[1] = viewport[1];
		m_viewp[2] = viewport[2];
		m_viewp[3] = viewport[3];
		return *this;
	}
	app_wnd_t::wnd_t& app_wnd_t::set_title(cstr_t window_title) {
		m_title = window_title;
		::SetWindowTextA(m_handle, window_title);
		return *this;
	}
	app_wnd_t::wnd_t& app_wnd_t::set_opacity(v1f_t opacity) {
		opacity = opacity > 1.0f ? 1.0f : opacity < 0.1f ? 0.1f : opacity;
		m_opacity = opacity;
		DWORD style_flags = ::GetWindowLongW(m_handle, GWL_EXSTYLE);
		if (opacity < 1.0f) {	// get window style and add "layered attribute" to it
			style_flags |= WS_EX_LAYERED;
			::SetWindowLongW(m_handle, GWL_EXSTYLE, style_flags);
			::SetLayeredWindowAttributes(m_handle, NC_NULL, static_cast<byte_t>(255.0f * opacity), LWA_ALPHA);
		}
		else {  // get rid of transparency
			style_flags &= ~WS_EX_LAYERED;	// "&~0b0010" == get all bits except this one
			::SetWindowLongW(m_handle, GWL_EXSTYLE, style_flags);
		}
		return *this;
	}
	app_wnd_t::wnd_t& app_wnd_t::set_enabled(v1bit_t is_enabled) {
		if (m_is_enabled == is_enabled) { return *this; }
		m_is_enabled = is_enabled;
		if (is_enabled) { ::EnableWindow(m_handle, NC_TRUTH); }
		else { ::EnableWindow(m_handle, NC_FALSE); }
		return *this;
	}
	app_wnd_t::wnd_t& app_wnd_t::set_focused(v1bit_t is_focused) {
		if (m_is_focused == is_focused) { return *this; }
		m_is_focused = is_focused;
		if (is_focused) { ::SetFocus(m_handle); }
		else { ::SetFocus(NC_NULL); }
		return *this;
	}
	app_wnd_t::wnd_t& app_wnd_t::set_hovered(v1bit_t is_hovered) {
		if (m_is_hovered == is_hovered) { return *this; }
		if (is_hovered) { }
		return *this;
	}
	app_wnd_t::wnd_t& app_wnd_t::set_icon(icon_tc& img) {
		return *this;
	}
	app_wnd_t::wnd_t& app_wnd_t::set_callback(event_callback_tc& event_proc) {
		m_event_proc = event_proc;
		return *this;
	}
	// commands //
	v1nil_t app_wnd_t::update()
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
	v1bit_t app_wnd_t::remake()
	{
		NC_CHECK(NC_TRUTH, "", return NC_FALSE);

		static TCHAR class_name[256];
		OemToChar(get_title(), class_name);
		m_class.lpszClassName = class_name;
		// register a window class to create a window;
		m_class.lpszMenuName = NC_NULL;
		m_class.lpfnWndProc = NC_NULL;
		// GetModuleHandle(NULL) is the current application;
		m_class.hInstance = GetModuleHandle(NC_NULL);
		m_class.hIcon = LoadIcon(m_class.hInstance, MAKEINTRESOURCE(NC_APP_ICON1));
		m_class.hIconSm = LoadIcon(m_class.hInstance, MAKEINTRESOURCE(NC_APP_ICON1));
		m_class.hCursor = LoadCursor(m_class.hInstance, MAKEINTRESOURCE(NC_APP_CURSOR0));
		m_class.hbrBackground = NC_NULL;
		m_class.cbClsExtra = NC_NULL;
		m_class.cbWndExtra = NC_NULL;
		m_class.cbSize = sizeof(WNDCLASSEX);
		// everything is left is:
		// ->set up window class procedure;
		// ->set up window class style;
		// ->create window handle;
		return NC_TRUTH;
	}

	// --==</core_methods>==--
	// --==<impl_methods>==--
	LRESULT WINAPI app_wnd_t::event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg != WM_NCCREATE) { return ::DefWindowProc(hwnd, msg, wparam, lparam); }

		CREATESTRUCT* crtst = reinterpret_cast<CREATESTRUCT*>(lparam);
		app_wnd_t* nc_app_wnd = reinterpret_cast<app_wnd_t*>(crtst->lpCreateParams);

		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nc_app_wnd));
		::SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(app_wnd_t::event_proc_static));

		return app_wnd_t::event_proc_static(hwnd, msg, wparam, lparam);
	}
	LRESULT WINAPI app_wnd_t::event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return reinterpret_cast<app_wnd_t*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA))->event_proc(hwnd, msg, wparam, lparam);
	}
	LRESULT inline WINAPI app_wnd_t::event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
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
			set_size_xy({ LOWORD(lparam), HIWORD(lparam) });
			nc_event_t wnd_evt = nc_event_t(NC_EVTYPE_WINDOW_SIZED, get_size_x(), get_size_y());
			m_event_proc(wnd_evt);
			PostMessage(m_handle, WM_PAINT, 0, 0);
			return 0l;
			break;
		}
		case WM_MOVE: {
			set_coord_xy(v2u{ LOWORD(lparam), HIWORD(lparam) });
			nc_event_t wnd_evt = nc_event_t(NC_EVTYPE_WINDOW_MOVED, get_coord_x(), get_coord_y());
			m_event_proc(wnd_evt);
			PostMessage(m_handle, WM_PAINT, NC_NULL, NC_NULL);
			return 0l;
			break;
		}
		case WM_SETFOCUS: {		// wparam is the last window was focused, lParam is not used
			nc_event_t wnd_evt(NC_EVTYPE_WINDOW_FOCUS, NC_TRUTH);
			m_event_proc(wnd_evt);
			set_focused(NC_TRUTH);
			return 0l;
			break;
		}
		case WM_CLOSE: {
			nc_event_t wnd_evt(NC_EVTYPE_WINDOW_CLOSE, NC_TRUTH);
			m_event_proc(wnd_evt);
			::PostQuitMessage(NC_NULL);
			return 0l;
			break;
		}
		case WM_KILLFOCUS: {	// wparam is the next window will be focused, lParam is not used
			set_focused(NC_FALSE);
			nc_event_t wnd_evt = nc_event_t(NC_EVTYPE_WINDOW_FOCUS, NC_FALSE);
			m_event_proc(wnd_evt);
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
#endif	// NC_WAPI