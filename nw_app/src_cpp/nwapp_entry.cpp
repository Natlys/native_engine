#include "nwapp_pch.hpp"

#define NWAPP_LAUNCH_TEST     1 << 0
#define NWAPP_LAUNCH_ENGINE   1 << 1
#define NWAPP_LAUNCH_CONSOLE  1 << 2
#define NWAPP_LAUNCH          NWAPP_LAUNCH_ENGINE

#include "nwapp.hpp"

int main(int nof_arguments, char* arguments[])
{
	try {
#if (NWAPP_LAUNCH & NWAPP_LAUNCH_ENGINE)
		NWAPP::core_engine game = NWAPP::core_engine("native_world");
		game.run();
		game.new_state<NWAPP::gui_core_state>();
		//game.new_state<NWAPP::gfx_core_state>();
		if (game.get_run_thread()->joinable()) { game.get_run_thread()->join(); }
#endif
#if (NWAPP_LAUNCH & NWAPP_LAUNCH_CONSOLE)
#endif
#if (NWAPP_LAUNCH & NWAPP_LAUNCH_TEST)
#	if true
#	endif
#	if false
		STARTUPINFO spInfo{ 0 };
		PROCESS_INFORMATION pcInfo{ 0 };
		if (!CreateProcess(
			LR"(F:\dev\lua_jit\lua_jit.exe)",
			//&(reinterpret_cast<TCHAR*>(R"(F:\dev\lua_jit\lua_jit.exe)"))[0],
			NULL,		// cmd arguments
			NULL,		// process is not inheritable
			NULL,		// not inherit the handle
			FALSE,		// handle inheritance to false
			0,			// no creation flags
			NULL,		// parrent's environment
			NULL,		// parent's directory
			&spInfo,
			&pcInfo
		)) {
			throw(NWAPP::error("failed to create a process", ::GetLastError()));
		}
		WaitForSingleObject(pcInfo.hProcess, INFINITE);
		CloseHandle(pcInfo.hProcess);
		CloseHandle(pcInfo.hThread);
#	endif
#endif
	}
	catch (NWAPP::a_error& exc) { std::cout << exc; }
	catch (std::exception& exc) { NW_ERR(exc.what()); }
	catch (...) { NW_ERR("somethig went wrong"); }

	return 0;
}