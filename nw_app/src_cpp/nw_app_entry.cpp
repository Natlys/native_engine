#include "nw_app_pch.hpp"

#define NW_APP_LAUNCH_TEST     1 << 0
#define NW_APP_LAUNCH_ENGINE   1 << 1
#define NW_APP_LAUNCH_CONSOLE  1 << 2
#define NW_APP_LAUNCH          NW_APP_LAUNCH_ENGINE

#include "nw_app.hpp"

#if (defined NW_APP_LAUNCH)
int main(int nof_arguments, char* arguments[])
{
	try {
#	if (NW_APP_LAUNCH & NW_APP_LAUNCH_ENGINE)
		auto& game = NW::app_engine::get();
		NW_CHECK(game.work_init(), "init error!", return -1);
		game.add_state<NW::app_state_game_t>();
		// game.add_state<NW::app_state_gui_t>();
		if (game.get_work_flow().joinable()) { game.get_work_flow().join(); }
#	endif
#	if (NW_APP_LAUNCH & NW_APP_LAUNCH_CONSOLE)
#	endif
#	if (NW_APP_LAUNCH & NW_APP_LAUNCH_TEST)
		NW_CHECK(NW::mem_sys::get().init(1 << 23), "init error!", return NW_FALSE);
		NW_CHECK(NW::mem_sys::get().quit(), "quit error!", return NW_FALSE);
#	endif
	}
	catch (NW::error_t& exc) { NW_ERROR(exc.get_str(), return -1); }
	catch (std::exception& exc) { NW_ERROR(exc.what(), return -1); }
	catch (...) { NW_ERROR("somethig went wrong", return -1); }

	return 0;
}
#endif	// NW_APP_LAUNCH