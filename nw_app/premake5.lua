--==<native_application>==--
--[[ cplusplus
project("nw_app")
	kind("consoleapp")
	staticruntime("on")
	language("c++")
	cppdialect("c++17")
	targetdir(dir_out_res)
	objdir(dir_out_int)
	pchheader("nw_app_pch.hpp")
	pchsource("src_cpp/nw_app_pch.cpp")
	files{ "src_cpp/**.cpp", "src_cpp/**.hpp", "src_cpp/**.rc"" }
	includedirs{ "%{dir_cpp.own}", "%{dir_cpp.nw_lib}", "%{dir_cpp.nw_lua}", "%{dir_cpp.nw_gfx}", "%{dir_cpp.nw_gui}", "%{dir_cpp.nw_cmd}" }
	libdirs{ "%{dir_lib.nw_lib}", "%{dir_lib.nw_lua}", "%{dir_lib.nw_gfx}", "%{dir_lib.nw_gui}", "%{dir_lib.nw_cmd}" }
	links{ "nw_lib", "nw_lua", "nw_gfx", "nw_gui", "nw_cmd" }
	]]
--[[ clanguage
	project("nw_app")
	kind("consoleapp")
	staticruntime("on")
	language("c")
	targetdir("bin_c98")
	objdir("bin_c98")
	files{ "src_c98/**.cpp", "src_c98/**.hpp", "src_c98/**.rc" }
	includedirs{ "src_c98" }
]]
--==</native_application>==--