#include <nw_pch.hpp>
#include <core/nw_engine_states.h>

#include <core/nw_engine.h>
#include <core/nw_gui_of.h>

namespace NW
{
	a_core_state::a_core_state(core_engine& engine) :
		m_core(&engine) { }
	a_core_state::~a_core_state() { }
}
namespace NW
{
	game_core_state::game_core_state(core_engine& engine) :
		a_core_state(engine) { }
	game_core_state::~game_core_state() { }

	// --==<core_methods>==--
	bool game_core_state::init() { return true; }
	void game_core_state::quit() { }
	void game_core_state::update() { }

	void game_core_state::event_proc(mouse_event& evt) { }
	void game_core_state::event_proc(keyboard_event& evt) { }
	void game_core_state::event_proc(window_event& evt) { }
	// --==</core_methods>==--
}
namespace NW
{
	vtx_2f2f vtx_quad_2f2f[] = {
		{ { -1.0f,	-1.0f },	{ 0.00f,	0.00f } },
		{ { -1.0f,	+1.0f },	{ 0.00f,	1.00f } },
		{ { +1.0f,	+1.0f },	{ 1.00f,	1.00f } },
		{ { +1.0f,	+1.0f },	{ 1.00f,	1.00f } },
		{ { +1.0f,	-1.0f },	{ 1.00f,	0.00f } },
		{ { -1.0f,	-1.0f },	{ 0.00f,	0.00f } },
	};
	vtx_3f vtx_cube_3f[] = {
		{ { -1.0f,  1.0f, -1.0f } },
		{ { -1.0f, -1.0f, -1.0f } },
		{ {  1.0f, -1.0f, -1.0f } },
		{ {  1.0f, -1.0f, -1.0f } },
		{ {  1.0f,  1.0f, -1.0f } },
		{ { -1.0f,  1.0f, -1.0f } },

		{ { -1.0f, -1.0f,  1.0f } },
		{ { -1.0f, -1.0f, -1.0f } },
		{ { -1.0f,  1.0f, -1.0f } },
		{ { -1.0f,  1.0f, -1.0f } },
		{ { -1.0f,  1.0f,  1.0f } },
		{ { -1.0f, -1.0f,  1.0f } },

		{  { 1.0f, -1.0f, -1.0f } },
		{  { 1.0f, -1.0f,  1.0f } },
		{  { 1.0f,  1.0f,  1.0f } },
		{  { 1.0f,  1.0f,  1.0f } },
		{  { 1.0f,  1.0f, -1.0f } },
		{  { 1.0f, -1.0f, -1.0f } },

		{ { -1.0f, -1.0f,  1.0f } },
		{ { -1.0f,  1.0f,  1.0f } },
		{ {  1.0f,  1.0f,  1.0f } },
		{ {  1.0f,  1.0f,  1.0f } },
		{ {  1.0f, -1.0f,  1.0f } },
		{ { -1.0f, -1.0f,  1.0f } },

		{ { -1.0f,  1.0f, -1.0f } },
		{ {  1.0f,  1.0f, -1.0f } },
		{ {  1.0f,  1.0f,  1.0f } },
		{ {  1.0f,  1.0f,  1.0f } },
		{ { -1.0f,  1.0f,  1.0f } },
		{ { -1.0f,  1.0f, -1.0f } },

		{ { -1.0f, -1.0f, -1.0f } },
		{ { -1.0f, -1.0f,  1.0f } },
		{ {  1.0f, -1.0f, -1.0f } },
		{ {  1.0f, -1.0f, -1.0f } },
		{ { -1.0f, -1.0f,  1.0f } },
		{ {  1.0f, -1.0f,  1.0f } },
	};
	vtx_3f2f3f vtx_cube_3f2f3f[] = {
		{ { -0.5f,	-0.5f,	-0.5f },	{ 0.0f, 0.0f }, { +0.0f,	+0.0f,	-1.0f} },
		{ { +0.5f,	-0.5f,	-0.5f },	{ 1.0f, 0.0f }, { +0.0f,	+0.0f,	-1.0f} },
		{ { +0.5f,	+0.5f,	-0.5f },	{ 1.0f, 1.0f }, { +0.0f,	+0.0f,	-1.0f} },
		{ { +0.5f,	+0.5f,	-0.5f },	{ 1.0f, 1.0f }, { +0.0f,	+0.0f,	-1.0f} },
		{ { -0.5f,	+0.5f,	-0.5f },	{ 0.0f, 1.0f },	{ +0.0f,	+0.0f,	-1.0f} },
		{ { -0.5f,	-0.5f,	-0.5f },	{ 0.0f, 0.0f }, { +0.0f,	+0.0f,	-1.0f} },

		{ { -0.5f,	-0.5f,	+0.5f },	{ 0.0f,	0.0f },	{ +0.0f,	+0.0f,	+1.0f } },
		{ { +0.5f,	-0.5f,	+0.5f },	{ 1.0f,	0.0f },	{ +0.0f,	+0.0f,	+1.0f } },
		{ { +0.5f,	+0.5f,	+0.5f },	{ 1.0f,	1.0f },	{ +0.0f,	+0.0f,	+1.0f } },
		{ { +0.5f,	+0.5f,	+0.5f },	{ 1.0f,	1.0f },	{ +0.0f,	+0.0f,	+1.0f } },
		{ { -0.5f,	+0.5f,	+0.5f },	{ 0.0f,	1.0f },	{ +0.0f,	+0.0f,	+1.0f } },
		{ { -0.5f,	-0.5f,	+0.5f },	{ 0.0f,	0.0f },	{ +0.0f,	+0.0f,	+1.0f } },
	
		{ { -0.5f,	+0.5f,	+0.5f },	{ 1.0f,	0.0f }, { -1.0f,	+0.0f,	+0.0f } },
		{ { -0.5f,	+0.5f,	-0.5f },	{ 1.0f,	1.0f }, { -1.0f,	+0.0f,	+0.0f } },
		{ { -0.5f,	-0.5f,	-0.5f },	{ 0.0f,	1.0f }, { -1.0f,	+0.0f,	+0.0f } },
		{ { -0.5f,	-0.5f,	-0.5f },	{ 0.0f,	1.0f }, { -1.0f,	+0.0f,	+0.0f } },
		{ { -0.5f,	-0.5f,	+0.5f },	{ 0.0f,	0.0f }, { -1.0f,	+0.0f,	+0.0f } },
		{ { -0.5f,	+0.5f,	+0.5f },	{ 1.0f,	0.0f }, { -1.0f,	+0.0f,	+0.0f } },
		
		{ { +0.5f,	+0.5f,	+0.5f },	{ 1.0f,	0.0f }, { +1.0f,	+0.0f,	+0.0f } },
		{ { +0.5f,	+0.5f,	-0.5f },	{ 1.0f,	1.0f }, { +1.0f,	+0.0f,	+0.0f } },
		{ { +0.5f,	-0.5f,	-0.5f },	{ 0.0f,	1.0f }, { +1.0f,	+0.0f,	+0.0f } },
		{ { +0.5f,	-0.5f,	-0.5f },	{ 0.0f,	1.0f }, { +1.0f,	+0.0f,	+0.0f } },
		{ { +0.5f,	-0.5f,	+0.5f },	{ 0.0f,	0.0f }, { +1.0f,	+0.0f,	+0.0f } },
		{ { +0.5f,	+0.5f,	+0.5f },	{ 1.0f,	0.0f }, { +1.0f,	+0.0f,	+0.0f } },
									
		{ { -0.5f, -0.5f,	-0.5f },	{ 0.0f,	1.0f }, { +0.0f,	-1.0f,	+0.0f } },
		{ { +0.5f, -0.5f,	-0.5f },	{ 1.0f,	1.0f },	{ +0.0f,	-1.0f,	+0.0f } },
		{ { +0.5f, -0.5f,	+0.5f },	{ 1.0f,	0.0f }, { +0.0f,	-1.0f,	+0.0f } },
		{ { +0.5f, -0.5f,	+0.5f },	{ 1.0f,	0.0f }, { +0.0f,	-1.0f,	+0.0f } },
		{ { -0.5f, -0.5f,	+0.5f },	{ 0.0f,	0.0f }, { +0.0f,	-1.0f,	+0.0f } },
		{ { -0.5f, -0.5f,	-0.5f },	{ 0.0f,	1.0f }, { +0.0f,	-1.0f,	+0.0f } },
											
		{ { -0.5f,	+0.5f,	-0.5f },	{ 0.0f,	1.0f }, { +0.0f,	+1.0f,	+0.0f } },
		{ { +0.5f,	+0.5f,	-0.5f },	{ 1.0f,	1.0f }, { +0.0f,	+1.0f,	+0.0f } },
		{ { +0.5f,	+0.5f,	+0.5f },	{ 1.0f,	0.0f }, { +0.0f,	+1.0f,	+0.0f } },
		{ { +0.5f,	+0.5f,	+0.5f },	{ 1.0f,	0.0f }, { +0.0f,	+1.0f,	+0.0f } },
		{ { -0.5f,	+0.5f,	+0.5f },	{ 0.0f,	0.0f }, { +0.0f,	+1.0f,	+0.0f } },
		{ { -0.5f,	+0.5f,	-0.5f },	{ 0.0f,	1.0f }, { +0.0f,	+1.0f,	+0.0f } }
	};
}
namespace NW
{
	gfx_core_state::gfx_core_state(core_engine& rEngine) :
		a_core_state(rEngine),
		m_gfx(m_core->get_graphics()),
		m_camera_lad() { }
	gfx_core_state::~gfx_core_state() { }
	// --==<core_methods>==--
	bool gfx_core_state::init() {
		m_gfx = m_core->get_graphics();
		
		mem_ref<buf_shd> sbuf;
		m_gfx->new_rsc<buf_shd>(sbuf);
		sbuf->add_elem(shd_elem("unf_model", DT_MAT4_FLOAT32, 0, 1), 1);
		sbuf->add_elem(shd_elem("unf_view", DT_MAT4_FLOAT32, 1, 1), 1);
		sbuf->add_elem(shd_elem("unf_proj", DT_MAT4_FLOAT32, 2, 1), 1);
		if (!sbuf->remake()) { return false; }
		
		mem_ref<state_sampler> smp;
		m_gfx->new_rsc<state_sampler>(smp);

		if (!init_skybox()) { return false; }
		if (!init_scene()) { return false; }

		return true;
	}
	void gfx_core_state::quit()
	{
	}
	void gfx_core_state::update()
	{
		{
			m_camera_lad.update(*m_core->get_keyboard(), *m_core->get_mouse(), *m_core->get_timer());
			
			f32 time_curr = m_core->get_timer()->get_curr();
			buf_16f16f16f unf_tform = {
				m4f(1.0f),
				m_camera_lad.get_view_mat(),
				m_camera_lad.get_proj_mat(),
			};
			
			m_gfx->get_rsc<buf_shd>(0).get_ref<buf_shd>()->set_data(sizeof(buf_16f16f16f), &unf_tform);
		}
		
		draw_skybox();
		draw_scene();
	}

	void gfx_core_state::event_proc(mouse_event& evt)
	{
	}
	void gfx_core_state::event_proc(keyboard_event& evt)
	{
		switch (evt.type) {
		case EVT_KEYBOARD_RELEASED:
			switch (evt.code) {
			case KBC_1: m_gfx->set_primitive(GPT_TRIANGLES); break;
			//case KBC_2: m_gfx->set_primitive(GPT_TRIANGLE_STRIP); break;
			//case KBC_3: m_gfx->set_primitive(GPT_TRIANGLE_FAN); break;
			//case KBC_4: m_gfx->set_primitive(GPT_LINE_STRIP); break;
			//case KBC_5: m_gfx->set_primitive(GPT_LINES); break;
			//case KBC_6: m_gfx->set_primitive(GPT_LINE_LOOP); break;
			//case KBC_7: m_gfx->set_primitive(GPT_POINTS); break;
			}
			break;
		default: break;
		}
	}
	void gfx_core_state::event_proc(window_event& evt) {
		switch (evt.type) {
		case EVT_WINDOW_RESIZE: {
			v4si viewport = m_gfx->get_configs().viewport;
			viewport[2] = evt.val_x;
			viewport[3] = evt.val_y;
			m_gfx->set_viewport(viewport[0], viewport[1], viewport[2], viewport[3]);
			m_camera_lad.aspect_ratio = static_cast<f32>(viewport[2] - viewport[0]) / static_cast<f32>(viewport[3] - viewport[1]);
			break;
		}
		case EVT_WINDOW_MOVE: { break; }
		default: break;
		}
	}
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline bool gfx_core_state::init_skybox() {
		mem_ref<drawable_vtx> drb;
		m_gfx->new_rsc<drawable_vtx>(drb);
		
		mem_ref<buf_vtx> vbuf;
		m_gfx->new_rsc<buf_vtx>(vbuf);
		if (!vbuf->remake<vtx_3f>(std::size(vtx_cube_3f), &vtx_cube_3f[0])) { return false; }
		drb->get_bufs().push_back(vbuf);

		dstring str_buf;

		mem_ref<a_shader> vshd;
		m_gfx->new_rsc<a_shader, shader_vtx>(vshd, "shd_skybox_vtx");
		if (!data_sys::load_file(R"(nw_gfx\src_glsl\skybox_3d_vtx.glsl)", str_buf)) { return false; }
		if (!vshd->remake(&str_buf[0])) { return false; }
		mem_ref<buf_shd> sbuf = m_gfx->get_rsc<buf_shd>(0);
		vshd->add_buf(sbuf);

		mem_ref<a_shader> pshd;
		m_gfx->new_rsc<a_shader, shader_pxl>(pshd, "shd_skybox_pxl");
		if (!data_sys::load_file(R"(nw_gfx\src_glsl\skybox_3d_pxl.glsl)", str_buf)) { return false; }
		if (!pshd->remake(&str_buf[0])) { return false; }

		mem_ref<a_texture> txr;
		m_gfx->new_rsc<a_texture, texture_cube>(txr, "txr_sb_0");
		image_bmp img(R"(img_sb_0)");
		if (!data_sys::load_file(R"(data\image\nw_sb.bmp)", img)) { return false; }
		if (!txr->remake(img)) { return false; }

		m_gfx->new_rsc<gfx_material>(drb->get_gmt(), "gmt");
		drb->get_gmt()->add_shader(vshd);
		drb->get_gmt()->add_shader(pshd);
		drb->get_gmt()->add_texture(txr);
		if (!drb->get_gmt()->remake()) { return false; }

		mem_ref<input_layout> layout;
		m_gfx->new_rsc<input_layout>(layout);
		layout->add_elem(shd_elem("atb_vtx_crd", DT_VEC3_FLOAT32, 0, 1), 1);
		if (!layout->remake(vshd)) { return false; }
		drb->add_rsc(layout);

		mem_ref<state_depth> dpt;
		m_gfx->new_rsc<state_depth>(dpt);
		dpt->set_enabled(false);
		drb->add_rsc(dpt);

		mem_ref<state_sampler> smp;
		m_gfx->new_rsc<state_sampler>(smp);
		smp->set_filter(TXFL_LINEAR);
		drb->add_rsc(smp);

		return true;
	}
	inline bool gfx_core_state::init_scene() {
		mem_ref<drawable_vtx> drb;
		m_gfx->new_rsc<drawable_vtx>(drb);

		mem_ref<buf_vtx> vbuf;
		m_gfx->new_rsc<buf_vtx>(vbuf);
		if (!vbuf->remake<vtx_3f2f3f>(std::size(vtx_cube_3f2f3f), &vtx_cube_3f2f3f[0])) { return false; }
		drb->get_bufs().push_back(vbuf);

		dstring str_buf = "default";

		mem_ref<a_shader> vshd;
		m_gfx->new_rsc<a_shader, shader_vtx>(vshd, "shd_default_vtx");
		if (!data_sys::load_file(R"(nw_gfx\src_glsl\default_3d_vtx.glsl)", str_buf)) { return false; }
		if (!vshd->remake(&str_buf[0])) { return false; }
		mem_ref<buf_shd> sbuf = m_gfx->get_rsc<buf_shd>(0);
		vshd->add_buf(sbuf);

		mem_ref<a_shader> pshd;
		m_gfx->new_rsc<a_shader, shader_pxl>(pshd, "shd_default_pxl");
		if (!data_sys::load_file(R"(nw_gfx\src_glsl\default_3d_pxl.glsl)", str_buf)) { return false; }
		if (!pshd->remake(&str_buf[0])) { return false; }
		
		mem_ref<a_texture> txr;
		m_gfx->new_rsc<a_texture, texture_2d>(txr, "txr_0");
		image_bmp img("nw_logo.bmp");
		if (!data_sys::load_file(R"(data\image\txr_wood_0.bmp)", img)) { return false; }
		if (!txr->remake(img)) { return false; }
		
		m_gfx->new_rsc<gfx_material>(drb->get_gmt(), "gmt_default");
		drb->get_gmt()->add_shader(vshd);
		drb->get_gmt()->add_shader(pshd);
		drb->get_gmt()->add_texture(txr);
		if (!drb->get_gmt()->remake()) { return false; }

		mem_ref<input_layout> layout;
		m_gfx->new_rsc<input_layout>(layout);
		layout->add_elem(shd_elem("atb_vtx_crd", DT_VEC3_FLOAT32, 0, 1), 1);
		layout->add_elem(shd_elem("atb_txr_crd", DT_VEC2_FLOAT32, 1, 1), 1);
		layout->add_elem(shd_elem("atb_nrm_crd", DT_VEC3_FLOAT32, 2, 1), 1);
		if (!layout->remake(vshd)) { return false; }
		drb->add_rsc(layout);

		mem_ref<state_depth> dpt;
		m_gfx->new_rsc<state_depth>(dpt);
		dpt->set_enabled(true);
		drb->add_rsc(dpt);

		mem_ref<state_sampler> smp;
		m_gfx->new_rsc<state_sampler>(smp);
		smp->set_filter(TXFL_NEAREST);
		drb->add_rsc(smp);

		return true;
	}
	inline void gfx_core_state::draw_skybox() {
		auto& drb = m_gfx->get_rsc<drawable>(0);
		drb->on_draw();
	}
	inline void gfx_core_state::draw_scene() {
		auto& drb = m_gfx->get_rsc<drawable>(1);
		drb->on_draw();
	}
	// --==</implementation_methods>==--
}
namespace NW
{
	gui_core_state::gui_core_state(core_engine& rEngine) :
		a_core_state(rEngine),
		m_enable_dockspace(true), m_full_screen_persist(true),
		m_gui_context(nullptr), m_gui_io(nullptr), m_gui_style(nullptr) { }
	gui_core_state::~gui_core_state() { }
	// --==<core_methods>==--
	bit gui_core_state::init()
	{
		if (!NW::imgui_init(m_core->get_window()->get_native(), m_core->get_graphics()->get_device(), m_core->get_graphics()->get_context())) { return false; }
		
		m_gui_of.resize(4);
		m_gui_of[0].make_ref<gui_of_core_engine>();
		m_gui_of[1].make_ref<gui_of_gfx_engine>();
		m_gui_of[2].make_ref<gui_of_cmd_engine>();
		m_gui_of[3].make_ref<gui_of_data_sys>();
		
		m_gui_of[0]->m_is_enabled = true;
		m_gui_of[1]->m_is_enabled = true;
		m_gui_of[3]->m_is_enabled = true;

		return true;
	}
	void gui_core_state::quit()
	{
		NW::imgui_quit();
	}
	void gui_core_state::update() {
		imgui_begin_frame();
		GUI::Begin("window");
		GUI::End();
		imgui_end_frame();
#if false
		imgui_begin_frame();

		if (GUI::BeginMenuBar()) {
			if (GUI::BeginMenu("view")) {
				for (auto& igui_of : m_gui_of) {
					GUI::Checkbox(igui_of->get_name(), &igui_of->m_is_enabled);
				}
				GUI::EndMenu();
			}
			GUI::EndMenuBar();
		}
		for (auto& igui_of : m_gui_of) {
			igui_of->on_draw();
		}

		imgui_end_frame();
#endif
	}

	void gui_core_state::event_proc(mouse_event& rmEvt) { }
	void gui_core_state::event_proc(keyboard_event& rkEvt) { }
	void gui_core_state::event_proc(window_event& rwEvt) { }
	// --==</core_methods>==--
}