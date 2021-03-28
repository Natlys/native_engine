#ifndef NWAPP_CORE_ENGINE_H
#define NWAPP_CORE_ENGINE_H
#include "nwapp_core.hpp"
#include "nwapp_wnd_core.h"
#include "nwapp_engine_states.h"
#if (defined NW_API)
namespace NWAPP
{
	/// core_engine class
	/// description:
	/// interface:
	class NW_API core_engine : public a_mem_user
	{
	public:
		using state = mem_ref<a_core_state>;
		using states = darray<state>;
		using window = mem_ref<app_wnd_core>;
		using graphics = app_wnd_core::graphics;
		using keyboard = app_wnd_core::keyboard;
		using mouse = app_wnd_core::mouse;
		using timer = time_state;
		using kbd_code = keyboard_codes;
		using msb_code = mouse_codes;
	public:
		core_engine(cstr name = "nwapp_engine");
		core_engine(const core_engine& copy) = delete;
		virtual ~core_engine();
		// --getters
		inline cstr get_name() const        { return &m_name[0]; }
		inline thread* get_run_thread()     { return &m_thr_run; }
		inline states& get_states()         { return m_states; }
		inline state& get_state(v1u idx)   { return m_states[idx % m_states.size()]; }
		inline window& get_window()         { return m_wnd; }
		inline graphics& get_graphics()     { return m_wnd->get_graphics(); }
		inline const mouse* get_mouse() const             { return m_wnd->get_mouse(); }
		inline const keyboard* get_keyboard() const       { return m_wnd->get_keyboard(); }
		inline const timer* get_timer() const             { return &m_timer; }
		inline v1f get_time_curr(v1f scale = 1.0f) const  { return m_timer.get_curr(scale); }
		inline v1f get_time_last(v1f scale = 1.0f) const  { return m_timer.get_last(scale); }
		inline v1f get_time_delta(v1f scale = 1.0f) const { return m_timer.get_delta(scale); }
		inline v1f get_time_ups(v1f scale = 1.0f) const   { return m_timer.get_ups(scale); }
		inline v1f get_mouse_move_coord_x() const                 { return m_wnd->get_mouse()->get_move_coord_x(); }
		inline v1f get_mouse_move_coord_y() const                 { return m_wnd->get_mouse()->get_move_coord_y(); }
		inline v1f get_mouse_free_coord_x(msb_code code) const { return m_wnd->get_mouse()->get_free_coord_x(code); }
		inline v1f get_mouse_free_coord_y(msb_code code) const { return m_wnd->get_mouse()->get_free_coord_y(code); }
		inline v1f get_mouse_free_delta_x(msb_code code) const { return m_wnd->get_mouse()->get_free_delta_x(code); }
		inline v1f get_mouse_free_delta_y(msb_code code) const { return m_wnd->get_mouse()->get_free_delta_y(code); }
		inline v1f get_mouse_held_coord_x(msb_code code) const { return m_wnd->get_mouse()->get_held_coord_x(code); }
		inline v1f get_mouse_held_coord_y(msb_code code) const { return m_wnd->get_mouse()->get_held_coord_y(code); }
		inline v1f get_mouse_held_delta_x(msb_code code) const { return m_wnd->get_mouse()->get_held_delta_x(code); }
		inline v1f get_mouse_held_delta_y(msb_code code) const { return m_wnd->get_mouse()->get_held_delta_y(code); }
		// --setters
		void set_cursor_enabled(v1b enable);
		template<class stype, typename ... args>
		state& new_state(args ... arguments);
		void del_state(v1u idx);
		void stop_running();
		// --predicates
		inline v1b is_running() const                    { return m_is_running; }
		inline v1b is_cursor_enabled() const             { return m_wnd->is_cursor_enabled(); }
		inline v1b is_mouse_released(msb_code code)const { return m_wnd->get_mouse()->is_released(code); }
		inline v1b is_mouse_pressed(msb_code code) const { return m_wnd->get_mouse()->is_pressed(code); }
		inline v1b is_mouse_free(msb_code code) const    { return m_wnd->get_mouse()->is_free(code); }
		inline v1b is_mouse_held(msb_code code) const    { return m_wnd->get_mouse()->is_held(code); }
		inline v1b is_key_released(kbd_code code) const  { return m_wnd->get_keyboard()->is_released(code); }
		inline v1b is_key_pressed(kbd_code code) const   { return m_wnd->get_keyboard()->is_pressed(code); }
		inline v1b is_key_free(kbd_code code) const      { return m_wnd->get_keyboard()->is_free(code); }
		inline v1b is_key_held(kbd_code code) const      { return m_wnd->get_keyboard()->is_held(code); }
		// --operators
		void operator=(const core_engine& copy) = delete;
		// --core_methods
		bool init();
		void quit();
		void run();
		void update();
		void event_proc(a_event& evt);
		// --data_methods
		dstr dialog_load(cstr filter);
		dstr dialog_save(cstr filter);
	private:
		cstr m_name;
		thread m_thr_run;
		v1b m_is_running;
		states m_states;
		window m_wnd;
		timer m_timer;
	};
	template<class stype, typename ... args>
	core_engine::state& core_engine::new_state(args ... arguments) {
		m_states.push_back(state());
		m_states.back().make_ref<stype>(*this, std::forward<args>(arguments)...);
		return m_states.back();
	}
}
#endif	// NW_API
#endif	// NWAPP_CORE_ENGINE_H