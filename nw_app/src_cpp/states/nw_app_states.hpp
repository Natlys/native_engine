#ifndef NW_APP_ENGINE_STATES_H
#define NW_APP_ENGINE_STATES_H
#include "nw_app_core.hpp"
#if (defined NW_API)
namespace NW
{
	/// application_state_type
	/// description:
	/// --this is the main interaction part for the engine.
	class NW_API app_state_t : public engine_state_t
	{
	public:
		using state_t = app_state_t;
		using state_tc = const state_t;
	public:
		// ctor_dtor //
		inline app_state_t() : engine_state_t() { }
		inline app_state_t(cstr_t name) : engine_state_t(name) { }
		virtual inline ~app_state_t() { }
		// getters //
		// setters //
		// commands //
		virtual inline v1bit_t init() override { return NW_TRUE; }
		virtual inline v1bit_t quit() override { return NW_TRUE; }
		virtual inline v1bit_t work() override { return NW_TRUE; }
		virtual inline v1bit_t event_proc(nw_event_t& evt) { return NW_TRUE; }
	protected:
	};
}
namespace NW
{
	/// application_state_game_type
	class app_state_game_t : public app_state_t
	{
	public:
	public:
		// ctor_dtor //
		app_state_game_t();
		~app_state_game_t();
		// getters //
		// commands //
		virtual v1bit_t init() override;
		virtual v1bit_t quit() override;
		virtual v1bit_t work() override;
		virtual v1bit_t event_proc(nw_event_t& evt) override;
	};
}
#	include <core/nw_app_gui_of.h>
namespace NW
{
	/// application_state_gui_type
	/// description:
	class app_state_gui_t : public app_state_t
	{
	public:
		using gui_ref = mem_ref_t<a_gui_of>;
		using gui_refs = array_t<gui_ref>;
	public:
		// ctor_dtor //
		app_state_gui_t();
		~app_state_gui_t();
		// getters //
		// commands //
		virtual v1bit_t init() override;
		virtual v1bit_t quit() override;
		virtual v1bit_t work() override;
		virtual v1bit_t event_proc(nw_event_t& evt) override;
	private:
		gui_refs m_gui_refs;
	};
}
#endif	// NW_API
#endif	// NW_APP_ENGINE_STATES_H
