#include "nwapp_pch.hpp"
#include "nwapp_gui_of.h"
#pragma warning(disable:4312)
#include <core/nwapp_engine.h>
namespace NWAPP
{
	a_gui_of::a_gui_of() { }
	a_gui_of::~a_gui_of() { }
	// --setters
	void a_gui_of::set_enabled(v1b enable) {
		m_is_enabled = enable;
	}
	// --core_methods
	void a_gui_of::draw_checkbox()
	{
		NWGUI::Checkbox(get_name(), &m_is_enabled);
	}
}
// --==<gui_of_engines>==--
namespace NWAPP
{
	// --==<gui_of_core_engine>==--
	gui_of_core_engine::gui_of_core_engine(core_engine& core) :
		m_core(&core)
	{
	}
	gui_of_core_engine::~gui_of_core_engine() { }
	// --setters
	// --core_methods
	void gui_of_core_engine::on_draw() {
		if (!m_is_enabled) { return; }
		NWGUI::Begin("core_engine", &m_is_enabled);
		NWGUI::Text("updates per second: %f;", m_core->get_timer()->get_ups());
		
		NWGUI::Columns(2, nullptr, true);
		if (NWGUI::TreeNodeEx("entity_system", NWGUI_TREE_FLAGS)) {
			for (auto& ietab : ent_sys::get().get_ent_reg()) {
				if (NWGUI::TreeNodeEx(&ietab, NWGUI_TREE_FLAGS, "type_id: %d", ietab.first)) {
					for (auto& ient : ietab.second) {
						if (NWGUI::TreeNodeEx(&ient, NWGUI_TREE_FLAGS, "ent_id: %d", ient.first)) {
							NWGUI::TreePop();	// ent_id
						}
					}
					NWGUI::TreePop();	// type_id
				}
			}
			NWGUI::TreePop();	// entities
		}
		NWGUI::NextColumn();
		if (NWGUI::TreeNodeEx("component_system", NWGUI_TREE_FLAGS)) {
			for (auto& ictab : cmp_sys::get().get_cmp_reg()) {
				if (NWGUI::TreeNodeEx(&ictab, NWGUI_TREE_FLAGS, "type_id: %d", ictab.first)) {
					for (auto& icmp : ictab.second) {
						if (NWGUI::TreeNodeEx(&icmp, NWGUI_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
							NWGUI::TreePop();	// cmp_id
						}
					}
					NWGUI::TreePop();	// type_id
				}
			}
			NWGUI::TreePop();	// components
		}
		NWGUI::Separator();
		
		NWGUI::End();
	}
	// --==</gui_ofcore_engine>==--
	// --==<gui_of_graphics_engine>==--
	gui_of_gfx_engine::gui_of_gfx_engine(gfx_engine& graphics) :
		m_gfx(&graphics)
	{
	}
	gui_of_gfx_engine::~gui_of_gfx_engine() { }
	// --setters
	// --core_methods
	void gui_of_gfx_engine::on_draw() {
		if (!m_is_enabled) { return; }
		NWGUI::Begin("graphics_engine", &m_is_enabled);
		
		if (NWGUI::TreeNodeEx("graphics_engine", NWGUI_TREE_FLAGS)) {
			NWGUI::Columns(2, nullptr, true);
			if (NWGUI::TreeNodeEx("entities", NWGUI_TREE_FLAGS)) {
				for (auto& ients : m_gfx->get_ent_reg()) {
					if (NWGUI::TreeNodeEx(&ients, NWGUI_TREE_FLAGS, "type_id: %d", ients.first)) {
						for (auto& ient : ients.second) {
							if (NWGUI::TreeNodeEx(&ient, NWGUI_TREE_FLAGS, "ent_id: %d", ient.first)) {
								NWGUI::TreePop();	// ent_id
							}
						}
						NWGUI::TreePop();	// type_id
					}
				}
				NWGUI::TreePop();	// entities
			}
			NWGUI::NextColumn();
			if (NWGUI::TreeNodeEx("components", NWGUI_TREE_FLAGS)) {
				for (auto& icmps : m_gfx->get_cmp_reg()) {
					if (NWGUI::TreeNodeEx(&icmps, NWGUI_TREE_FLAGS, "type_id: %d", icmps.first)) {
						for (auto& icmp : icmps.second) {
							if (NWGUI::TreeNodeEx(&icmp, NWGUI_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
								NWGUI::TreePop();
							}
						}
						NWGUI::TreePop();	// type_id
					}
				}
				NWGUI::TreePop();	// components
			}
			NWGUI::TreePop();	// gfx_engine
		}
		NWGUI::Separator();

		NWGUI::End();
	}
	// --==</gui_of_graphics_enigne>==--

	// --==<gui_ofcmd_engine>==--
	gui_of_cmd_engine::gui_of_cmd_engine() { }
	gui_of_cmd_engine::~gui_of_cmd_engine() { }
	// --core_methods
	void gui_of_cmd_engine::on_draw() {
		if (!m_is_enabled) return;
		NWGUI::Begin("console_engine");
		NWGUI::End();
	}
	// --==</gui_ofcmd_engine>==--
}
// --==</gui_of_engines>==--
// --==<gui_of_systems>==--
namespace NWAPP
{
	// --==<gui_of_io_sys>==--
	gui_of_io_sys::gui_of_io_sys()
	{
	}
	gui_of_io_sys::~gui_of_io_sys() { }
	// --core_methods
	void gui_of_io_sys::on_draw() {
		if (!m_is_enabled) return;
		NWGUI::Begin("io_system", &m_is_enabled, ImGuiWindowFlags_MenuBar);

		if (NWGUI::BeginMenuBar()) {
			if (NWGUI::BeginMenu("files")) {
				if (NWGUI::MenuItem("new...", "ctrl+n")) {
				}
				if (NWGUI::MenuItem("save_as...", "ctrl+s")) {
				}
				if (NWGUI::MenuItem("load...", "ctrl+l")) {
				}
				NWGUI::EndMenu();
			}
			NWGUI::EndMenuBar();
		}
#if false
		if (NWGUI::TreeNodeEx("io_system", NWGUI_TREE_FLAGS)) {
			for (auto& icmps : io_sys::get_registry()) {
				if (NWGUI::TreeNodeEx(&icmps, NWGUI_TREE_FLAGS, "type_id: %d", icmps.first)) {
					for (auto& icmp : icmps.second) {
						if (NWGUI::TreeNodeEx(&icmp, NWGUI_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
							NWGUI::TreePop();
						}
					}
					NWGUI::TreePop();
				}
			}
			NWGUI::TreePop();
		}
#endif

		NWGUI::End();
	}
	// --==</gui_ofio_sys>==--
}
// --==</gui_of_systems>==--
// --==<gui_of_editors>==--
namespace NWAPP
{
	// --==<gui_of_material_editor>==--
	gui_of_material_editor::gui_of_material_editor() { }
	// --setters
	void gui_of_material_editor::set_context(context& ref) {
		m_context.set_ref(ref);
		if (ref.is_valid()) {
			m_is_enabled = true;
		}
		else {
			m_is_enabled = false;
		}
	}
	// --core_methods
	void gui_of_material_editor::on_draw()
	{
		NWGUI::Begin("shd_editor", &m_is_enabled, ImGuiWindowFlags_MenuBar);
		if (!m_context.is_valid()) { NWGUI::End(); return; }
		NWGUI::End();
	}
	// --==</gui_of_material_editor>==--

	// --==<gui_of_sprite_editor>==--
	gui_of_sprite_editor::gui_of_sprite_editor() { }
	// --setters
	void gui_of_sprite_editor::set_context(context& ref) {
		m_context.set_ref(ref);
		if (!ref.is_valid()) {
			m_is_enabled = false;
		}
		else {
			m_is_enabled = true;
		}
	}
	// --core_methods
	void gui_of_sprite_editor::on_draw() {
		if (!m_is_enabled) return;
		NWGUI::Begin("sprite_editor", &m_is_enabled, ImGuiWindowFlags_MenuBar);

		if (NWGUI::BeginMenuBar()) {
			if (NWGUI::BeginMenu("file")) {
				if (NWGUI::MenuItem("save...")) {
				}
				else if (NWGUI::MenuItem("load...")) {
				}
				NWGUI::EndMenu();
			}
			NWGUI::EndMenuBar();
		}
		if (!m_context.is_valid()) { NWGUI::End(); return; }

		NWGUI::Image(reinterpret_cast<ptr>(m_context->get_handle()), { 64.0f, 64.0f });

		NWGUI::End();
	}
	// --==</gui_ofSpriteEditor>==--
}
// --==</gui_of_editors>==--