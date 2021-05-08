#ifndef NW_APP_SUB_WINDOW_H
#define NW_APP_SUB_WINDOW_H
#include "nw_app_core.hpp"
#if (defined NW_WAPI)
#	include "nw_app_wnd.h"
namespace NW
{
	class NW_API app_wnd_sub : public app_wnd_t
	{
	public:
		app_wnd_sub(app_wnd_core_t& core);
		virtual ~app_wnd_sub();
		// getters //
		// setters //
		// commands //
		v1nil_t update();
		v1bit_t remake();
	protected:
#	if (NW_WAPI & NW_WAPI_WIN)
		static LRESULT WINAPI event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT WINAPI event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		inline LRESULT WINAPI event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#	endif
	protected:
		app_wnd_core_t* m_core;
	};
}
#endif	// NW_WAPI
#endif	// NW_APP_SUB_WINDOW_H
