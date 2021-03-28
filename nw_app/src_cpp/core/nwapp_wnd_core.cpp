#include "nwapp_pch.hpp"
#include "nwapp_wnd_core.h"
#if (defined NW_WAPI)
#include <core/nwapp_engine.h>
#include "nwapp_wnd_sub.h"
#if (NW_WAPI & NW_WAPI_WIN)
#include <shellapi.h>
namespace NWAPP
{
	app_wnd_core::app_wnd_core(cinfo& information) :
		app_wnd(information),
		m_gfx(graphics()), m_kbd(keyboard()), m_mouse(mouse())
	{
		// register a window class to create a window; ModuleHandle is the current application;
		m_class.lpfnWndProc = app_wnd_core::event_proc_init;
		m_class.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
		if (::RegisterClassEx(&m_class) == FALSE) { throw init_error(__FILE__, __LINE__); return; }
		// WS_OVERLAPPEDWINDOW is the same as:
		// (WS_OVERLAPPED | WS_CAPTION | WS_SYS_MENU | WS_THICKFRAME | WS_MINIMIZE_BOX | WS_MAXIMIZE_BOX);
		// WS_TILEDWINDOW is the same as:
		// (WS_TILED | WS_CAPTION | WS_SYS_MENU | WS_THICKFRAME | WS_MINIMIZE_BOX | WS_MAXIMIZE_BOX);
		DWORD style = 0;
		// control styles;
		style |= WS_CAPTION;		// title bar;
		style |= WS_SYSMENU;		// must be used with WS_CAPTION;
		style |= WS_MINIMIZEBOX;	// button sizing;
		style |= WS_MAXIMIZEBOX;	// button sizing;
		style |= WS_SIZEBOX;		// arrow-border sizing;
		//style |= WS_HSCROLL;		// horizontal scrollbar;
		//style |= WS_VSCROLL;		// vertical scrollbar;
		// view styles;
		style |= WS_OVERLAPPED;		// title bar and border; same as WS_TILED;
		style |= WS_TILED;			// title bar and border; same as WS_OVERLAPPED;
		style |= WS_BORDER;			// thin-line border;
		style |= WS_THICKFRAME;		// ;
		// initial styles;
		style |= WS_VISIBLE;		// initialy visible
		// style |= WS_DISABLED;	// initialy disabled;
		// style |= WS_ICONIC;		// initialy minimized;
		style |= WS_MAXIMIZE;		// initialy maximized;
		// extended styles
		// WS_EX_OVERLAPPEDWINDOW is the same as
		// (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE);
		// WS_EX_PALETTEWINDOW is the same as
		// (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
		DWORD style_ex = 0;
		style_ex |= WS_EX_APPWINDOW;	// force top window onto the taskbar whe it is visible;
		style_ex |= WS_EX_ACCEPTFILES;	// drag-drop files;
		// style_ex |= WS_EX_CONTEXTHELP;	// add an icon which makes WM_HELP message and changes the cursor to question mark;
		// style_ex |= WS_EX_LAYERED;		// cannot be used with CS_OWNDC or CS_CLASSDC;
		style_ex |= WS_EX_LEFT;			// generic left-aligned properties;
		//style_ex |= WS_EX_RIGHT;		// generic right-aligned properties;
		// style_ex |= WS_EX_TOOLWINDOW;	// the window becomes a floating toolbar;
		// style_ex |= WS_EX_TRANSPARENT;	// the window should not be draw until siblings beneath it;
		style_ex |= WS_EX_WINDOWEDGE;	// the window has a border with a raised edge
		// adjust sizes;
		RECT wnd_rect = { 100, 100, 100 + static_cast<LONG>(get_size_x()), 100 + static_cast<LONG>(get_size_y()) };
		::AdjustWindowRect(&wnd_rect, style, FALSE);
		// create the window itself;
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
		// do some stuff with the window;
		::ShowWindow(m_handle, SW_SHOWDEFAULT);
		::UpdateWindow(m_handle);
		set_title(get_title());
		// register mouse raw input device;
		RAWINPUTDEVICE raw_dvc;
		raw_dvc.usUsagePage = 0x01;	// mouse page bit;
		raw_dvc.usUsage = 0x02;		// mouse usage bit;
		raw_dvc.dwFlags = 0u;		// additional param;
		raw_dvc.hwndTarget = NULL;	// we don't really need a target;
		if (::RegisterRawInputDevices(&raw_dvc, 1u, sizeof(RAWINPUTDEVICE)) == FALSE) {
			throw init_error(__FILE__, __LINE__);
			return;
		}

		m_gfx.make_ref<gfx_engine>(m_handle);
		m_gfx->set_viewport(0, 0, get_size_x(), get_size_y());
		m_gfx->set_fbuf_size(get_size_x(), get_size_y());
	}
	app_wnd_core::~app_wnd_core()
	{
	}
	// --setters

	void app_wnd_core::set_cursor_enabled(v1b enable_cursor) {
		m_mouse.set_cursor_enabled(enable_cursor);
		NWGUI::gui_set_mouse_enabled(enable_cursor);
	}
	// --==<core_methods>==--
	void app_wnd_core::update()
	{
		m_gfx->update();
		m_mouse.update();
		m_kbd.update();
		app_wnd::update();
	}
	// --==</core_methods>==--
	// --==<impl_methods>==--
	LRESULT inline __stdcall app_wnd_core::event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg != WM_NCCREATE) { return ::DefWindowProc(hwnd, msg, wparam, lparam); }

		CREATESTRUCT* crtst = reinterpret_cast<CREATESTRUCT*>(lparam);
		app_wnd* nwapp_window = reinterpret_cast<app_wnd*>(crtst->lpCreateParams);

		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nwapp_window));
		::SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(app_wnd_core::event_proc_static));

		return app_wnd_core::event_proc_static(hwnd, msg, wparam, lparam);
	}
	LRESULT inline __stdcall app_wnd_core::event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (NWGUI::gui_wapi_event_proc(hwnd, msg, wparam, lparam) == TRUE) { return 0l; }
		return reinterpret_cast<app_wnd_core*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA))->event_proc(hwnd, msg, wparam, lparam);
	}
	inline LRESULT __stdcall app_wnd_core::event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		switch (msg) {
		// events_of_application
		{
		case WM_CREATE: { return 0l; break; }
		case WM_DESTROY: { return 0l; break; }
		case WM_QUIT: { return 0l; break; }
		case WM_PAINT: { return 0l; break; }
		case WM_ERASEBKGND: { return 0l; break; }
		case WM_DROPFILES: {	// wparam is HDROP; lparam is not used;
			TCHAR str_buf[MAX_PATH]{ 0 };
			HDROP drop_info = reinterpret_cast<HDROP>(wparam);
			UINT drop_result = DragQueryFile(drop_info, 0u, str_buf, MAX_PATH);	// we can retrieve the file path;
			if (drop_result == FALSE) { throw run_error(__FILE__, __LINE__); }
			DragFinish(drop_info);	// free that resources that windows has just taken to help us;
			DWORD attribs = GetFileAttributes(str_buf);
			if (attribs & FILE_ATTRIBUTE_ARCHIVE) {
				std::cout << "a file or directory that is an archieve file or directory;" << std::endl;
				std::cout << "usually used to mark files for backup or removal;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_COMPRESSED) {
				std::cout << "all file data is compressed;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_DEVICE) {
				std::cout << "the value is reserved for system use;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_DIRECTORY) {
				std::cout << "the file identifies a directory;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_EA) {
				std::cout << "the value is reserved for system use;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_VIRTUAL) {
				std::cout << "the value is reserved for system use;" << std::endl;
			}
			if (attribs & FILE_ATTRIBUTE_NORMAL) {
				std::cout << "a file does not have other attributes set;" << std::endl;
			}
			app_event app_evt(EVT_APP_DROP_FILE, str_buf);
			m_info.event_proc(app_evt);
			return 0l; break;
		}
		}
		// events_of_input
		{
		case WM_INPUT: {
			UINT in_data_size = 0u;
			if (::GetRawInputData(	//  fill the number of bytes variable for the data;
				reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, NULL, &in_data_size, sizeof(RAWINPUTHEADER)) == -1) {
				break;
			}
			// error with NULL returns -1, but we just bail that;
			m_raw_buf.resize(in_data_size);
			if (::GetRawInputData(	// fill data buffer in this time;
				reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, &m_raw_buf[0], &in_data_size, sizeof(RAWINPUTHEADER)) != in_data_size) {
				break;
			}
			// error with reading returns number of the read bytes, bail it again;
			// raw input data processing;
			const RAWINPUT& raw_in = reinterpret_cast<const RAWINPUT&>(m_raw_buf[0]);
			// we are interested in the header and data;
			if (raw_in.header.dwType == RIM_TYPEMOUSE) {
				if (raw_in.data.mouse.lLastX != 0 || raw_in.data.mouse.lLastY != 0) {
					ms_event ms_evt = ms_event(EVT_MS_MOVE, raw_in.data.mouse.lLastX, raw_in.data.mouse.lLastY);
					m_mouse.event_proc(ms_evt);
					m_info.event_proc(ms_evt);
				}
			}
			break;
		}
		}
		// events_of_mouse
		{
		case WM_MOUSEMOVE: {
			POINTS coord = MAKEPOINTS(lparam);
			ms_event ms_evt = ms_event(EVT_MS_COORD, coord.x, coord.y);
			m_mouse.event_proc(ms_evt);
			m_info.event_proc(ms_evt);
			return 0l;
			break;
		}
		case WM_MOUSEHWHEEL: {
			ms_event ms_evt = ms_event(EVT_MS_SCROLL, GET_WHEEL_DELTA_WPARAM(wparam) / static_cast<v1f64>(WHEEL_DELTA), 0.0);
			m_mouse.event_proc(ms_evt);
			m_info.event_proc(ms_evt);
			return 0l;
			break;
		}
		case WM_MOUSEWHEEL: {
			ms_event ms_evt = ms_event(EVT_MS_SCROLL, 0.0, GET_WHEEL_DELTA_WPARAM(wparam) / static_cast<v1f64>(WHEEL_DELTA));
			m_mouse.event_proc(ms_evt);
			m_info.event_proc(ms_evt);
			return 0l;
			break;
		}
		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
		{
			mouse_codes msb_code = MSC_DEFAULT;
			if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { msb_code = MSC_LEFT; }
			if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { msb_code = MSC_RIGHT; }
			if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { msb_code = MSC_MIDDLE; }
			if (::GetCapture() == nullptr) { ::SetCapture(m_handle); }
			ms_event ms_evt = ms_event(EVT_MS_PRESSED, msb_code);
			m_mouse.event_proc(ms_evt);
			m_info.event_proc(ms_evt);
			return 0l;
			break;
		}
		}
		{
		case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP: {
			mouse_codes msb_code = MSC_DEFAULT;
			if (msg == WM_LBUTTONUP) { msb_code = MSC_LEFT; }
			if (msg == WM_RBUTTONUP) { msb_code = MSC_RIGHT; }
			if (msg == WM_MBUTTONUP) { msb_code = MSC_MIDDLE; }
			ms_event ms_evt = ms_event(EVT_MS_RELEASED, msb_code);
			m_mouse.event_proc(ms_evt);
			m_info.event_proc(ms_evt);
			if (::GetCapture() == m_handle) { ::ReleaseCapture(); }
			return 0l;
			break;
		}
		// events_of_keyboard
		case WM_KEYDOWN: case WM_SYSKEYDOWN: {
			kbd_event kbd_evt = kbd_event(EVT_KBD_PRESSED, static_cast<keyboard_codes>(wparam));
			m_kbd.event_proc(kbd_evt);
			m_info.event_proc(kbd_evt);
			return 0l;
			break;
		}
		case WM_KEYUP: case WM_SYSKEYUP: {
			kbd_event kbd_evt = kbd_event(EVT_KBD_RELEASED, static_cast<keyboard_codes>(wparam));
			m_kbd.event_proc(kbd_evt);
			m_info.event_proc(kbd_evt);
			return 0l;
			break;
		}
		case WM_CHAR: case WM_SYSCHAR: {
			kbd_event kbd_evt = kbd_event(EVT_KBD_CHAR, static_cast<keyboard_codes>(wparam));
			m_kbd.event_proc(kbd_evt);
			m_info.event_proc(kbd_evt);
			return 0l;
			break;
		}
		}
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
		case WM_ACTIVATE: {
			if (m_mouse.is_cursor_enabled()) {
				if (wparam & WA_ACTIVE) { m_mouse.set_cursor_enabled(false); }
				else { m_mouse.set_cursor_enabled(true); }
			}
			break;
		}
		}
		}
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	// --==</impl_methods>==--
}
#endif
#endif // NW_WAPI