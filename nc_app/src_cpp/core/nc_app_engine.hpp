#ifndef NC_APP_CORE_ENGINE_H
#define NC_APP_CORE_ENGINE_H
#include "nc_app_core.hpp"
#if (defined NC_API)
#	include "nc_app_wnd_core.h"
#	include "nc_app_states.h"
#	include "std/nc_std_engine.h"
namespace NC
{
	/// app_eng class
	/// description:
	/// interface:
	class NC_API app_eng : public std_engine_t<app_eng, app_state_t>
	{
	public:
		using window_t = app_wnd_core_t;
		using window_tc = const window_t;
		using nc_keybod_t = nc_keybod_t;
		using keybod_tc = const nc_keybod_t;
		using kbd_code_t = nc_keybod_t::code_t;
		using kbd_code_tc = nc_keybod_t::code_tc;
		using cursor_t = nc_cursor_t;
		using cursor_tc = const cursor_t;
		using crs_code_t = cursor_t::code_t;
		using crs_code_tc = cursor_t::code_tc;
		//using timer_t = timer_t;
		using timer_tc = const timer_t;
	public:
		app_eng();
		~app_eng();
		// getters //
		inline window_t* get_window()        { return &m_window; }
		inline window_tc* get_window() const { return &m_window; }
		inline nc_keybod_t* get_keybod()        { return &m_keybod; }
		inline keybod_tc* get_keybod() const { return &m_keybod; }
		inline cursor_t* get_cursor()        { return &m_cursor; }
		inline cursor_tc* get_cursor() const { return &m_cursor; }
		inline timer_t* get_timer()        { return &m_timer; }
		inline timer_tc* get_timer() const { return &m_timer; }
		inline v1f_tc get_timer_curr(v1f_t scale = 1.0f) const  { return m_timer.get_curr(scale); }
		inline v1f_tc get_timer_last(v1f_t scale = 1.0f) const  { return m_timer.get_last(scale); }
		inline v1f_tc get_timer_delta(v1f_t scale = 1.0f) const { return m_timer.get_delta(scale); }
		inline v1f_tc get_timer_ups(v1f_t scale = 1.0f) const   { return m_timer.get_ups(scale); }
		inline cstr_t get_window_title() const { return m_window.get_title(); }
		inline v1u_tc get_window_size_x() const  { return m_window.get_size_x(); }
		inline v1u_tc get_window_size_y() const  { return m_window.get_size_y(); }
		inline cv2u get_window_size_xy() const { return m_window.get_size_xy(); }
		inline v1f_tc get_cursor_move_coord_x() const                 { return get_cursor()->get_move_coord_x(); }
		inline v1f_tc get_cursor_move_coord_y() const                 { return get_cursor()->get_move_coord_y(); }
		inline v1f_tc get_cursor_free_coord_x(crs_code_tc code) const { return get_cursor()->get_free_coord_x(code); }
		inline v1f_tc get_cursor_free_coord_y(crs_code_tc code) const { return get_cursor()->get_free_coord_y(code); }
		inline v1f_tc get_cursor_free_delta_x(crs_code_tc code) const { return get_cursor()->get_free_delta_x(code); }
		inline v1f_tc get_cursor_free_delta_y(crs_code_tc code) const { return get_cursor()->get_free_delta_y(code); }
		inline v1f_tc get_cursor_held_coord_x(crs_code_tc code) const { return get_cursor()->get_held_coord_x(code); }
		inline v1f_tc get_cursor_held_coord_y(crs_code_tc code) const { return get_cursor()->get_held_coord_y(code); }
		inline v1f_tc get_cursor_held_delta_x(crs_code_tc code) const { return get_cursor()->get_held_delta_x(code); }
		inline v1f_tc get_cursor_held_delta_y(crs_code_tc code) const { return get_cursor()->get_held_delta_y(code); }
		// setters //
		std_engine_t& set_keybod_enabled(v1bit_t enable);
		std_engine_t& set_cursor_enabled(v1bit_t enable);
		std_engine_t& set_cursor_bounds(cv4s& bounds);
		// predicates //
		inline v1bit_t is_keybod_enabled() const  { return m_keybod.is_enabled(); }
		inline v1bit_t is_keybod_raise(kbd_code_tc code) const { return m_keybod.is_raise(code); }
		inline v1bit_t is_keybod_press(kbd_code_tc code) const { return m_keybod.is_press(code); }
		inline v1bit_t is_keybod_free(kbd_code_tc code) const  { return m_keybod.is_free(code); }
		inline v1bit_t is_keybod_held(kbd_code_tc code) const  { return m_keybod.is_held(code); }
		inline v1bit_t is_cursor_enabled() const  { return m_cursor.is_enabled(); }
		inline v1bit_t is_cursor_raise(crs_code_tc code) const { return m_cursor.is_raise(code); }
		inline v1bit_t is_cursor_press(crs_code_tc code) const { return m_cursor.is_press(code); }
		inline v1bit_t is_cursor_free(crs_code_tc code) const  { return m_cursor.is_free(code); }
		inline v1bit_t is_cursor_held(crs_code_tc code) const  { return m_cursor.is_held(code); }
		// operators //
		// commands //
		virtual v1bit_t init() override;
		virtual v1bit_t quit() override;
		virtual v1bit_t work() override;
		v1bit_t event_proc(nc_event_t& evt);
		// --data_methods
	private:
		window_t m_window;
		nc_keybod_t m_keybod;
		cursor_t m_cursor;
		timer_t m_timer;
	};
}
#endif	// NC_API
#endif	// NC_APP_CORE_ENGINE_H
