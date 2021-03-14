#include <nw_pch.hpp>
#include "nw_window.h"
#if (defined NW_OS)
#include <core/nw_engine.h>
#include <resource.h>
#include <gui/nwg_imgui.h>
namespace NW
{
	window_info::window_info(cstring window_title, ui16 width, ui16 height) :
		title(window_title),
		coord_x(0), coord_y(0), size_x(width), size_y(height),
		aspect_ratio(static_cast<f32>(width) / static_cast<f32>(height)) { }
	// --operators
	out_stream& window_info::operator<<(out_stream& stm) const {
		return stm <<
			"--==<window_info>==--" << std::endl <<
			"title: " << &title[0] << std::endl <<
			"coord_x/coord_y: " << coord_x << "/" << coord_y << std::endl <<
			"size_w/size_h: " << size_x << "/" << size_y << std::endl <<
			"aspect_ratio: " << aspect_ratio << std::endl <<
			"api_version: " << &api_version[0] << std::endl <<
			"--==</window_info>==--";
	}
	in_stream& window_info::operator>>(in_stream& stm) {
		return stm >>
			title >> api_version >>
			coord_x >> coord_y >> size_x >> size_y >>
			aspect_ratio >>
			opacity >>
			is_hovered >> is_hovered >> is_hovered;
	}
}
#if (NW_OS & NW_OS_WIN)
namespace NW
{
	core_window::core_window(const info& info) :
		m_info(info), m_native{ 0 },
		m_kbd(keyboard()), m_mouse(mouse()),
		m_class{ 0 }, m_msg{ 0 },
		m_pts{ 0 }
	{
		// register a window class to create a window. ModuleHandle is the current app;
		auto class_name = L"nw_core_window";
		m_class.lpszClassName = class_name;
		m_class.lpszMenuName = NULL;
		m_class.lpfnWndProc = event_proc_init;
		m_class.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
		m_class.hInstance = GetModuleHandle(NULL);
		m_class.hIcon = LoadIcon(m_class.hInstance, MAKEINTRESOURCE(NW_ICON1));
		m_class.hIconSm = LoadIcon(m_class.hInstance, MAKEINTRESOURCE(NW_ICON1));
		m_class.hCursor = LoadCursor(m_class.hInstance, MAKEINTRESOURCE(NW_CURSOR0));
		m_class.hbrBackground = NULL;
		m_class.cbClsExtra = 0;
		m_class.cbWndExtra = 0;
		m_class.cbSize = sizeof(WNDCLASSEX);
		if (::RegisterClassEx(&m_class) == FALSE) { throw error("window class is not registred"); return; }
		// adjust sizes;
		DWORD wnd_style = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_VISIBLE;
		RECT wnd_rect = { 100, 100, 100 + get_size_x(), 100 + get_size_y() };
		::AdjustWindowRect(&wnd_rect, wnd_style, FALSE);
		// create the window itself;
		wchar wnd_name[256]{ 0 };
		OemToChar(get_title(), &wnd_name[0]);
		m_native = CreateWindowEx(0, class_name, wnd_name,
			wnd_style,
			CW_USEDEFAULT, CW_USEDEFAULT, get_size_x(), get_size_y(),
			NULL, NULL, m_class.hInstance, this);
		if (m_native== nullptr) { throw error("window class is not registred"); return; }
		// do some stuff with the window;
		::ShowWindow(m_native, SW_SHOWDEFAULT);
		::UpdateWindow(m_native);
		set_title(get_title());
		std::cout << m_info;
		// register mouse raw input device;
		RAWINPUTDEVICE raw_in_dev;
		raw_in_dev.usUsagePage = 0x01;	// mouse page bit;
		raw_in_dev.usUsage = 0x02;		// mouse usage bit;
		raw_in_dev.dwFlags = 0u;		// additional param;
		raw_in_dev.hwndTarget = NULL;	// we don't really need a target;
		if (::RegisterRawInputDevices(&raw_in_dev, 1u, sizeof(RAWINPUTDEVICE)) == FALSE) {
			throw error("raw input device is not registred!");
			return;
		}
	}
	core_window::~core_window()
	{
		// get rid of the windows window and it's class
		::DestroyWindow(m_native);
		m_native = { 0 };
		UnregisterClass(m_class.lpszClassName, m_class.hInstance);
		m_class = { 0 };
	}
	// --setters
	void core_window::set_title(cstring window_title) {
		m_info.title = window_title;
		::SetWindowTextA(m_native, window_title);
	}
	void core_window::set_opacity(f32 opacity) {
		opacity = opacity > 1.0f ? 1.0f : opacity < 0.1f ? 0.1f : opacity;
		m_info.opacity = opacity;
		ui64 nStyle = ::GetWindowLongW(m_native, GWL_EXSTYLE);
		if (opacity < 1.0f) {	// get window style and add "layered attribute" to it
			nStyle |= WS_EX_LAYERED;
			::SetWindowLongW(m_native, GWL_EXSTYLE, nStyle);
			::SetLayeredWindowAttributes(m_native, 0u, static_cast<ubyte>(255 * opacity), LWA_ALPHA);
		}
		else {  // get rid of transparency
			nStyle &= ~WS_EX_LAYERED;	// "&~0b0010" == get all bits except this one
			::SetWindowLongW(m_native, GWL_EXSTYLE, nStyle);
		}
	}
	void core_window::set_focused(bit is_focused) {
		if (m_info.is_focused == is_focused) { return; }
		if (is_focused) { SetFocus(m_native); }
	}
	void core_window::set_enabled(bit is_enabled) {
		if (m_info.is_enabled == is_enabled) { return; }
		m_info.is_enabled = is_enabled;
		::EnableWindow(m_native, is_enabled);
	}
	void core_window::set_cursor_enabled(bit enable_cursor) { m_mouse.set_cursor_enabled(enable_cursor); }
	void core_window::set_icon(const a_image& img) { }
	void core_window::set_callback(const event_callback& event_proc) { m_info.event_proc = event_proc; }
	// --==<core_methods>==--
	void core_window::update()
	{
		m_mouse.update();
		m_kbd.update();
		// if the message queue is empty - thread can be blocked by GetMessage()
		// if there is 0 - we've got a quit; can be also -1
		if (m_msg.message != WM_QUIT) {
			// if there is false - we don't have a message
			if (::PeekMessage(&m_msg, m_native, 0u, 0u, PM_REMOVE)) {
				::TranslateMessage(&m_msg);	// make WM_CHAR/WM_SYSCHAR messages
				::DispatchMessage(&m_msg);	// send all the messages into window procedure
			}
		}
	}
	
	LRESULT __stdcall core_window::event_proc_init(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg != WM_NCCREATE) { return DefWindowProc(wnd, msg, wparam, lparam); }

		CREATESTRUCT* crtst = reinterpret_cast<CREATESTRUCT*>(lparam);
		core_window* core_wnd = reinterpret_cast<core_window*>(crtst->lpCreateParams);

		SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(core_wnd));
		SetWindowLongPtr(wnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(event_proc_static));

		return event_proc_static(wnd, msg, wparam, lparam);
	}
	LRESULT __stdcall core_window::event_proc_static(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		core_window* core_wnd = reinterpret_cast<core_window*>(GetWindowLongPtr(wnd, GWLP_USERDATA));

		//if (NW::imgui_event_proc(wnd, msg, wparam, lparam)) { return true; }

		return core_wnd->event_proc(wnd, msg, wparam, lparam);
	}
	LRESULT __stdcall core_window::event_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {

		switch (msg) {
		case WM_CREATE: {
			return 0l;
		}
		case WM_DESTROY: {
			return 0l;
		}
		case WM_QUIT: {
			return 0l;
		}
		case WM_PAINT: {
			return 0l;
		}
		case WM_ERASEBKGND: {
			return 0l;
		}
		// process raw input data
		case WM_INPUT: {
			UINT in_data_size = 0u;
			if (::GetRawInputData(	//  fill the number of bytes variable for the data;
				reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, NULL, &in_data_size, sizeof(RAWINPUTHEADER)) == -1) { break; }
			// error with NULL returns -1, but we just bail that;
			raw_buf.resize(in_data_size);
			if (::GetRawInputData(	// fill data buffer in this time;
				reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, &raw_buf[0], &in_data_size, sizeof(RAWINPUTHEADER)) != in_data_size) { break; }
			// error with reading returns number of the read bytes, bail it again;
			// raw input data processing;
			const RAWINPUT& raw_in = reinterpret_cast<const RAWINPUT&>(raw_buf[0]);
			// we are interested in the header and data;
			if (raw_in.header.dwType == RIM_TYPEMOUSE) {
				if (raw_in.data.mouse.lLastX != 0 || raw_in.data.mouse.lLastY != 0) {
					mouse_event ms_evt = mouse_event(EVT_MOUSE_MOVE, raw_in.data.mouse.lLastX, raw_in.data.mouse.lLastY);
					m_mouse.event_proc(ms_evt);
					m_info.event_proc(ms_evt);
				}
			}
			break;
		}
		case WM_MOUSEHWHEEL: {
			mouse_event ms_evt = mouse_event(EVT_MOUSE_SCROLL, GET_WHEEL_DELTA_WPARAM(wparam) / 100.0f, 0.0);
			m_mouse.event_proc(ms_evt);
			m_info.event_proc(ms_evt);
			return 0l;
		}
		case WM_MOUSEWHEEL: {
			mouse_event ms_evt = mouse_event(EVT_MOUSE_SCROLL, 0.0, GET_WHEEL_DELTA_WPARAM(wparam) / 100.0f);
			m_mouse.event_proc(ms_evt);
			m_info.event_proc(ms_evt);
			return 0l;
		}
		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
		case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
		{
			mouse_codes msb_code = MSC_DEFAULT;
			if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { msb_code = MSC_LEFT; }
			if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { msb_code = MSC_RIGHT; }
			if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { msb_code = MSC_MIDDLE; }
			if (::GetCapture() == nullptr) { ::SetCapture(m_native); }
			mouse_event ms_evt = mouse_event(EVT_MOUSE_PRESSED, msb_code);
			m_mouse.event_proc(ms_evt);
			m_info.event_proc(ms_evt);
			return 0l;
		}
		case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP: case WM_XBUTTONUP: {
			mouse_codes msb_code = MSC_DEFAULT;
			if (msg == WM_LBUTTONUP) { msb_code = MSC_LEFT; }
			if (msg == WM_RBUTTONUP) { msb_code = MSC_RIGHT; }
			if (msg == WM_MBUTTONUP) { msb_code = MSC_MIDDLE; }
			mouse_event ms_evt = mouse_event(EVT_MOUSE_RELEASED, msb_code);
			m_mouse.event_proc(ms_evt);
			m_info.event_proc(ms_evt);
			if (::GetCapture() == m_native) { ::ReleaseCapture(); }
			return 0l;
		}

		case WM_KEYDOWN: case WM_SYSKEYDOWN: {
			keyboard_event kbd_evt = keyboard_event(EVT_KEYBOARD_PRESSED, static_cast<keyboard_codes>(wparam));
			m_kbd.event_proc(kbd_evt);
			m_info.event_proc(kbd_evt);
			return 0l;
		}
		case WM_KEYUP: case WM_SYSKEYUP: {
			keyboard_event kbd_evt = keyboard_event(EVT_KEYBOARD_RELEASED, static_cast<keyboard_codes>(wparam));
			m_kbd.event_proc(kbd_evt);
			m_info.event_proc(kbd_evt);
			return 0l;
		}
		case WM_CHAR: case WM_SYSCHAR: {
			keyboard_event kbd_evt = keyboard_event(EVT_KEYBOARD_CHAR, static_cast<keyboard_codes>(wparam));
			m_kbd.event_proc(kbd_evt);
			m_info.event_proc(kbd_evt);
			return 0l;
		}

		case WM_SIZE: {
			m_info.size_x = LOWORD(lparam);
			m_info.size_y = HIWORD(lparam);
			//SetWindowPos(m_native, NULL, 0, 0, GetSizeW(), GetSizeH(), SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
			window_event wnd_evt = window_event(EVT_WINDOW_RESIZE, get_size_x(), get_size_y());
			m_info.event_proc(wnd_evt);
			PostMessage(m_native, WM_PAINT, 0, 0);
			return 0l;
		}
		case WM_MOVE: {
			m_info.coord_y = LOWORD(lparam);
			m_info.coord_x = HIWORD(lparam);
			//SetWindowPos(m_native, NULL, GetCoordX(), GetCoordY(), 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
			window_event wnd_evt = window_event(EVT_WINDOW_MOVE, get_coord_x(), get_coord_y());
			m_info.event_proc(wnd_evt);
			PostMessage(m_native, WM_PAINT, 0, 0);
			return 0l;
		}
		case WM_SETFOCUS: {		// wparam is the last window was focused, lParam is not used
			window_event wnd_evt = window_event(EVT_WINDOW_FOCUS);
			m_info.event_proc(wnd_evt);
			m_info.is_focused = true;
			return 0l;
		}
		case WM_KILLFOCUS: {	// wparam is the next window will be focused, lParam is not used
			m_info.is_focused = false;
			return 0l;
		}
		case WM_ACTIVATE:
			if (m_mouse.is_cursor_enabled()) {
				if (wparam & WA_ACTIVE) { m_mouse.set_cursor_enabled(false); }
				else { m_mouse.set_cursor_enabled(true); }
			}
			break;
		case WM_CLOSE: {
			m_info.event_proc(window_event(EVT_WINDOW_CLOSE));
			::PostQuitMessage(0);
			return 0l;
		}
		}
		return ::DefWindowProc(wnd, msg, wparam, lparam);
	}
	// --==</core_methods>==--
}
#endif
#endif // NW_OS