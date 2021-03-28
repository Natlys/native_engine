#ifndef NWAPP_SUB_WINDOW_H
#define NWAPP_SUB_WINDOW_H
#include "nwapp_core.hpp"
#if (defined NW_WAPI)
#include "nwapp_wnd.h"
namespace NWAPP
{
	class NW_API app_wnd_sub : public app_wnd
	{
	public:
		app_wnd_sub(app_wnd_core& core, cinfo& information);
		virtual ~app_wnd_sub();
		// --getters
		// --setters
		// --core_methods
		void update();
	protected:
#if (NW_WAPI & NW_WAPI_WIN)
		static LRESULT __stdcall event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT __stdcall event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		inline LRESULT __stdcall event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif
	protected:
		app_wnd_core* m_core;
	};
}
#endif	// NW_WAPI
#endif	// NWAPP_SUB_WINDOW_H
