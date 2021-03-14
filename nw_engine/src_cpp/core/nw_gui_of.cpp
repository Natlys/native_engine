#include <nw_pch.hpp>
#include "nw_gui_of.h"
#pragma warning(disable:4312)
namespace NW
{
	a_gui_of::a_gui_of() { }
	a_gui_of::~a_gui_of() { }
}
// --==<gui_of_engines>==--
namespace NW
{
	// --==<gui_of_core_engine>==--
	gui_of_core_engine::gui_of_core_engine() { }
	gui_of_core_engine::~gui_of_core_engine() { }

	// --core_methods
	void gui_of_core_engine::on_draw() {
		if (!m_is_enabled) return;
		GUI::Begin("core_engine", &m_is_enabled);
		GUI::End();
	}
	// --==</gui_ofcore_engine>==--

	// --==<gui_of_graphics_engine>==--
	gui_of_gfx_engine::gui_of_gfx_engine() { }
	gui_of_gfx_engine::~gui_of_gfx_engine() { }
	
	// --core_methods
	void gui_of_gfx_engine::on_draw() {
		if (!m_is_enabled) return;
		GUI::Begin("graphics_engine", &m_is_enabled);
		GUI::End();
	}
	// --==</gui_of_graphics_enigne>==--

	// --==<gui_ofcmd_engine>==--
	gui_of_cmd_engine::gui_of_cmd_engine() { }
	gui_of_cmd_engine::~gui_of_cmd_engine() { }
	// --core_methods
	void gui_of_cmd_engine::on_draw() {
		if (!m_is_enabled) return;
		GUI::Begin("console_engine");
		GUI::End();
	}
	// --==</gui_ofcmd_engine>==--
}
// --==</gui_of_engines>==--
// --==<gui_of_systems>==--
namespace NW
{
	// --==<gui_of_data_sys>==--
	gui_of_data_sys::gui_of_data_sys()
	{
	}
	gui_of_data_sys::~gui_of_data_sys() { }
	// --core_methods
	void gui_of_data_sys::on_draw() {
		if (!m_is_enabled) return;
		GUI::Begin("data_system", &m_is_enabled, ImGuiWindowFlags_MenuBar);

		if (GUI::BeginMenuBar()) {
			if (GUI::BeginMenu("files")) {
				if (GUI::MenuItem("new...", "ctrl+n")) {
				}
				if (GUI::MenuItem("save_as...", "ctrl+s")) {
				}
				if (GUI::MenuItem("load...", "ctrl+l")) {
				}
				GUI::EndMenu();
			}
			GUI::EndMenuBar();
		}

		if (GUI::Button("file system tree")) { system("tree"); }
		else if (GUI::Button("files list")) { system("dir"); }
		GUI::SeparatorEx(ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_SpanAllColumns);

		GUI::End();
	}
	// --==</gui_ofdata_sys>==--
}
// --==</gui_of_systems>==--
// --==<gui_of_editors>==--
namespace NW
{
	// --==<gui_of_material_editor>==--
	gui_of_material_editor::gui_of_material_editor() { }
	// --setters
	void gui_of_material_editor::set_context(mem_ref<gfx_material>& ref) {
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
		GUI::Begin("shader_editor", &m_is_enabled, ImGuiWindowFlags_MenuBar);
		if (!m_context.is_valid()) { GUI::End(); return; }
		GUI::End();
	}
	// --==</gui_of_material_editor>==--

	// --==<gui_of_sprite_editor>==--
	gui_of_sprite_editor::gui_of_sprite_editor() { }
	// --setters
	void gui_of_sprite_editor::set_context(mem_ref<a_texture>& ref) {
		m_context.set_ref(ref);
		if (!ref.is_valid()) {
			m_is_enabled = false;
		}
		else {
			m_is_enabled = true;
		}
		strcpy(m_context_name, m_context->get_name());
	}
	// --core_methods
	void gui_of_sprite_editor::on_draw() {
		if (!m_is_enabled) return;
		GUI::Begin("sprite_editor", &m_is_enabled, ImGuiWindowFlags_MenuBar);

		if (GUI::BeginMenuBar()) {
			if (GUI::BeginMenu("file")) {
				if (GUI::MenuItem("save...")) {
				}
				else if (GUI::MenuItem("load...")) {
				}
				GUI::EndMenu();
			}
			GUI::EndMenuBar();
		}
		if (!m_context.is_valid()) { GUI::End(); return; }

		GUI::Image(reinterpret_cast<imgui_texture_id>(m_context->get_ogl_id()), { 64.0f, 64.0f });

		GUI::End();
	}
	// --==</gui_ofSpriteEditor>==--
}
// --==</gui_of_editors>==--