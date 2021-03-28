#ifndef NWAPP_WINDOW_H
#define NWAPP_WINDOW_H
#include "nwapp_core.hpp"
#if (defined NW_WAPI)
namespace NWAPP
{
	/// window_info struct
	struct NW_API window_info : public a_info
	{
	public:
		dstr title = "default";
		dstr api_version = "default";
		v1u coord_x = 0, coord_y = 0;
		v1u size_x = 800, size_y = 600;
		v1f aspect_ratio = 800.0f / 600.0f;
		v1f opacity = 0.0f;
		v1b is_hovered = false;
		v1b is_focused = false;
		v1b is_enabled = false;
		event_callback event_proc = [](a_event&)->void { return; };
	public:
		window_info(cstr window_title = "default", v1u width = 800, v1u height = 600);
		// --operators
		virtual stm_out& operator<<(stm_out& stm) const override;
		virtual stm_in& operator>>(stm_in& stm) override;
	};
}
namespace NWAPP
{
	class NW_API app_wnd : public a_mem_user
	{
	public:
		using info = window_info;
		using cinfo = const window_info;
		using handle = window_handle;
		using ñhandle = const window_handle;
	public:
		app_wnd(cinfo& information);
		virtual ~app_wnd();
		// --getters
		inline cinfo& get_info() const     { return m_info; }
		inline v2u get_coord() const       { return { m_info.coord_x, m_info.coord_y }; }
		inline v1u get_coord_x() const     { return m_info.coord_x; }
		inline v1u get_coord_y() const     { return m_info.coord_y; }
		inline v2u get_size() const        { return { m_info.size_x, m_info.size_y }; }
		inline v1u get_size_x() const      { return m_info.size_x; }
		inline v1u get_size_y() const      { return m_info.size_y; }
		inline v4u get_viewport() const    { return { m_info.coord_x, m_info.coord_y, m_info.size_x, m_info.size_y }; }
		inline v1u get_opacity() const     { return m_info.opacity; }
		inline cstr get_title() const      { return &m_info.title[0]; }
		inline handle& get_handle()        { return m_handle; }
		inline ñhandle& get_handle() const { return m_handle; }
		// --setters
		void set_title(cstr title);
		void set_focused(v1b is_focused);
		void set_enabled(v1b enable);
		void set_opacity(v1f opacity);
		void set_icon(const a_img_cmp& img);
		void set_callback(const event_callback& event_proc);
		// --predicates
		inline v1b is_hovered() const { return m_info.is_hovered; }
		inline v1b is_focused() const { return m_info.is_focused; }
		inline v1b is_enabled() const { return m_info.is_enabled; }
		// --core_methods
		void update();
	protected:
#if (NW_WAPI & NW_WAPI_WIN)
		static LRESULT __stdcall event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT __stdcall event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		inline LRESULT __stdcall event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif
	protected:
		info m_info;
		handle m_handle;
#if (NW_WAPI & NW_WAPI_WIN)
		MSG m_msg;
		WNDCLASSEX m_class;
#endif
	};
}
#endif	// NW_WAPI
#endif	// NWAPP_WINDOW_H
