--==<native_application>==--
project("nw_app")
	kind("consoleapp")
	staticruntime("on")
	language("c++")
	cppdialect("c++17")
	targetdir(dir_out_res)
	objdir(dir_out_int)
	pchheader("nwapp_pch.hpp")
	pchsource("src_cpp/nwapp_pch.cpp")
	files{ "%{dir_cpp.own}**.c**", "%{dir_cpp.own}**.h**", dir_data.."**.h", dir_data.."**.rc",  dir_data.."**.aps" }
	includedirs{ "%{dir_cpp.own}", "%{dir_cpp.nw_lib}", "%{dir_cpp.nw_lua}", "%{dir_cpp.nw_gfx}", "%{dir_cpp.nw_gui}", "%{dir_cpp.nw_cmd}", dir_data }
	libdirs{ "%{dir_lib.nw_lib}", "%{dir_lib.nw_lua}", "%{dir_lib.nw_gfx}", "%{dir_lib.nw_gui}", "%{dir_lib.nw_cmd}" }
	links{ "nw_lib", "nw_lua", "nw_gfx", "nw_gui", "nw_cmd" }
--==</native_application>==--