#ifndef NW_APP_CORE_WINDOW_H
#define NW_APP_CORE_WINDOW_H
#include "nw_app_core.hpp"
#if (defined NW_WAPI)
#	include "nw_app_wnd.h"
namespace NW
{
	/// application_window_core_type
	class NW_API app_wnd_core_t : public app_wnd_t
	{
	public:
		using wnd_core_t = app_wnd_core_t;
		using raw_buf_t = array_t<sbyte_t>;
	public:
		app_wnd_core_t();
		virtual ~app_wnd_core_t();
		// getters //
		// setters //
		// predicates //
		// commands //
		v1nil_t update();
		v1bit_t remake();
		inline v1bit_t remake(cv2u size_xy) { set_size_xy(size_xy); return remake(); }
		inline v1bit_t remake(cstr_t title) { set_title(title); return remake(); }
		inline v1bit_t remake(cstr_t title, cv2u size_xy) { set_title(title); set_size_xy(size_xy); return remake(); }
		inline v1bit_t remake(cstr_t title, v1u_tc dims, v1u_tc size_y) { set_title(title); set_size_x(dims); set_size_y(size_y); return remake(); }
#	if (NW_WAPI & NW_WAPI_WIN)
		static LRESULT WINAPI event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT WINAPI event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		inline LRESULT WINAPI event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#	endif	// WAPI_WIN
	private:
#	if (NW_WAPI & NW_WAPI_WIN)
		raw_buf_t m_raw_buf;
#	endif
	};
}
#endif	// NW_WAPI
#endif	// NW_APP_CORE_WINDOW_H
