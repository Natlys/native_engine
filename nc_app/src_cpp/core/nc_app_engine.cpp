#include "nc_app_pch.hpp"
#include "nc_app_eng.h"
#if (defined NC_API)
namespace NC
{
	app_eng::app_eng() : std_engine_t(), m_window(window_t()), m_keybod(nc_keybod_t()), m_cursor(cursor_t()), m_timer(timer_t()) { }
	app_eng::~app_eng() { }
	// setters //
	app_eng::std_engine_t& app_eng::set_keybod_enabled(v1bit_t enable) {
		m_keybod.set_enabled(enable);
		gui_set_cursor_enabled(enable);
		return *this;
	}
	app_eng::std_engine_t& app_eng::set_cursor_enabled(v1bit_t enable) {
		m_cursor.set_enabled(enable);
		gui_set_cursor_enabled(enable);
		if (enable) { set_cursor_bounds(cv4s(NC_NULL)); }
		else { set_cursor_bounds(get_window()->get_viewp_xywh()); }
		return *this;
	}
	app_eng::std_engine_t& app_eng::set_cursor_bounds(cv4s& bounds) {
		m_cursor.set_bounds(bounds);
		return *this;
	}
	// commands //
	v1bit_t app_eng::init()
	{
		NC_CHECK(mem_sys::get().init(NC_MAX_MEMORY), "init error!", return NC_FALSE);
		NC_CHECK(iop_sys::get().init(), "init error!", return NC_FALSE);
		NC_CHECK(cmp_sys::get().init(), "init error!", return NC_FALSE);
		NC_CHECK(ent_sys::get().init(), "init error!", return NC_FALSE);

		NC_CHECK(m_window.remake(NC_NAME_STR, 1200u, 800u), "remake error!", return NC_FALSE);
		m_window.set_callback([this](nc_event_t& evt)->v1bit_t { return this->event_proc(evt); });

#		if (NC_FLOWS & NC_FLOWS_SINGLE)
		NC_CHECK(nc_gfx_eng::get().set_window(m_window.get_handle()).init(), "init error!", return NC_FALSE);
		//NC_CHECK(cmd_engine::get().init(), "init error!", return NC_FALSE);
#		endif	// FLOWS_SINGLE
#		if (NC_FLOWS & NC_FLOWS_MULTIP)
		//NC_CHECK(nc_gfx_eng::get().set_window(m_window.get_handle()).work_init(), "init error!", return NC_FALSE);
		//NC_CHECK(cmd_engine::get().work_init(), "init error!", return NC_FALSE);
#		endif	// FLOWS_MULTIP
		nc_gfx_eng::get().set_viewp({ NC_NULL, NC_NULL, NC_CAST_FLOAT(m_window.get_size_x()), NC_CAST_FLOAT(m_window.get_size_y()) });

		NC_CHECK(std_engine_t::init(), "init error!", return NC_FALSE);

		return NC_TRUTH;
	}
	v1bit_t app_eng::quit()
	{
		NC_CHECK(std_engine_t::quit(), "quit error!", return NC_FALSE);
		
		m_window = window_t();
		m_keybod = nc_keybod_t();
		m_cursor = cursor_t();
		m_timer = timer_t();

		NC_CHECK(cmp_sys::get().quit(), "quit error!", return NC_FALSE);
		NC_CHECK(ent_sys::get().quit(), "quit error!", return NC_FALSE);

#		if (NC_FLOWS & NC_FLOWS_SINGLE)
		NC_CHECK(nc_gfx_eng::get().quit(), "quit error!", return NC_FALSE);
		//NC_CHECK(cmd_engine::get().quit(), "quit error!", return NC_FALSE);
#		endif	// FLOWS_SINGLE
#		if (NC_FLOWS & NC_FLOWS_MULTIP)
		//NC_CHECK(nc_gfx_eng::get().work_quit(), "quit error!", return NC_FALSE);
		//if (nc_gfx_eng::get().get_work_flow().joinable()) { nc_gfx_eng::get().get_work_flow().join(); }
		//NC_CHECK(cmd_engine::get().work_quit(), "quit error!", return NC_FALSE);
		//if (cmd_engine::get().get_work_flow().joinable()) { cmd_engine::get().get_work_flow().join(); }
#		endif	// FLOWS_MULTIP

		NC_CHECK(iop_sys::get().quit(), "quit error!", return NC_FALSE);
		NC_CHECK(mem_sys::get().quit(), "quit error!", return NC_FALSE);

		return NC_TRUTH;
	}
	v1bit_t app_eng::work()
	{
		m_keybod.update();
		m_cursor.update();
		m_timer.update();
		m_window.update();

#		if (NC_FLOWS & NC_FLOWS_SINGLE)
		NC_CHECK(nc_gfx_eng::get().work(), "work error!", return NC_FALSE);
#		endif	// FLOWS_SINGLE
#		if (NC_FLOWS & NC_FLOWS_MULTIP)
#		endif	// FLOWS_MULTIP
		
		NC_CHECK(std_engine_t::work(), "work error!", return NC_FALSE);

		return NC_TRUTH;
	}
	v1bit_t app_eng::event_proc(nc_event_t& evt)
	{
		if (evt.has_sort(NC_EVSORT_CURSOR)) {
			cursor_t::nc_event_t& crs_evt = evt;
			m_cursor.event_proc(crs_evt);
		}
		else if (evt.has_sort(NC_EVSORT_KEYBOD)) {
			nc_keybod_t::nc_event_t& kbd_evt = evt;
			m_keybod.event_proc(kbd_evt);
			switch (evt.get_type()) {
			case NC_EVTYPE_KEYBOD_PRESS: {
				switch (kbd_evt.get_code()) {
				case NC_KEYCODE_ESCP: { work_quit(); return NC_TRUTH; break; }
				case NC_KEYCODE_F1: { m_window.set_opacity(0.1f); return NC_TRUTH; break; }
				case NC_KEYCODE_F2: { m_window.set_opacity(0.2f); return NC_TRUTH; break; }
				case NC_KEYCODE_F3: { m_window.set_opacity(0.3f); return NC_TRUTH; break; }
				case NC_KEYCODE_F4: { m_window.set_opacity(0.4f); return NC_TRUTH; break; }
				case NC_KEYCODE_F5: { m_window.set_opacity(0.5f); return NC_TRUTH; break; }
				case NC_KEYCODE_F6: { m_window.set_opacity(0.6f); return NC_TRUTH; break; }
				case NC_KEYCODE_F7: { m_window.set_opacity(0.7f); return NC_TRUTH; break; }
				case NC_KEYCODE_F8: { m_window.set_opacity(0.8f); return NC_TRUTH; break; }
				case NC_KEYCODE_F9: { m_window.set_opacity(0.9f); return NC_TRUTH; break; }
				case NC_KEYCODE_F10: { m_window.set_opacity(1.0f); return NC_TRUTH; break; }
				break;
				}
			}
			}
		}
		else if (evt.has_sort(NC_EVSORT_WINDOW)) {
			nc_event_window_t& wnd_evt = evt;
			switch (evt.get_type()) {
			case NC_EVTYPE_WINDOW_SIZED: {
				nc_gfx_eng::get().set_viewp({ 0, 0, NC_CAST_FLOAT(wnd_evt.get_val_x()), NC_CAST_FLOAT(wnd_evt.get_val_y()) });
				break;
			}
			case NC_EVTYPE_WINDOW_ACTIV: {
				if (wnd_evt.is_activ()) { if (is_cursor_enabled()) { set_cursor_enabled(NC_FALSE); } }
				else { if (is_cursor_enabled()) { set_cursor_enabled(NC_TRUTH); } }
				break;
			}
			case NC_EVTYPE_WINDOW_CLOSE: {
				if (wnd_evt.is_close()) {
					work_quit();
					evt.set_handled(NC_TRUTH);
					return NC_TRUTH;
					break;
				}
			}
		}
		}
		else if (evt.has_sort(NC_EVSORT_APPLIC)) {
			switch (evt.get_type()) {
			case NC_EVTYPE_APPLIC_DROPF: {
				break;
			}
			}
		}
		for (auto& istt : m_states) { if (evt.is_handled()) { return NC_FALSE; } istt->event_proc(evt); }
		return NC_TRUTH;
	}
	// --==</core_methods>==--
}
#endif	// NC_API