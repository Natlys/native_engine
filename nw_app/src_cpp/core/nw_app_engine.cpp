#include "nw_app_pch.hpp"
#include "nw_app_engine.h"
#if (defined NW_API)
namespace NW
{
	app_engine::app_engine() : std_engine_t(), m_window(window_t()), m_keybod(nw_keybod_t()), m_cursor(cursor_t()), m_timer(timer_t()) { }
	app_engine::~app_engine() { }
	// setters //
	app_engine::std_engine_t& app_engine::set_keybod_enabled(v1bit_t enable) {
		m_keybod.set_enabled(enable);
		gui_set_cursor_enabled(enable);
		return *this;
	}
	app_engine::std_engine_t& app_engine::set_cursor_enabled(v1bit_t enable) {
		m_cursor.set_enabled(enable);
		gui_set_cursor_enabled(enable);
		if (enable) { set_cursor_bounds(cv4s(NW_NULL)); }
		else { set_cursor_bounds(get_window()->get_viewp_xywh()); }
		return *this;
	}
	app_engine::std_engine_t& app_engine::set_cursor_bounds(cv4s& bounds) {
		m_cursor.set_bounds(bounds);
		return *this;
	}
	// commands //
	v1bit_t app_engine::init()
	{
		NW_CHECK(mem_sys::get().init(NW_MAX_MEMORY), "init error!", return NW_FALSE);
		NW_CHECK(iop_sys::get().init(), "init error!", return NW_FALSE);
		NW_CHECK(cmp_sys::get().init(), "init error!", return NW_FALSE);
		NW_CHECK(ent_sys::get().init(), "init error!", return NW_FALSE);

		NW_CHECK(m_window.remake(NW_NAME_STR, 1200u, 800u), "remake error!", return NW_FALSE);
		m_window.set_callback([this](nw_event_t& evt)->v1bit_t { return this->event_proc(evt); });

#		if (NW_FLOWS & NW_FLOWS_SINGLE)
		NW_CHECK(gfx_engine::get().set_window(m_window.get_handle()).init(), "init error!", return NW_FALSE);
		//NW_CHECK(cmd_engine::get().init(), "init error!", return NW_FALSE);
#		endif	// FLOWS_SINGLE
#		if (NW_FLOWS & NW_FLOWS_MULTIP)
		//NW_CHECK(gfx_engine::get().set_window(m_window.get_handle()).work_init(), "init error!", return NW_FALSE);
		//NW_CHECK(cmd_engine::get().work_init(), "init error!", return NW_FALSE);
#		endif	// FLOWS_MULTIP
		gfx_engine::get().set_viewp({ NW_NULL, NW_NULL, NW_CAST_FLOAT(m_window.get_size_x()), NW_CAST_FLOAT(m_window.get_size_y()) });

		NW_CHECK(std_engine_t::init(), "init error!", return NW_FALSE);

		return NW_TRUE;
	}
	v1bit_t app_engine::quit()
	{
		NW_CHECK(std_engine_t::quit(), "quit error!", return NW_FALSE);
		
		m_window = window_t();
		m_keybod = nw_keybod_t();
		m_cursor = cursor_t();
		m_timer = timer_t();

		NW_CHECK(cmp_sys::get().quit(), "quit error!", return NW_FALSE);
		NW_CHECK(ent_sys::get().quit(), "quit error!", return NW_FALSE);

#		if (NW_FLOWS & NW_FLOWS_SINGLE)
		NW_CHECK(gfx_engine::get().quit(), "quit error!", return NW_FALSE);
		//NW_CHECK(cmd_engine::get().quit(), "quit error!", return NW_FALSE);
#		endif	// FLOWS_SINGLE
#		if (NW_FLOWS & NW_FLOWS_MULTIP)
		//NW_CHECK(gfx_engine::get().work_quit(), "quit error!", return NW_FALSE);
		//if (gfx_engine::get().get_work_flow().joinable()) { gfx_engine::get().get_work_flow().join(); }
		//NW_CHECK(cmd_engine::get().work_quit(), "quit error!", return NW_FALSE);
		//if (cmd_engine::get().get_work_flow().joinable()) { cmd_engine::get().get_work_flow().join(); }
#		endif	// FLOWS_MULTIP

		NW_CHECK(iop_sys::get().quit(), "quit error!", return NW_FALSE);
		NW_CHECK(mem_sys::get().quit(), "quit error!", return NW_FALSE);

		return NW_TRUE;
	}
	v1bit_t app_engine::work()
	{
		m_keybod.update();
		m_cursor.update();
		m_timer.update();
		m_window.update();

#		if (NW_FLOWS & NW_FLOWS_SINGLE)
		NW_CHECK(gfx_engine::get().work(), "work error!", return NW_FALSE);
#		endif	// FLOWS_SINGLE
#		if (NW_FLOWS & NW_FLOWS_MULTIP)
#		endif	// FLOWS_MULTIP
		
		NW_CHECK(std_engine_t::work(), "work error!", return NW_FALSE);

		return NW_TRUE;
	}
	v1bit_t app_engine::event_proc(nw_event_t& evt)
	{
		if (evt.has_sort(NW_EVSORT_CURSOR)) {
			cursor_t::nw_event_t& crs_evt = evt;
			m_cursor.event_proc(crs_evt);
		}
		else if (evt.has_sort(NW_EVSORT_KEYBOD)) {
			nw_keybod_t::nw_event_t& kbd_evt = evt;
			m_keybod.event_proc(kbd_evt);
			switch (evt.get_type()) {
			case NW_EVTYPE_KEYBOD_PRESS: {
				switch (kbd_evt.get_code()) {
				case NW_KEYCODE_ESCP: { work_quit(); return NW_TRUE; break; }
				case NW_KEYCODE_F1: { m_window.set_opacity(0.1f); return NW_TRUE; break; }
				case NW_KEYCODE_F2: { m_window.set_opacity(0.2f); return NW_TRUE; break; }
				case NW_KEYCODE_F3: { m_window.set_opacity(0.3f); return NW_TRUE; break; }
				case NW_KEYCODE_F4: { m_window.set_opacity(0.4f); return NW_TRUE; break; }
				case NW_KEYCODE_F5: { m_window.set_opacity(0.5f); return NW_TRUE; break; }
				case NW_KEYCODE_F6: { m_window.set_opacity(0.6f); return NW_TRUE; break; }
				case NW_KEYCODE_F7: { m_window.set_opacity(0.7f); return NW_TRUE; break; }
				case NW_KEYCODE_F8: { m_window.set_opacity(0.8f); return NW_TRUE; break; }
				case NW_KEYCODE_F9: { m_window.set_opacity(0.9f); return NW_TRUE; break; }
				case NW_KEYCODE_F10: { m_window.set_opacity(1.0f); return NW_TRUE; break; }
				break;
				}
			}
			}
		}
		else if (evt.has_sort(NW_EVSORT_WINDOW)) {
			nw_event_window_t& wnd_evt = evt;
			switch (evt.get_type()) {
			case NW_EVTYPE_WINDOW_SIZED: {
				gfx_engine::get().set_viewp({ 0, 0, NW_CAST_FLOAT(wnd_evt.get_val_x()), NW_CAST_FLOAT(wnd_evt.get_val_y()) });
				break;
			}
			case NW_EVTYPE_WINDOW_ACTIV: {
				if (wnd_evt.is_activ()) { if (is_cursor_enabled()) { set_cursor_enabled(NW_FALSE); } }
				else { if (is_cursor_enabled()) { set_cursor_enabled(NW_TRUE); } }
				break;
			}
			case NW_EVTYPE_WINDOW_CLOSE: {
				if (wnd_evt.is_close()) {
					work_quit();
					evt.set_handled(NW_TRUE);
					return NW_TRUE;
					break;
				}
			}
		}
		}
		else if (evt.has_sort(NW_EVSORT_APPLIC)) {
			switch (evt.get_type()) {
			case NW_EVTYPE_APPLIC_DROPF: {
				break;
			}
			}
		}
		for (auto& istt : m_states) { if (evt.is_handled()) { return NW_FALSE; } istt->event_proc(evt); }
		return NW_TRUE;
	}
	// --==</core_methods>==--
}
#endif	// NW_API