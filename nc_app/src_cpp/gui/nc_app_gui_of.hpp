#ifndef NC_APP_GUI_OF_H
#define NC_APP_GUI_OF_H
#include "nc_app_core.hpp"
#if (defined NC_API)
namespace NC
{
	/// abstract gui_of struct
	class NC_API a_gui_of
	{
	public:
		a_gui_of();
		virtual ~a_gui_of();
		// getters //
		virtual inline cstr_t get_name() const = 0;
		// setters //
		v1nil_t set_enabled(v1bit_t enable);
		// predicates //
		inline v1bit_t is_enabled() const	{ return m_is_enabled; }
		// commands //
		virtual v1nil_t on_draw() = 0;
		v1nil_t draw_checkbox();
	protected:
		v1bit_t m_is_enabled = false;
	};
}
namespace NC
{
	/// gui_of_app_eng class
	/// description:
	/// --renders the core engine state and window gui
	class NC_API gui_of_app_eng : public a_gui_of
	{
	public:
		gui_of_app_eng();
		~gui_of_app_eng();
		// getters //
		virtual inline cstr_t get_name() const override { return "gui_of_app_eng"; }
		// commands //
		virtual v1nil_t on_draw() override;
	private:
	};
	/// gui_of_graphics_engine class
	class NC_API gui_of_nc_gfx_eng : public a_gui_of
	{
	public:
		gui_of_nc_gfx_eng();
		~gui_of_nc_gfx_eng();
		// getters //
		virtual inline cstr_t get_name() const override { return "gui_of_nc_gfx_eng"; }
		// commands //
		virtual v1nil_t on_draw() override;
	private:
	};
	/// gui_of_console_engine class
	class NC_API gui_of_cmd_engine : public a_gui_of
	{
	public:
		gui_of_cmd_engine();
		~gui_of_cmd_engine();
		// getters //
		virtual inline cstr_t get_name() const override { return "gui_of_cmd_engine"; }
		// commands //
		virtual v1nil_t on_draw() override;
	};
}
namespace NC
{
	/// gui_of_io_sys class
	class NC_API gui_of_io_sys : public a_gui_of
	{
	public:
		gui_of_io_sys();
		~gui_of_io_sys();
		// getters //
		virtual inline cstr_t get_name() const override { return "gui_of_io_sys"; }
		// commands //
		virtual v1nil_t on_draw() override;
	};
}
namespace NC
{
	/// gui_of_graphics_material class
	class NC_API gui_of_material_editor : public a_gui_of
	{
		using context = mem_ref_t<gfx_mtl_t>;
	public:
		gui_of_material_editor();
		// getters //
		virtual inline cstr_t get_name() const override { return "gui_of_material_editor"; }
		// setters //
		v1nil_t set_context(mem_ref_t<gfx_mtl_t>& ref);
		// commands //
		virtual v1nil_t on_draw() override;
	private:
		context m_context;
	};
	/// gui_of_sprite_editor class
	struct NC_API gui_of_sprite_editor : public a_gui_of
	{
		using context = mem_ref_t<gfx_txr_t>;
	public:
		gui_of_sprite_editor();
		// getters //
		virtual inline cstr_t get_name() const override { return "gui_of_sprite_editor"; }
		// setters //
		v1nil_t set_context(context& ref);
		// commands //
		virtual v1nil_t on_draw() override;
	private:
		context m_context;
		char_t m_context_name[128];
	};
}
#endif	// NC_API
#endif	// NC_APP_GUI_OF_H
