#include "nc_app_pch.hpp"
#include "nc_app_states.h"
#if (defined NC_API)
#	include "nc_app_eng.h"
#	include "nc_app_gui_of.h"
#	include "core/nc_gfx_data.h"
#	include "num/nc_num_rect.h"
#	include "num/nc_num_circ.h"
#	include "num/nc_num_voron.h"
namespace NC
{
	inline gfx_img_t make_pxl_voron(cv2u size_xy, v1u_tc scount) {
		gfx_img_t image;
		image.remake(mem_layt_tt<v4u08>("pixel"), size_xy);
		array_t<v2f_t> sites(scount, NC_NULL);
		for (auto& isite : sites) { isite = v2f_t::make_rand(0.0f, 1.0f); }
		for (v1u_t ipxl = 0u; ipxl < image.get_size(); ipxl++) {
			v2f_t pxl_crd = v2f_t{
				NC_CAST_FLOAT(ipxl % image.get_size_x()),
				NC_CAST_FLOAT(ipxl / image.get_size_x())
			};
			pxl_crd[0] = (pxl_crd[0] == NC_NULL) ? pxl_crd[0] : pxl_crd[0] / NC_CAST_FLOAT(image.get_size_x());
			pxl_crd[1] = (pxl_crd[1] == NC_NULL) ? pxl_crd[1] : pxl_crd[1] / NC_CAST_FLOAT(image.get_size_y());
			v1u08_t min_dst = static_cast<v1u08_t>(pxl_crd.get_near_idx(sites) * 255.0f / NC_CAST_FLOAT(scount));
			//v1u08_t min_dst = static_cast<v1u08_t>(pxl_crd.get_near_idx(sites) * 255.0f / NC_CAST_FLOAT(scount));
			image[ipxl] = v4u08{ min_dst, min_dst, min_dst, 255u };
		}
		return image;
	}
}
namespace NC
{
	// ctor_dtor //
	app_state_game_t::app_state_game_t() : app_state_t(type_info_t::get_name<app_state_game_t>()) { }
	app_state_game_t::~app_state_game_t() { }
	// commands //
	v1bit_t app_state_game_t::init()
	{
		// component creation
		if constexpr (NC_TRUTH) {
			// transformations
			if constexpr (NC_TRUTH) {
				// cam_tform
				if constexpr (NC_TRUTH) {
					auto& camera = cmp_sys::get().new_ref<gfx_tform_cam>();
					camera->set_slot(0u);
					camera->remake();
				}
			}
			// buffers;
			if constexpr (NC_TRUTH)
			{
				// 3default
				if constexpr (NC_TRUTH) {
					constexpr const size_tc dcount = 4u;
					// d_rect
					if constexpr (NC_TRUTH)
					{
						auto& layt = cmp_sys::get().new_ref<gfx_layt_t>().get_val();
						layt.get_layt().add_node<v3f_t>("vsi_vtx_crd").add_node<v2f_t>("vsi_txr_crd").add_node<v3f_t>("vsi_nrm_crd");
						auto& vbuf = cmp_sys::get().new_ref<gfx_buf_vtx_t>(layt, NC_NUM_POW(2u, dcount)).get_val();
						auto& ibuf = cmp_sys::get().new_ref<gfx_buf_idx_t>(mem_layt_tt<v1u_t>("index"), vbuf.get_count() * vbuf.get_count()).get_val();
						using rect_t = num_rect_t<dcount>;
						rect_t rect(rect_t::vec_t(0.0f), rect_t::vec_t(1.0f));
						for (v1u_t ivert(0u); ivert < rect.get_vcount(); ivert++) {
							v4f_t icrd = rect.make_vert(rect, ivert);
							v3f_t vtx_crd = icrd;
							v2f_t txr_crd = ((icrd - rect.m_center) / rect.m_halfsz + 1.0f) / 2.0f; // convert from vertex to texture coordinate;
							v3f_t nrm_crd = (icrd - rect.m_center) / rect.m_halfsz;
							if (NC_TRUTH) {
								num_mat_t<v1f_t, 5u, 5u> proj(NC_NULL);
								num_mat_t<v1f_t, 2u, 4u> bounds(NC_NULL);
								bounds[0] = v4f_t{ -1.0f, +1.0f };
								bounds[1] = v4f_t{ -1.0f, +1.0f };
								bounds[2] = v4f_t{ -1.0f, +1.0f };
								bounds[3] = v4f_t{ +0.01f, +100.0f };
								
								proj[0][0] = (+2.0f * bounds[3][0]) / (bounds[0][1] - bounds[0][0]);
								proj[0][3] = (bounds[0][1] + bounds[0][0]) / (bounds[0][1] - bounds[0][0]);

								proj[1][1] = (+2.0f * bounds[3][0]) / (bounds[1][1] - bounds[1][0]);
								proj[1][3] = (bounds[1][1] + bounds[1][0]) / (bounds[1][1] - bounds[1][0]);
								
								proj[2][2] = (+2.0f * bounds[3][0]) / (bounds[2][1] - bounds[2][0]);
								proj[2][3] = (bounds[2][1] + bounds[2][0]) / (bounds[2][1] - bounds[2][0]);
								
								proj[3][3] = (-1.0f * (bounds[3][1] + bounds[3][0])) / (bounds[3][1] - bounds[3][0]);
								proj[4][3] = (-2.0f * (bounds[3][1] * bounds[3][0])) / (bounds[3][1] - bounds[3][0]);
								proj[3][4] = (-1.0f);
								
								auto& temp = proj * num_vec_t<v1f_t, 5u>({ icrd[0], icrd[1], icrd[2], icrd[3], +1.0f });
								icrd = temp / temp[4];
								vtx_crd = icrd;
								txr_crd = ((icrd - rect.m_center) / rect.m_halfsz + 1.0f) / 2.0f; // convert from vertex to texture coordinate;
								nrm_crd = (icrd - rect.m_center) / rect.m_halfsz;
							}
							if (NC_FALSE) {
								for (v1u_t idim(3u); idim < dcount; idim++) {
									vtx_crd[0u] += icrd[idim] / NC_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
									vtx_crd[1u] += icrd[idim] / NC_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
									vtx_crd[2u] += icrd[idim] / NC_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
									nrm_crd = vtx_crd;
								}
							}
							vbuf[ivert][0u].set<v3f_t>(vtx_crd);
							vbuf[ivert][1u].set<v2f_t>(txr_crd);
							vbuf[ivert][2u].set<v3f_t>(nrm_crd);
						}
						auto idata = rect.get_idata_wire(0u);
						for (v1u_t idx(0u); idx < idata.size(); idx++) { ibuf[idx].set<v1u_t>(idata[idx]); }
						vbuf.set_data();
						ibuf.set_data();
						NC_CHECK(layt.remake(), "remake error!", return NC_FALSE);
					}
					// d_circ
					if constexpr (NC_FALSE) {
						auto& layt = cmp_sys::get().new_ref<gfx_layt_t>().get_val();
						layt.get_layt().add_node<v3f_t>("vsi_vtx_crd").add_node<v2f_t>("vsi_txr_crd").add_node<v3f_t>("vsi_nrm_crd");
						auto& vbuf = cmp_sys::get().new_ref<gfx_buf_vtx_t>(layt, NC_NUM_POW(16u, dcount - 1u)).get_val();
						auto& ibuf = cmp_sys::get().new_ref<gfx_buf_idx_t>(mem_layt_tt<v1u_t>("index"), NC_NUM_POW(16u, dcount)).get_val();
						using circ_t = num_circ_t<dcount>;
						circ_t circ(circ_t::vert_t(0.0f), circ_t::vert_t(1.0f));
						for (v1u_t ivert(0u); ivert < vbuf.get_count(); ivert++) {
							auto icrd = circ.make_vert(circ, vbuf.get_count(), ivert);
							v3f_t vtx_crd = icrd;
							v2f_t txr_crd = ((icrd - circ.m_center) / circ.m_halfsz + 1.0f) / 2.0f; // convert from vertex to texture coordinate;
							v3f_t nrm_crd = (icrd - circ.m_center) / circ.m_halfsz;
							for (v1u_t idim(3u); idim < dcount; idim++) {
								vtx_crd[0u] += icrd[idim] / NC_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
								vtx_crd[1u] += icrd[idim] / NC_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
								vtx_crd[2u] += icrd[idim] / NC_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
								nrm_crd = vtx_crd;
							}
							//vbuf[ivert].set_elem(0u, vtx_crd, txr_crd, nrm_crd);
							vbuf[ivert][0u].set<v3f_t>(vtx_crd);
							vbuf[ivert][1u].set<v2f_t>(txr_crd);
							vbuf[ivert][2u].set<v3f_t>(nrm_crd);
						}
						auto idata = circ.get_idata_trig(vbuf.get_count(), 0u);
						for (v1u_t idx(0u); idx < idata.size(); idx++) { ibuf[idx].set<v1u_t>(idata[idx]); }
						vbuf.set_data();
						ibuf.set_data();
						NC_CHECK(layt.remake(), "remake error!", return NC_FALSE);
					}
				}
				// voronoi;
				if constexpr (NC_FALSE) {
					auto& layt = cmp_sys::get().new_ref<gfx_layt_t>().get_val();
					layt.get_layt().add_node<v2f_t>("vsi_vtx_crd").add_node<v2f_t>("vsi_txr_crd");
					auto& vbuf = cmp_sys::get().new_ref<gfx_buf_vtx_t>(layt, 4u).get_val<gfx_buf_t>();
					auto& ibuf = cmp_sys::get().new_ref<gfx_buf_idx_t>(mem_layt_tt<v1u_t>("index"), vbuf.get_count()).get_val<gfx_buf_t>();
					if constexpr (NC_TRUTH) {
						if constexpr (NC_FALSE) {
							for (v1u_t ivert = 0u; ivert < vbuf.get_count(); ivert++) {
								cv2f vtx_crd = circ2d_t::make_vert(circ2d_t(v2f_t{ 0.0f, 0.0f }, v2f_t{ 1.0f, 1.0f }), vbuf.get_count(), ivert);
								vbuf[ivert][0u] = v2f_t{ vtx_crd };
								vbuf[ivert][1u] = v2f_t{ (vtx_crd + 1.0f) / 2.0f };
								ibuf[ivert] = ivert;
							}
						}
						if constexpr (NC_TRUTH) {
							for (v1u_t ivert = 0u; ivert < vbuf.get_count(); ivert++) {
								cv2f vtx_crd = rect2d_t::make_vert(rect2d_t(v2f_t{ 0.0f, 0.0f }, v2f_t{ 1.0f, 1.0f }), ivert);
								vbuf[ivert][0u] = v2f_t{ vtx_crd };
								vbuf[ivert][1u] = v2f_t{ (vtx_crd + 1.0f) / 2.0f };
							}
							ibuf[0u] = 0u;
							ibuf[1u] = 1u;
							ibuf[2u] = 2u;
							ibuf[3u] = 2u;
							ibuf[4u] = 3u;
							ibuf[5u] = 0u;
						}
					}
					vbuf.set_data();
					ibuf.set_data();
					NC_CHECK(layt.remake(), "remake error!", return NC_FALSE);
				}
			}
			// texture voronoi update;
			if constexpr (NC_FALSE) {
				auto& image = cmp_sys::get().get_ref<gfx_txr_t>(0u).get_val<gfx_img_t>();
				image = make_pxl_voron(image.get_size_xy() * 16u, 24u);
			}
			// materials;
			if constexpr (NC_TRUTH) {
				// 3default
				if constexpr (NC_TRUTH) {
					auto& vshd = cmp_sys::get().new_ref<gfx_shd_t, gfx_shd_vtx_t>(R"(nc_gfx\src_glsl\default_3d_vtx.glsl)");
					auto& pshd = cmp_sys::get().new_ref<gfx_shd_t, gfx_shd_pxl_t>(R"(nc_gfx\src_glsl\default_3d_pxl.glsl)");
					auto& vshd_bind = cmp_sys::get().new_ref<gfx_bind_t>(
						vshd,
						gfx_bind_t::buf_list_tc{ cmp_sys::get().get_ref<gfx_buf_shd_t>(1u) },
						gfx_bind_t::txr_list_tc{ },
						gfx_bind_t::smp_list_tc{ }
					);
					auto& pshd_bind = cmp_sys::get().new_ref<gfx_bind_t>(
						pshd,
						gfx_bind_t::buf_list_tc{ },
						gfx_bind_t::txr_list_tc{ cmp_sys::get().get_ref<gfx_txr_t>(0u) },
						gfx_bind_t::smp_list_tc{ cmp_sys::get().get_ref<gfx_smp_t>(0u) }
					);
					auto& gmtl = cmp_sys::get().new_ref<gfx_mtl_t>(gfx_mtl_t::bind_list_t{ vshd_bind, pshd_bind });
				}
				// voronoi;
				if constexpr (NC_FALSE) {
					auto& vshd = cmp_sys::get().new_ref<gfx_shd_t, gfx_shd_vtx_t>(R"(nc_gfx/src_glsl/voronoi_2d_vtx.glsl)");
					auto& pshd = cmp_sys::get().new_ref<gfx_shd_t, gfx_shd_pxl_t>(R"(nc_gfx/src_glsl/voronoi_2d_pxl.glsl)");
					auto& vshd_bind = cmp_sys::get().new_ref<gfx_bind_t>(
						vshd,
						gfx_bind_t::buf_list_tc{ },
						gfx_bind_t::txr_list_tc{ },
						gfx_bind_t::smp_list_tc{ }
						);
					auto& pshd_bind = cmp_sys::get().new_ref<gfx_bind_t>(
						pshd,
						gfx_bind_t::buf_list_tc{ },
						gfx_bind_t::txr_list_tc{ },
						gfx_bind_t::smp_list_tc{ }
					);
					auto& gmtl = cmp_sys::get().new_ref<gfx_mtl_t>(gfx_mtl_t::bind_list_tc{ vshd_bind, pshd_bind });
				}
			}
			// render graph;
			if constexpr (NC_TRUTH) {
				auto& graph = cmp_sys::get().new_ref<gfx_graph_t>().get_val();
				graph
					.add_pass(
						gfx_pass_t(
							gfx_pass_t::cmd_list_tc{
							gfx_cmd_t(NC_GFX_CMD_IDX, NC_GFX_PRIM_POINTS,
									{
										cmp_sys::get().get_ref<gfx_buf_idx_t>(3u),
										cmp_sys::get().get_ref<gfx_layt_t>(1u),
										cmp_sys::get().get_ref<gfx_mtl_t>(0u),
										cmp_sys::get().get_ref<gfx_mode_depst_t>(0u)
									}
								),
							gfx_cmd_t(NC_GFX_CMD_IDX, NC_GFX_PRIM_LINES,
									{
										cmp_sys::get().get_ref<gfx_buf_idx_t>(3u),
										cmp_sys::get().get_ref<gfx_layt_t>(1u),
										cmp_sys::get().get_ref<gfx_mtl_t>(0u),
										cmp_sys::get().get_ref<gfx_mode_depst_t>(0u)
									}
								),
#						if (NC_FALSE)
							gfx_cmd_t(NC_GFX_CMD_IDX, NC_GFX_PRIM_TRIANGLES,
									{
										cmp_sys::get().get_ref<gfx_buf_idx_t>(3u),
										cmp_sys::get().get_ref<gfx_layt_t>(1u),
										cmp_sys::get().get_ref<gfx_mtl_t>(0u),
										cmp_sys::get().get_ref<gfx_mode_depst_t>(1u)
									}
								),
#						endif
							}
						)
					)
					.set_root(0u)
					;
				//graph.get_pass(0u).add_link(&graph.get_pass(1u));
				//graph.get_pass(1u).add_link(&graph.get_pass(2u));
				NC_CHECK(graph.remake(), "remake error!", return NC_FALSE);
			}
		}
		
		return NC_TRUTH;
	}
	v1bit_t app_state_game_t::quit() { return NC_TRUTH; }
	v1bit_t app_state_game_t::work()
	{
		auto keybod = app_eng::get().get_keybod();
		auto cursor = app_eng::get().get_cursor();
		auto timer = app_eng::get().get_timer();
		// transform update
		if constexpr (NC_TRUTH) {
			auto& camera = cmp_sys::get().get_ref<gfx_tform>(1u).get_val<gfx_tform_cam>();
			camera.update(keybod, cursor, timer);
		}
		// title setting
		if constexpr (NC_TRUTH) {
			static dstr_t title = app_eng::get().get_window()->get_title();
			char_t buf[256u];
			sprintf_s(buf, "%s||ups:%d||", &title[0], static_cast<v1u_t>(timer->get_ups()));
			app_eng::get().get_window()->set_title(&buf[0]);
		}
		return NC_TRUTH;
	}
	v1bit_t app_state_game_t::event_proc(nc_event_t& evt)
	{
		if (evt.has_sort(NC_EVSORT_KEYBOD)) {
			nc_event_keybod_t& kbd_evt = evt;
			switch (evt.get_type()) {
			case NC_EVTYPE_KEYBOD_RAISE:
				switch (kbd_evt.get_code()) {
				case NC_KEYCODE_M: {
					auto& camera = cmp_sys::get().get_ref<gfx_tform_cam>(1u).get_val();
					if (!app_eng::get().is_cursor_enabled()) {
						app_eng::get().set_cursor_enabled(NC_TRUTH);
						camera.set_mode(NC_CAMERA_2D);
					}
					else {
						app_eng::get().set_cursor_enabled(NC_FALSE);
						camera.set_mode(NC_CAMERA_3D);
					}
					break;
				}
				}
				break;
			}
		}
		else if (evt.has_sort(NC_EVSORT_WINDOW)) {
			nc_event_window_t& wnd_evt = evt;
			switch (wnd_evt.get_type()) {
			case NC_EVTYPE_WINDOW_SIZED: {
				if (cmp_sys::get().has_ref<gfx_tform_cam>(1u)) {
					auto& camera = cmp_sys::get().get_ref<gfx_tform_cam>(1u).get_val();
					camera.set_ratio(wnd_evt.get_val_x(), wnd_evt.get_val_y());
				}
				break;
			}
			}
		}
		else if (evt.has_sort(NC_EVSORT_APPLIC)) {
			nc_event_applic_t& app_evt = static_cast<nc_event_applic_t& > (evt);
			switch (app_evt.get_type()) {
			case NC_EVTYPE_APPLIC_DROPF: {
				if (strstr(app_evt.get_desc(), ".bmp")) {
				}
				break;
			}
			}
		}
		return NC_TRUTH;
	}
	// --==</core_methods>==--
}
namespace NC
{
	app_state_gui_t::app_state_gui_t() : app_state_t(type_info_t::get_name<app_state_gui_t>()) { }
	app_state_gui_t::~app_state_gui_t() { }
	// commands //
	v1bit_t app_state_gui_t::init()
	{
		NC_CHECK(gui_init(nc_gfx_eng::get().get_context()), "init error", return NC_FALSE);

		gui_ref ref;
		
		ref.make_ref<gui_of_app_eng>();
		ref->set_enabled(NC_TRUTH);
		m_gui_refs.push_back(ref);
		
		ref.make_ref<gui_of_io_sys>();
		ref->set_enabled(NC_TRUTH);
		m_gui_refs.push_back(ref);

		ref.make_ref<gui_of_nc_gfx_eng>();
		ref->set_enabled(NC_TRUTH);
		m_gui_refs.push_back(ref);

		return NC_TRUTH;
	}
	v1bit_t app_state_gui_t::quit()
	{
		gui_quit();
		return NC_TRUTH;
	}
	v1bit_t app_state_gui_t::work()
	{
		gui_begin_frame();

		if (NC_GUI::BeginMenuBar()) {
			if (NC_GUI::BeginMenu("view")) {
				for (auto& igui : m_gui_refs) { igui->draw_checkbox(); }
				NC_GUI::EndMenu();
			}
			NC_GUI::EndMenuBar();
		}
		for (auto& igui_of : m_gui_refs) { igui_of->on_draw(); }

		gui_end_frame();
		
		return NC_TRUTH;
	}
	v1bit_t app_state_gui_t::event_proc(nc_event_t& evt)
	{
		if (evt.has_type(NC_EVTYPE_WINDOW_SIZED)) {
			nc_event_window_t& wnd_evt = static_cast<nc_event_window_t&>(evt);
		}
		return NC_TRUTH;
	}
	// --==</core_methods>==--
}
#endif	// NC_API