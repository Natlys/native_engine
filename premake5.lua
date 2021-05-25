--==<native_code>==--
workspace("native_code")
	--general--
	configurations{"work", "game"}
	characterset("ascii")
	floatingpoint("default")
	removeflags{"excludefrombuild"}
	--build--
	buildmessage("comp: %{file.basename};")
	prelinkmessage("link: %{prj.name}")
	--environment--
	cpp_comp_name = "F:\\dev\\gcc\\bin\\g++.exe"
	cxx_comp_name = "F:\\dev\\gcc\\bin\\gcc.exe"
	lua_comp_name = "F:\\dev\\lua\\bin\\luaj.exe"
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
	toolset("ncc")
	debugger("gdb")
	warnings("off")
	--operational_system--
	architecture ("x64")
	platforms("win64")
	--language--
	--language("c++")
	language("c")
	--filters--
	--==platform_filters==--
	filter("platforms:win64")
		system("windows")
	--==system_filters==--
	filter("system:windows")
		systemversion("latest")
	--==config_filters==--
	filter("configurations:work")
		symbols("on")
		floatingpoint("strict")
		runtime("debug")
	filter("configurations:play")
		optimize("full")
		floatingpoint("fast")
		runtime("release")
	--==language_filters==--
	filter("language:c++")
		--general--
		entrypoint("main")
		callingconvention("cdecl")
		cppdialect("c++17")
		rtti("off")
		--build--
		buildoptions{ "-fpermissive" }
		--source--
		files{"%{prj.name}/src_cpp/**.cpp","%{prj.name}/src_cpp/**.hpp"}
		includedirs {"%{prj.name}/src_cpp/"}
		--binary--
		targetdir ("%{prj.name}/bin_cpp/")
		objdir ("%{prj.name}/bin_cpp/")
	filter("language:c")
	--standard--
		entrypoint("main")
		callingconvention("cdecl")
		cdialect("c99")
		clr("off")
		--build--
		buildoptions{ "-fpermissive" }
		--source--
		files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
		includedirs {"%{prj.name}/src_cxx/"}
		--binary--
		targetdir ("%{prj.name}/bin_cxx/")
		objdir ("%{prj.name}/bin_cxx/")
	filter("language:c#")
		--standard--
		csversion("4.0.0")
		rtti("off")
	--==file_filters==--
	filter("files:**.cpp")
		--general--
		compileas("c++")
		pchheader("nc_pch.hpp")
		pchsource("src_cpp/nc_pch.cpp")
		flags{"c++14"}
		--build--
		buildaction("compile")
		buildmessage("compiling %{file.relpath};")
		buildinputs("%{file.relpath}")
		buildoutputs("%{prj.name}/bin_cxx/%{file.basename}.out")
		--buildcommands{"%{cpp_comp_name} -o %{cfg.objdir}/%{file.basename}.out"}
		rules{"comp_cpp"}
	filter("files:**.cxx")
		--general--
		compileas("c")
		pchheader("default")
		pchsource("default")
		flags{"nopch", "c99"}
		--build--
		buildaction("compile")
		buildmessage("compiling %{file.relpath};")
		buildinputs("%{file.relpath}")
		buildoutputs("%{prj.name}/bin_cxx/%{file.basename}.out")
		--buildcommands{"%{cxx_comp_name} -o %{cfg.objdir}/%{file.basename}.out"}
		rules{"comp_cxx"}
	filter("files:**.csh")
		--general--
	filter("files:**.lua")
		--general--
		--build--
		buildaction("compile")
		buildmessage("compiling %{file.relpath};")
		buildinputs("%{file.relpath}")
		buildoutputs("%{prj.name}/bin_lua/%{file.basename}.out")
		--buildcommands{"%{lua_comp_name} -o %{cfg.objdir}/%{file.basename}.out"}
		rules{"comp_lua"}
	filter("files:**.hlsl")
		flags("excludefrombuild")
		shadermodel("5.0")
		--shaderassembler("assemblycode")
		--shaderobjectfileoutput("../nc_gfx/src_hlsl/%(file.basename).cso")
		--shaderassembleroutput("../nc_gfx/src_hlsl/%(file.basename).asm")
	filter("files:**_vtx.hlsl")
		shadertype("vertex")
	filter("files:**_pxl.hlsl")
		shadertype("pixel")
	filter("files:**_gmt.hlsl")
		shadertype("geometry")
	filter("files:**.glsl")
		flags{"excludefrombuild"}
	--rules--
	rule("comp_cpp")
		display("native_code_compiler")
		fileextension(".cpp")
		buildmessage("compile %(filename) with "..cpp_comp_name)
		buildcommands("%(cpp_comp_name) -c %(fullpath) -o %(intdir)/%(filename).out")
	rule("comp_cxx")
		display("native_code_compiler")
		fileextension(".cxx")
		buildmessage("compile %(filename) with "..cxx_comp_name)
		buildcommands("%(cxx_comp_name) -c %(fullpath) -o %(intdir)/%(filename).out")
	rule("comp_lua")
		display("native_code_compiler")
		fileextension(".lua")
		buildmessage("compile %(filename) with "..lua_comp_name)
		buildcommands("%(lua_comp_name) -c %(fullpath) -o %(intdir)/%(filename).out")
	--==native_code_application==--
	project("nc_app")
		--general--
		location("%{prj.name}/")
		kind("consoleapp")
		--libraries--
		links{"nc_cfg","nc_lib","nc_mem","nc_iop","nc_lua"}
		libdirs{"nc_cfg/","nc_lib/","nc_mem/","nc_iop/","nc_lua/"}
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cpp/**.cpp","%{prj.name}/src_cpp/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
	--==native_code_audio==--
	project("nc_aud")
		--general--
		location("%{prj.name}/")
		kind("none")
		--libraries--
		links{"nc_cfg","nc_lib","nc_mem","nc_iop"}
		libdirs{"nc_cfg/","nc_lib/","nc_mem/","nc_iop/"}
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cxx/**.cpp","%{prj.name}/src_cxx/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
	--==<native_code_config>==--
	project("nc_cfg")
		--general--
		location("%{prj.name}/")
		kind("staticlib")
		--libraries--
		links{ }
		libdirs{ }
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cxx/**.cpp","%{prj.name}/src_cxx/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
	--==<native_code_console>==--
	project("nc_cmd")
		--general--
		location("%{prj.name}/")
		kind("none")
		--libraries--
		links{"nc_cfg","nc_lib","nc_mem","nc_iop"}
		libdirs{"nc_cfg/","nc_lib/","nc_mem/","nc_iop/"}
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cxx/**.cpp","%{prj.name}/src_cxx/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
	--==<native_code_entity_component_system>==--
	project("nc_ecs")
		--general--
		location("%{prj.name}/")
		kind("none")
		--libraries--
		links{"nc_cfg","nc_lib","nc_mem","nc_iop"}
		libdirs{"nc_cfg/","nc_lib/","nc_mem/","nc_iop/"}
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cxx/**.cpp","%{prj.name}/src_cxx/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
	--==<native_code_graphics>==--
	project("nc_gfx")
		--general--
		location("%{prj.name}/")
		kind("none")
		--libraries--
		links{"nc_cfg","nc_lib","nc_mem","nc_iop"}
		libdirs{"nc_cfg/","nc_lib/","nc_mem/","nc_iop/"}
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cxx/**.cpp","%{prj.name}/src_cxx/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
	--==<native_code_graphical_user_interface>==--
	project("nc_gui")
		--general--
		location("%{prj.name}/")
		kind("none")
		--libraries--
		links{"nc_cfg","nc_lib","nc_mem","nc_iop"}
		libdirs{"nc_cfg/","nc_lib/","nc_mem/","nc_iop/"}
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cxx/**.cpp","%{prj.name}/src_cxx/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
	--==<native_code_input_output>==--
	project("nc_iop")
		--general--
		location("%{prj.name}/")
		kind("none")
		--libraries--
		links{"nc_cfg"}
		libdirs{"nc_cfg/"}
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cxx/**.cpp","%{prj.name}/src_cxx/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
	--==<native_code_lua_library>==--
	project("nc_lua")
		--general--
		location("%{prj.name}/")
		kind("none")
		--libraries--
		links{"nc_cfg"}
		libdirs{"nc_cfg/"}
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cxx/**.cpp","%{prj.name}/src_cxx/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
	--==<native_code_library>==--
	project("nc_lib")
		--general--
		location("%{prj.name}/")
		kind("staticlib")
		--libraries--
		links{"nc_cfg"}
		libdirs{"nc_cfg/"}
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cxx/**.cpp","%{prj.name}/src_cxx/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
	--==<native_code_memory>==--
	project("nc_mem")
		--general--
		location("%{prj.name}/")
		kind("none")
		--libraries--
		links{"nc_cfg"}
		libdirs{"nc_cfg/"}
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cxx/**.cpp","%{prj.name}/src_cxx/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
	--==native_code_numbers==--
	project("nc_num")
		--general--
		location("%{prj.name}/")
		kind("none")
		--libraries--
		links{"nc_cfg"}
		libdirs{"nc_cfg/"}
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"%{prj.name}/src_cxx/**.cpp","%{prj.name}/src_cxx/**.hpp"}
			includedirs {"%{prj.name}/src_cpp/"}
			--binary--
			targetdir ("%{prj.name}/bin_cpp/")
			objdir ("%{prj.name}/bin_cpp/")
		filter("language:c")
			--source--
			files{"%{prj.name}/src_cxx/**.cxx","%{prj.name}/src_cxx/**.hxx"}
			includedirs {"%{prj.name}/src_cxx/"}
			--binary--
			targetdir ("%{prj.name}/bin_cxx/")
			objdir ("%{prj.name}/bin_cxx/")
		filter("language:c#")
			--source--
			files{"%{prj.name}/src_csh/**.csh"}
			includedirs {"%{prj.name}/src_csh/"}
			--binary--
			targetdir ("%{prj.name}/bin_csh/")
			objdir ("%{prj.name}/bin_csh/")
--end_of_file--