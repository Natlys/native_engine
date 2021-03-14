#ifndef NW_CORE_ENGINE_H
#define NW_CORE_ENGINE_H
#include <nw_core.hpp>
#include <core/nw_window.h>
#include <core/nw_engine_states.h>;
namespace NW
{
	/// core_engine class
	/// description:
	/// interface:
	class NW_API core_engine : public a_mem_user
	{
	public:
		using state = a_core_state;
		using states = darray<state*>;
		using window = mem_ref<core_window>;
		using graphics = mem_ref<gfx_engine>;
		using keyboard = keyboard_state;
		using mouse = mouse_state;
		using timer = time_state;
	public:
		core_engine(cstring name = "nw_engine");
		core_engine(const core_engine& copy) = delete;
		virtual ~core_engine();
		// --getters
		inline cstring get_name()			{ return &m_name[0]; }
		inline thread* get_run_thread()		{ return &m_thr_run; }
		inline window& get_window()			{ return m_wnd; }
		inline graphics& get_graphics()		{ return m_gfx; }
		inline state* get_state(ui32 idx)	{ return m_states[idx]; }
		inline const mouse* get_mouse() const		{ return m_wnd->get_mouse(); }
		inline const keyboard* get_keyboard() const	{ return m_wnd->get_keyboard(); }
		inline const timer* get_timer() const		{ return &m_timer; }
		// --setters
		void add_state(state& state);
		void rmv_state(ui8 idx);
		void stop_running();
		// --predicates
		inline bit is_running() const { return m_is_running; }
		// --operators
		void operator=(const core_engine& copy) = delete;
		// --core_methods
		bool init();
		void quit();
		void run();
		void update();
		void event_proc(a_event& evt);
		// --data_methods
		dstring dialog_load(cstring filter);
		dstring dialog_save(cstring filter);
	private:
		cstring m_name;
		thread m_thr_run;
		bit m_is_running;
		window m_wnd;
		graphics m_gfx;
		states m_states;
		time_state m_timer;
	};
}
#endif	// NW_CORE_ENGINE_H