#include "nwapp_pch.hpp"
#include "nwapp_engine.h"
#if (defined NW_API)
namespace NWAPP
{
	core_engine::core_engine(cstr name) :
		m_name(&name[0]),
		m_is_running(false), m_thr_run(thread()),
		m_states(states()),
		m_wnd(mem_ref<app_wnd_core>())
	{
	}
	core_engine::~core_engine()
	{
	}
	// --setters
	void core_engine::set_cursor_enabled(v1b enable) {
		m_wnd->set_cursor_enabled(enable);
	}
	void core_engine::del_state(v1u idx) {
		if (m_states.size() <= idx) { return; }
		m_states.erase(m_states.begin() + idx);
	}
	void core_engine::stop_running() {
		m_is_running = false;
	}
	// --==<core_methods>==--
	bool core_engine::init()
	{
		if (m_wnd.is_valid()) { return false; }
		if (!m_is_running) { m_is_running = true; }
		
		m_wnd.make_ref<app_wnd_core>(window_info{ &m_name[0], 1200, 800 });
		m_wnd->set_callback([this](a_event& evt)->void { return this->event_proc(evt); });

		for (auto& istate : m_states) { if (!istate->init()) { throw init_error(__FILE__, __LINE__); return false; } }
		
		return true;
	}
	void core_engine::quit()
	{
		if (!m_wnd.is_valid()) { return; }
		if (!m_is_running) { m_is_running = true; }

		for (auto& istate : m_states) { istate->quit(); }
		
		m_states.clear();
		m_wnd.reset();
	}
	void core_engine::run()
	{
		m_is_running = true;
		auto run_loop = [this]()->void {
			try {
				if (!init()) { throw init_error(__FILE__, __LINE__); return; }
				while (m_is_running) { update(); }
				quit();
			}
			catch (NWAPP::a_error& exc) {
				std::cout << exc;
				quit();
			}
			catch (std::exception& exc) {
				NW_ERR(exc.what());
				quit();
			}
		};
		m_thr_run = thread(run_loop);
	}
	void core_engine::update()
	{
		for (auto& istate : m_states) { istate->update(); }
		m_timer.update();
		m_wnd->update();
	}
	void core_engine::event_proc(a_event& evt)
	{
		for (auto& istt : m_states) {
			if (evt.is_handled) { return; }
			istt->event_proc(evt);
		}
		switch (evt.type) {
		case EVT_KBD_PRESSED: {
			kbd_event& kbd_evt = static_cast<kbd_event&>(evt);
			switch (kbd_evt.code) {
			case KBC_ESCAPE: { stop_running(); return; break; }
			case KBC_F1: { m_wnd->set_opacity(0.1f); return; break; }
			case KBC_F2: { m_wnd->set_opacity(0.2f); return; break; }
			case KBC_F3: { m_wnd->set_opacity(0.3f); return; break; }
			case KBC_F4: { m_wnd->set_opacity(0.4f); return; break; }
			case KBC_F5: { m_wnd->set_opacity(0.5f); return; break; }
			case KBC_F6: { m_wnd->set_opacity(0.6f); return; break; }
			case KBC_F7: { m_wnd->set_opacity(0.7f); return; break; }
			case KBC_F8: { m_wnd->set_opacity(0.8f); return; break; }
			case KBC_F9: { m_wnd->set_opacity(0.9f); return; break; }
			case KBC_F10: { m_wnd->set_opacity(1.0f); return; break; }
			}
			break;
		}
		case EVT_WND_RESIZE: {
			wnd_event& wnd_evt = static_cast<wnd_event&>(evt);
			get_graphics()->set_viewport(0, 0, wnd_evt.val_x, wnd_evt.val_y);
			get_graphics()->set_fbuf_size(wnd_evt.val_x, wnd_evt.val_y);
			break;
		}
		case EVT_WND_CLOSE: { stop_running(); return; break; }
		}
	}

	dstr core_engine::dialog_save(cstr filter) { return io_sys::get().save_dlg(filter, m_wnd->get_handle()); }
	dstr core_engine::dialog_load(cstr filter) { return io_sys::get().load_dlg(filter, m_wnd->get_handle()); }
	// --==</core_methods>==--
}
#endif	// NW_API