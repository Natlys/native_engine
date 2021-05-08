#include "nw_app_pch.hpp"
#include "nw_app_gui_of.h"
#if (defined NW_API)
#include "core/nw_app_engine.h"
#pragma warning(disable : 4312) // "reinterpret_cast' : conversion from "type" to "type" of greater size
namespace NW
{
	a_gui_of::a_gui_of() { }
	a_gui_of::~a_gui_of() { }
	// setters //
	v1nil_t a_gui_of::set_enabled(v1bit_t enable) {
		m_is_enabled = enable;
	}
	// commands //
	v1nil_t a_gui_of::draw_checkbox()
	{
		NW_GUI::Checkbox(get_name(), &m_is_enabled);
	}
}
// --==<gui_of_engines>==--
namespace NW
{
	// --==<gui_of_app_engine>==--
	gui_of_app_engine::gui_of_app_engine()
	{
	}
	gui_of_app_engine::~gui_of_app_engine() { }
	// setters //
	// commands //
	v1nil_t gui_of_app_engine::on_draw() {
		if (!m_is_enabled) { return; }
		NW_GUI::Begin("app_engine", &m_is_enabled);
		NW_GUI::Text("updates per second: %f;", app_engine::get().get_timer()->get_ups());
		
		NW_GUI::Columns(2, nullptr, NW_TRUE);
		if (NW_GUI::TreeNodeEx("entity_system", NW_GUI_TREE_FLAGS)) {
			for (auto& ietab : ent_sys::get().get_reg()) {
				if (NW_GUI::TreeNodeEx(&ietab, NW_GUI_TREE_FLAGS, "type_id: %d", ietab.first)) {
					for (auto& ient : ietab.second) {
						if (NW_GUI::TreeNodeEx(&ient, NW_GUI_TREE_FLAGS, "ent_id: %d", ient.first)) {
							NW_GUI::TreePop();	// ent_id
						}
					}
					NW_GUI::TreePop();	// type_id
				}
			}
			NW_GUI::TreePop();	// entities
		}
		NW_GUI::NextColumn();
		if (NW_GUI::TreeNodeEx("component_system", NW_GUI_TREE_FLAGS)) {
			for (auto& ictab : cmp_sys::get().get_reg()) {
				if (NW_GUI::TreeNodeEx(&ictab, NW_GUI_TREE_FLAGS, "type_id: %d", ictab.first)) {
					for (auto& icmp : ictab.second) {
						if (NW_GUI::TreeNodeEx(&icmp, NW_GUI_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
							NW_GUI::TreePop();	// cmp_id
						}
					}
					NW_GUI::TreePop();	// type_id
				}
			}
			NW_GUI::TreePop();	// components
		}
		NW_GUI::Separator();
		
		NW_GUI::End();
	}
	// --==</gui_ofapp_engine>==--
	// --==<gui_of_graphics_engine>==--
	gui_of_gfx_engine::gui_of_gfx_engine()
	{
	}
	gui_of_gfx_engine::~gui_of_gfx_engine() { }
	// setters //
	// commands //
	v1nil_t gui_of_gfx_engine::on_draw() {
		if (!m_is_enabled) { return; }
		NW_GUI::Begin("graphics_engine", &m_is_enabled);
		
		if (NW_GUI::TreeNodeEx("graphics_engine", NW_GUI_TREE_FLAGS)) {
			NW_GUI::TreePop();
		}
		NW_GUI::Separator();

		if constexpr (NW_TRUE) {
			auto& fmbuf = cmp_sys::get().get_ref<gfx_fmbuf_t>(NW_NULL);
			auto fmbuf_part = fmbuf->get_part();

			auto& size_xy_gui = NW_GUI::GetContentRegionAvail();
			
			v2u size_xy = { static_cast<v1u_t>(size_xy_gui.x), static_cast<v1u_t>(size_xy_gui.x) };
			if (fmbuf->get_size_x() != size_xy[0] || fmbuf->get_size_y() != size_xy[1]) {
				//gfx_engine::get().set_fmbuf_size(size_xy[0], size_xy[1]);
			}
			gfx_engine::get().set_viewp({ 0.0f, 0.0f, size_xy_gui[0], size_xy_gui[1] });
			NW_GUI::Image(
				reinterpret_cast<ImTextureID>(fmbuf_part->get_handle()),
				size_xy_gui, { 0.0f, 1.0f }, { 1.0f, 0.0f }
			);
		}

		NW_GUI::End();
	}
	// --==</gui_of_graphics_enigne>==--

	// --==<gui_ofcmd_engine>==--
	gui_of_cmd_engine::gui_of_cmd_engine() { }
	gui_of_cmd_engine::~gui_of_cmd_engine() { }
	// commands //
	v1nil_t gui_of_cmd_engine::on_draw() {
		if (!m_is_enabled) return;
		NW_GUI::Begin("console_engine");
		NW_GUI::End();
	}
	// --==</gui_ofcmd_engine>==--
}
// --==</gui_of_engines>==--
// --==<gui_of_systems>==--
namespace NW
{
	// --==<gui_of_io_sys>==--
	gui_of_io_sys::gui_of_io_sys()
	{
	}
	gui_of_io_sys::~gui_of_io_sys() { }
	// commands //
	v1nil_t gui_of_io_sys::on_draw() {
		if (!m_is_enabled) return;
		NW_GUI::Begin("io_system", &m_is_enabled, ImGuiWindowFlags_MenuBar);

		if (NW_GUI::BeginMenuBar()) {
			if (NW_GUI::BeginMenu("files")) {
				if (NW_GUI::MenuItem("new...", "ctrl+n")) {
				}
				if (NW_GUI::MenuItem("save_as...", "ctrl+s")) {
				}
				if (NW_GUI::MenuItem("load...", "ctrl+l")) {
				}
				NW_GUI::EndMenu();
			}
			NW_GUI::EndMenuBar();
		}
#if (NW_FALSE)
		if (NW_GUI::TreeNodeEx("io_system", NW_GUI_TREE_FLAGS)) {
			for (auto& icmps : iop_sys::get_registry()) {
				if (NW_GUI::TreeNodeEx(&icmps, NW_GUI_TREE_FLAGS, "type_id: %d", icmps.first)) {
					for (auto& icmp : icmps.second) {
						if (NW_GUI::TreeNodeEx(&icmp, NW_GUI_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
							NW_GUI::TreePop();
						}
					}
					NW_GUI::TreePop();
				}
			}
			NW_GUI::TreePop();
		}
#endif

		NW_GUI::End();
	}
	// --==</gui_ofio_sys>==--
}
// --==</gui_of_systems>==--
// --==<gui_of_editors>==--
namespace NW
{
	// --==<gui_of_material_editor>==--
	gui_of_material_editor::gui_of_material_editor() { }
	// setters //
	v1nil_t gui_of_material_editor::set_context(context& ref) {
		m_context.set_ref(ref);
		if (ref.is_valid()) {
			m_is_enabled = NW_TRUE;
		}
		else {
			m_is_enabled = false;
		}
	}
	// commands //
	v1nil_t gui_of_material_editor::on_draw()
	{
		NW_GUI::Begin("shd_editor", &m_is_enabled, ImGuiWindowFlags_MenuBar);
		if (!m_context.is_valid()) { NW_GUI::End(); return; }
		NW_GUI::End();
	}
	// --==</gui_of_material_editor>==--

	// --==<gui_of_sprite_editor>==--
	gui_of_sprite_editor::gui_of_sprite_editor() { }
	// setters //
	v1nil_t gui_of_sprite_editor::set_context(context& ref) {
		m_context.set_ref(ref);
		if (!ref.is_valid()) {
			m_is_enabled = false;
		}
		else {
			m_is_enabled = NW_TRUE;
		}
	}
	// commands //
	v1nil_t gui_of_sprite_editor::on_draw() {
		if (!m_is_enabled) return;
		NW_GUI::Begin("sprite_editor", &m_is_enabled, ImGuiWindowFlags_MenuBar);

		if (NW_GUI::BeginMenuBar()) {
			if (NW_GUI::BeginMenu("file")) {
				if (NW_GUI::MenuItem("save...")) {
				}
				else if (NW_GUI::MenuItem("load...")) {
				}
				NW_GUI::EndMenu();
			}
			NW_GUI::EndMenuBar();
		}
		if (!m_context.is_valid()) { NW_GUI::End(); return; }

		NW_GUI::Image(reinterpret_cast<ImTextureID>(m_context->get_handle()), { 64.0f, 64.0f });

		NW_GUI::End();
	}
	// --==</gui_ofSpriteEditor>==--
}
// --==</gui_of_editors>==--
#endif	// NW_API