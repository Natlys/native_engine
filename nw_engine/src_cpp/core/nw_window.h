#ifndef NW_CORE_WINDOW_H
#define NW_CORE_WINDOW_H
#include <nw_core.hpp>
#if (defined NW_OS)
namespace NW
{
	/// window_info struct
	struct NW_API window_info : public a_info
	{
	public:
		dstring title = "default";
		dstring api_version = "default";
		ui16 coord_x = 0, coord_y = 0;
		ui16 size_x = 800, size_y = 600;
		f32 aspect_ratio = 800.0f / 600.0f;
		f32 opacity= 0.0f;
		bit is_hovered = false;
		bit is_focused = false;
		bit is_enabled = false;
		event_callback event_proc = [](a_event&)->void { return; };
	public:
		window_info(cstring window_title = "default", ui16 width = 800, ui16 height = 600);
		// --operators
		virtual out_stream& operator<<(out_stream& stm) const override;
		virtual in_stream& operator>>(in_stream& stm) override;
	};
}
namespace NW
{
	/// core_window class
	class NW_API core_window : public a_mem_user
	{
		using info = window_info;
		using handle = window_handle;
		using keyboard = keyboard_state;
		using mouse = mouse_state;
	public:
		core_window(const info& info);
		core_window(const core_window& copy) = delete;
		virtual ~core_window();
		// --getters
		inline ui16 get_coord_x() const		{ return m_info.coord_x; }
		inline ui16 get_coord_y() const		{ return m_info.coord_y; }
		inline ui16 get_size_x() const		{ return m_info.size_x; }
		inline ui16 get_size_y() const		{ return m_info.size_y; }
		inline ui16 get_opacity() const		{ return m_info.opacity; }
		inline cstring get_title() const	{ return &m_info.title[0]; }
		inline const info& get_info() const	{ return m_info; }
		inline handle* get_native()					{ return &m_native; }
		inline const handle* get_native() const		{ return &m_native; }
		inline const mouse* get_mouse()	const		{ return &m_mouse; }
		inline const keyboard* get_keyboard() const	{ return &m_kbd; }
		// --setters
		void set_title(cstring title);
		void set_focused(bit is_focused);
		void set_enabled(bit enable);
		void set_cursor_enabled(bit enable_cursor);
		void set_opacity(f32 opacity);
		void set_icon(const a_image& img);
		void set_callback(const event_callback& event_proc);
		// --predicates
		inline bit is_hovered() const			{ return m_info.is_hovered; }
		inline bit is_focused() const			{ return m_info.is_focused; }
		inline bit is_enabled() const			{ return m_info.is_enabled; }
		inline bit is_cursor_enabled() const	{ return m_mouse.is_cursor_enabled(); }
		// --operators
		void operator=(const core_window& copy) = delete;
		// --core_methods
		void update();
#if (NW_OS & NW_OS_WIN)
		static LRESULT __stdcall event_proc_init(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT __stdcall event_proc_static(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
		LRESULT __stdcall event_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif
	private:
		info m_info;
		handle m_native;
		keyboard m_kbd;
		mouse m_mouse;
#if (NW_OS & NW_OS_WIN)
		MSG m_msg;
		WNDCLASSEX m_class;
		PAINTSTRUCT m_pts;
		darray<sbyte> raw_buf;
#endif
	};
}
#endif	// NW_OS
#endif	// NW_CORE_WINDOW_H
/*
* Development started 18.10.2020
* --Tutorials used:
* -->Cherno GameEngine series
* -->MihailRis - Minecraft Voxel Engine
*/