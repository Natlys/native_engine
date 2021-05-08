#ifndef NC_APP_WINDOW_H
#define NC_APP_WINDOW_H
#include "nc_app_core.hpp"
#if (defined NC_WAPI)
namespace NC
{
	class NC_API app_wnd_t : public mem_user_t
	{
	public:
		using wnd_t = app_wnd_t;
		using wnd_tc = const wnd_t;
		using handle_t = window_handle;
		using handle_tc = const handle_t;
		using nc_event_t = nc_event_window_t;
		using event_tc = const nc_event_t;
		using icon_t = gfx_img_t;
		using icon_tc = const icon_t;
	public:
		app_wnd_t();
		virtual ~app_wnd_t();
		// getters //
		inline handle_t& get_handle()        { return m_handle; }
		inline handle_tc& get_handle() const { return m_handle; }
		inline v1u_tc get_size() const     { return m_viewp[2] * m_viewp[3]; }
		inline cv2u get_size_xy() const  { return cv2u{ m_viewp[2], m_viewp[3] }; }
		inline v1u_tc get_size_x() const   { return m_viewp[2]; }
		inline v1u_tc get_size_y() const   { return m_viewp[3]; }
		inline cv2u get_coord_xy() const { return cv2u{ m_viewp[0], m_viewp[1] }; }
		inline v1u_tc get_coord_x() const  { return m_viewp[0]; }
		inline v1u_tc get_coord_y() const  { return m_viewp[1]; }
		inline cv4u get_viewp_xywh() const { return m_viewp; }
		inline v1f_tc get_size_ratio() const { return NC_CAST_FLOAT(get_size_x()) / NC_CAST_FLOAT(get_size_y()); }
		inline v1u_tc get_opacity() const  { return m_opacity; }
		inline cstr_t get_title() const  { return &m_title[0]; }
		// setters //
		wnd_t& set_size_xy(cv2u size_xy);
		inline wnd_t& set_size_x(v1u_tc dims) { return set_size_xy({dims, get_size_y()}); }
		inline wnd_t& set_size_y(v1u_tc size_y) { return set_size_xy({ get_size_x(), size_y }); }
		inline wnd_t& set_size_xy(v1u_tc dims, v1u_tc size_y) { return set_size_xy({ dims, size_y }); }
		wnd_t& set_coord_xy(cv2u coord_xy);
		inline wnd_t& set_coord_x(v1u_tc coord_x) { return set_size_xy({ coord_x, get_coord_y() }); }
		inline wnd_t& set_coord_y(v1u_tc coord_y) { return set_size_xy({ get_coord_x(), coord_y }); }
		inline wnd_t& set_coord_xy(v1u_tc coord_x, v1u_tc coord_y) { return set_coord_xy({ coord_x, coord_y }); }
		wnd_t& set_viewp_xywh(cv4u viewport);
		inline wnd_t& set_viewp_xywh(cv2u coord_xy, cv2u size_xy) { return set_viewp_xywh({ coord_xy[0], coord_xy[1], size_xy[0], size_xy[1] }); }
		inline wnd_t& set_viewp_xywh(v1u_tc coord_x, v1u_tc coord_y, v1u_tc dims, v1u_tc size_y) { return set_viewp_xywh({ coord_x, coord_y, dims, size_y }); }
		wnd_t& set_title(cstr_t title);
		wnd_t& set_enabled(v1bit_t is_enabled);
		wnd_t& set_hovered(v1bit_t is_hovered);
		wnd_t& set_focused(v1bit_t is_focused);
		wnd_t& set_opacity(v1f_t opacity);
		wnd_t& set_icon(icon_tc& icon);
		wnd_t& set_callback(event_callback_tc& event_proc);
		// predicates //
		inline v1bit_t is_enabled() const { return m_is_enabled; }
		inline v1bit_t is_hovered() const { return m_is_hovered; }
		inline v1bit_t is_focused() const { return m_is_focused; }
		// commands //
		v1nil_t update();
		v1bit_t remake();
	protected:
#	if (NC_WAPI & NC_WAPI_WIN)
		static LRESULT WINAPI event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT WINAPI event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		inline LRESULT WINAPI event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#	endif
	protected:
		handle_t m_handle;
#	if (NC_WAPI & NC_WAPI_WIN)
		MSG m_msg;
		WNDCLASSEX m_class;
#	endif
		dstr_t m_title;
		dstr_t m_ver_str;
		v4u m_viewp;
		v1f_t m_ratio;
		v1f_t m_opacity;
		v1bit_t m_is_enabled = NC_FALSE;
		v1bit_t m_is_hovered = NC_FALSE;
		v1bit_t m_is_focused = NC_FALSE;
		event_callback_t m_event_proc;
	};
}
#endif	// NC_WAPI
#endif	// NC_APP_WINDOW_H
