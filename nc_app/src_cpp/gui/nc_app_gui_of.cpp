#include "nc_app_pch.hpp"
#include "nc_app_gui_of.h"
#if (defined NC_API)
#include "core/nc_app_eng.h"
#pragma warning(disable : 4312) // "reinterpret_cast' : conversion from "type" to "type" of greater size
namespace NC
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
		NC_GUI::Checkbox(get_name(), &m_is_enabled);
	}
}
// --==<gui_of_engines>==--
namespace NC
{
	// --==<gui_of_app_eng>==--
	gui_of_app_eng::gui_of_app_eng()
	{
	}
	gui_of_app_eng::~gui_of_app_eng() { }
	// setters //
	// commands //
	v1nil_t gui_of_app_eng::on_draw() {
		if (!m_is_enabled) { return; }
		NC_GUI::Begin("app_eng", &m_is_enabled);
		NC_GUI::Text("updates per second: %f;", app_eng::get().get_timer()->get_ups());
		
		NC_GUI::Columns(2, nullptr, NC_TRUTH);
		if (NC_GUI::TreeNodeEx("entity_system", NC_GUI_TREE_FLAGS)) {
			for (auto& ietab : ent_sys::get().get_reg()) {
				if (NC_GUI::TreeNodeEx(&ietab, NC_GUI_TREE_FLAGS, "type_id: %d", ietab.first)) {
					for (auto& ient : ietab.second) {
						if (NC_GUI::TreeNodeEx(&ient, NC_GUI_TREE_FLAGS, "ent_id: %d", ient.first)) {
							NC_GUI::TreePop();	// ent_id
						}
					}
					NC_GUI::TreePop();	// type_id
				}
			}
			NC_GUI::TreePop();	// entities
		}
		NC_GUI::NextColumn();
		if (NC_GUI::TreeNodeEx("component_system", NC_GUI_TREE_FLAGS)) {
			for (auto& ictab : cmp_sys::get().get_reg()) {
				if (NC_GUI::TreeNodeEx(&ictab, NC_GUI_TREE_FLAGS, "type_id: %d", ictab.first)) {
					for (auto& icmp : ictab.second) {
						if (NC_GUI::TreeNodeEx(&icmp, NC_GUI_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
							NC_GUI::TreePop();	// cmp_id
						}
					}
					NC_GUI::TreePop();	// type_id
				}
			}
			NC_GUI::TreePop();	// components
		}
		NC_GUI::Separator();
		
		NC_GUI::End();
	}
	// --==</gui_ofapp_eng>==--
	// --==<gui_of_graphics_engine>==--
	gui_of_nc_gfx_eng::gui_of_nc_gfx_eng()
	{
	}
	gui_of_nc_gfx_eng::~gui_of_nc_gfx_eng() { }
	// setters //
	// commands //
	v1nil_t gui_of_nc_gfx_eng::on_draw() {
		if (!m_is_enabled) { return; }
		NC_GUI::Begin("graphics_engine", &m_is_enabled);
		
		if (NC_GUI::TreeNodeEx("graphics_engine", NC_GUI_TREE_FLAGS)) {
			NC_GUI::TreePop();
		}
		NC_GUI::Separator();

		if constexpr (NC_TRUTH) {
			auto& fmbuf = cmp_sys::get().get_ref<gfx_fmbuf_t>(NC_NULL);
			auto fmbuf_part = fmbuf->get_part();

			auto& size_xy_gui = NC_GUI::GetContentRegionAvail();
			
			v2u size_xy = { static_cast<v1u_t>(size_xy_gui.x), static_cast<v1u_t>(size_xy_gui.x) };
			if (fmbuf->get_size_x() != size_xy[0] || fmbuf->get_size_y() != size_xy[1]) {
				//nc_gfx_eng::get().set_fmbuf_size(size_xy[0], size_xy[1]);
			}
			nc_gfx_eng::get().set_viewp({ 0.0f, 0.0f, size_xy_gui[0], size_xy_gui[1] });
			NC_GUI::Image(
				reinterpret_cast<ImTextureID>(fmbuf_part->get_handle()),
				size_xy_gui, { 0.0f, 1.0f }, { 1.0f, 0.0f }
			);
		}

		NC_GUI::End();
	}
	// --==</gui_of_graphics_enigne>==--

	// --==<gui_ofcmd_engine>==--
	gui_of_cmd_engine::gui_of_cmd_engine() { }
	gui_of_cmd_engine::~gui_of_cmd_engine() { }
	// commands //
	v1nil_t gui_of_cmd_engine::on_draw() {
		if (!m_is_enabled) return;
		NC_GUI::Begin("console_engine");
		NC_GUI::End();
	}
	// --==</gui_ofcmd_engine>==--
}
// --==</gui_of_engines>==--
// --==<gui_of_systems>==--
namespace NC
{
	// --==<gui_of_io_sys>==--
	gui_of_io_sys::gui_of_io_sys()
	{
	}
	gui_of_io_sys::~gui_of_io_sys() { }
	// commands //
	v1nil_t gui_of_io_sys::on_draw() {
		if (!m_is_enabled) return;
		NC_GUI::Begin("io_system", &m_is_enabled, ImGuiWindowFlags_MenuBar);

		if (NC_GUI::BeginMenuBar()) {
			if (NC_GUI::BeginMenu("files")) {
				if (NC_GUI::MenuItem("new...", "ctrl+n")) {
				}
				if (NC_GUI::MenuItem("save_as...", "ctrl+s")) {
				}
				if (NC_GUI::MenuItem("load...", "ctrl+l")) {
				}
				NC_GUI::EndMenu();
			}
			NC_GUI::EndMenuBar();
		}
#if (NC_FALSE)
		if (NC_GUI::TreeNodeEx("io_system", NC_GUI_TREE_FLAGS)) {
			for (auto& icmps : iop_sys::get_registry()) {
				if (NC_GUI::TreeNodeEx(&icmps, NC_GUI_TREE_FLAGS, "type_id: %d", icmps.first)) {
					for (auto& icmp : icmps.second) {
						if (NC_GUI::TreeNodeEx(&icmp, NC_GUI_TREE_FLAGS, "cmp_id: %d", icmp.first)) {
							NC_GUI::TreePop();
						}
					}
					NC_GUI::TreePop();
				}
			}
			NC_GUI::TreePop();
		}
#endif

		NC_GUI::End();
	}
	// --==</gui_ofio_sys>==--
}
// --==</gui_of_systems>==--
// --==<gui_of_editors>==--
namespace NC
{
	// --==<gui_of_material_editor>==--
	gui_of_material_editor::gui_of_material_editor() { }
	// setters //
	v1nil_t gui_of_material_editor::set_context(context& ref) {
		m_context.set_ref(ref);
		if (ref.is_valid()) {
			m_is_enabled = NC_TRUTH;
		}
		else {
			m_is_enabled = false;
		}
	}
	// commands //
	v1nil_t gui_of_material_editor::on_draw()
	{
		NC_GUI::Begin("shd_editor", &m_is_enabled, ImGuiWindowFlags_MenuBar);
		if (!m_context.is_valid()) { NC_GUI::End(); return; }
		NC_GUI::End();
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
			m_is_enabled = NC_TRUTH;
		}
	}
	// commands //
	v1nil_t gui_of_sprite_editor::on_draw() {
		if (!m_is_enabled) return;
		NC_GUI::Begin("sprite_editor", &m_is_enabled, ImGuiWindowFlags_MenuBar);

		if (NC_GUI::BeginMenuBar()) {
			if (NC_GUI::BeginMenu("file")) {
				if (NC_GUI::MenuItem("save...")) {
				}
				else if (NC_GUI::MenuItem("load...")) {
				}
				NC_GUI::EndMenu();
			}
			NC_GUI::EndMenuBar();
		}
		if (!m_context.is_valid()) { NC_GUI::End(); return; }

		NC_GUI::Image(reinterpret_cast<ImTextureID>(m_context->get_handle()), { 64.0f, 64.0f });

		NC_GUI::End();
	}
	// --==</gui_ofSpriteEditor>==--
}
// --==</gui_of_editors>==--
#endif	// NC_API