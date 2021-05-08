--==<native_code/>==--
workspace("native_code")
	--general--
	configurations{"work", "test", "play"}
	characterset("ascii")
	floatingpoint("default")
	removeflags("excludefrombuild")
	--tools--
	cpp_comp_name = "F:\\dev\\gcc\\bin\\g++.exe"
	cxx_comp_name = "F:\\dev\\gcc\\bin\\gcc.exe"
	lua_comp_name = "F:\\dev\\lua\\bin\\luaj.exe"
	--native_code_compiler--
	premake.tools.ncc = premake.tools.gcc
	premake.tools.ncc.getlinks = premake.tools.gcc.getlinks
	premake.tools.ncc.getcflags = premake.tools.gcc.getcflags
	premake.tools.ncc.getcxxflags = premake.tools.gcc.getcxxflags
	premake.tools.ncc.getcppflags = premake.tools.gcc.getcppflags
	premake.tools.ncc.getldflags = premake.tools.gcc.getldflags
	premake.tools.ncc.getdefines = premake.tools.gcc.getdefines
	premake.tools.ncc.getincludedirs = premake.tools.gcc.getincludedirs
	premake.tools.ncc.getlibrarydirectories = premake.tools.gcc.getlibrarydirectories
	premake.tools.ncc.getmakesettings = premake.tools.gcc.getmakesettings
	premake.tools.ncc.getcforceincludes = premake.tools.gcc.getcforceincludes
	premake.tools.ncc.gettoolname = function(cfg, tool)
		if (tool == "cc") then return cxx_comp_name end
		if (tool == "cxx") then return cpp_comp_name end
		if (tool == "ar") then return "ar" end
		return "default"
		end
	--environment--
	debugger("gdb")
	warnings("off")
	--operation_system--
	architecture ("x64")
	platforms("win64")
	--language--
	language("c")
	toolset("ncc")
	--filters--
	----platform_filters----
	filter("platforms:win64")
		system("windows")
	----system_filters----
	filter("system:windows")
		systemversion("latest")
	----config_filters----
	filter("configurations:work")
		symbols("on")
		floatingpoint("strict")
		runtime("debug")
	filter("configurations:test")
		optimize("on")
		floatingpoint("fast")
		runtime("release")
	filter("configurations:play")
		optimize("full")
		floatingpoint("fast")
		runtime("release")
	----language_filters----
	filter("language:c++")
		--entrypoint("entry")
		cppdialect("c++17")
		rtti("off")
		--source--
		files{"%{prj.name}/src_cpp/**.cpp","%{prj.name}/src_cpp/**.hpp"}
		includedirs {"%{prj.name}/src_cpp/"}
		--binary--
		targetdir ("%{prj.name}/bin_cpp/")
		objdir ("%{prj.name}/bin_cpp/")
	filter("language:c")
		--standard--
		--entrypoint("entry")
		cdialect("c99")
		clr("off")
		--source--
		files{"%{prj.name}/src_cxx/**.c","%{prj.name}/src_cxx/**.h"}
		includedirs {"%{prj.name}/src_cxx/"}
		--binary--
		targetdir ("%{prj.name}/bin_cxx/")
		objdir ("%{prj.name}/bin_cxx/")
	filter("language:c#")
		--standard--
		csversion("4.0.0")
		rtti("off")
	----cxx_file_filters----
	filter("files:**.c")
		compileas("c")
		pchheader("default")
		pchsource("default")
		flags{"nopch", "c99"}
	----cpp_file_filters----
	filter("files:**.cpp")
		compileas("c++")
		pchheader("nc_pch.hpp")
		pchsource("src_cpp/nc_pch.cpp")
		flags{"c++14"}
	----cs_file_filters----
	filter("files:**.cs")
	----lua_file_filters----
	filter("files:**.lua")
		buildinputs("%(files)")
		buildoutputs("%(cfg.objdir)/(file.basename).out")
		buildmessage("compiling %(file.relpath);")
		buildcommands("luac -o %(cfg.objdir)/%(file.basename).out \"%(file.relpath))\"")
		removeflags("excludefrombuild")
	----hlsl_file_filters----
	filter("files:**.hlsl")
		flags("excludefrombuild")
		shadermodel("5.0")
		--shaderassembler("assemblycode")
		--shaderobjectfileoutput("../nc_gfx/src_hlsl/%{file.basename}.cso")
		--shaderassembleroutput("../nc_gfx/src_hlsl/%{file.basename}.asm")
	filter("files:**_vtx.hlsl")
		shadertype("vertex")
	filter("files:**_pxl.hlsl")
		shadertype("pixel")
	filter("files:**_gmt.hlsl")
		shadertype("geometry")
	----glsl_file_filters----
	filter("files:**.glsl")
		flags("excludefrombuild")
	--includes--
	--==<config>==--
	include("nc_cfg")
	--==<library>==--
	include("nc_lib")
	--==<numbers>==--
	include("nc_num")
	--==<memory>==--
	include("nc_mem")
	--==<audio>==--
	include("nc_aud")
	--==<input_output>==--
	include("nc_iop")
	--==<console>==--
	include("nc_cmd")
	--==<lua_library>==--
	--include("nc_lua")
	--==<graphics>==--
	--include("nc_gfx")
	--==<graphical_user_interface>==--
	--include("nc_gui")
	--==<application>==--
	include("nc_app")
	--rules--
--	rules{"comp_cpp", "comp_cxx", "comp_lua"}
--	rule("comp_cpp")
--		fileextension(".cpp")
--		buildmessage("compile %(filename) with "..cpp_comp_name)
--		buildcommands("%(cpp_comp_name) -c %(fullpath) -o %(intdir)/%(filename).luac")
--	rule("comp_cxx")
--		fileextension(".cxx")
--		buildmessage("compile %(filename) with "..cxx_comp_name)
--		buildcommands("%(cxx_comp_name) -c %(fullpath) -o %(intdir)/%(filename).luac")
--	rule("comp_lua")
--		fileextension(".lua")
--		buildmessage("compile %(filename) with "..lua_comp_name)
--		buildcommands("%(lua_comp_name) -c %(fullpath) -o %(intdir)/%(filename).luac")
--==</native_code>==--