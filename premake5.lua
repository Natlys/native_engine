--==<nati_code>==--
workspace ("nati_code")
	--==general==--
	configurations ({"work","game"})
	characterset   ("ascii")
	floatingpoint  ("default")
	removeflags    ({"excludefrombuild"})
	--==build==--
	buildmessage   ("comp: %{file.basename};")
	prelinkmessage ("link: %{prj.name}")
	--==environment==--
	env = { } --info_table--
	env.name = "nc_gen"
	env.root = "." --root diretory with projects--
	env.proj = "%{env.root}/%{prj.name}" --project directory with source code--
	env.fdir = { } --file_directory--
	env.fdir.iput = "%{env.root}"
	env.fdir.oput = "%{env.proj}"
	env.fext = { } --file_extensions--
	env.fext.iput = "prem"
	env.fext.oput = "make"
	env.fobj = { } --file_objects--
	env.fobj.iput = "%{env.name}.%{env.fext.iput}"
	env.fobj.oput = "%{prj.name}.%{env.fext.oput}"
	env.arch = { } --architecture_to_use--
	env.plat = { } --platform_to_use--
	env.osys = { } --operating_system_to_use--
	env.lang = { } --language_to_use--
	--==architecture==--
	--x32bit--
	arch_x32 = { }
	arch_x32.name = "x32"
	--x64bit--
	arch_x64 = { }
	arch_x64.name = "x64"
	--==platform==--
	--windows--
	plat_win = { }
	plat_win.name = "win64"
	--==operating_system==--
	--windows--
	osys_win = { }
	osys_win.name = "windows"
	osys_win.vers = "latest"
	--==language==--
	--cplusplus--
	lang_cpp = { } --info_table--
	lang_cpp.name = "c++"
	lang_cpp.comp = "g++.exe"
	lang_cpp.fext = { } --file_extensions--
	lang_cpp.fext.iput = "cpp"
	lang_cpp.fext.oput = "out"
	lang_cpp.fdir = { } --file_locations--
	lang_cpp.fdir.iput = "%{env.proj}/src_cpp"
	lang_cpp.fdir.oput = "%{env.proj}/bin_cpp"
	lang_cpp.fobj = { } --file_objects--
	lang_cpp.fobj.iput = {"%{lang_cpp.fdir.iput}/**.cpp","%{lang_cpp.fdir.iput}/**.hpp"}
	lang_cpp.fobj.oput = "%{lang_cpp.fdir.oput}/**.out"
	--clanguage--
	lang_cxx = { } --info_table--
	lang_cxx.name = "c"
	lang_cxx.comp = "gcc.exe"
	lang_cxx.fext = { } --file_extensions--
	lang_cxx.fext.iput = "cxx"
	lang_cxx.fext.oput = "out"
	lang_cxx.fdir = { } --file_locations--
	lang_cxx.fdir.iput = "%{env.proj}/src_cxx"
	lang_cxx.fdir.oput = "%{env.proj}/bin_cxx"
	lang_cxx.fobj = { } --file_objects--
	lang_cxx.fobj.iput = {"%{lang_cxx.fdir.iput}/**.cxx","%{lang_cxx.fdir.iput}/**.hxx"}
	lang_cxx.fobj.oput = "%{lang_cxx.fdir.oput}/**.out"
	--csharplan--
	lang_csh = { } --info_table--
	lang_csh.name = "c#"
	lang_csh.comp = "csc.exe"
	lang_csh.fext = { } --file_extensions--
	lang_csh.fext.iput = "csh"
	lang_csh.fext.oput = "out"
	lang_csh.fdir = { } --file_locations--
	lang_csh.fdir.iput = "%{env.proj}/src_csh"
	lang_csh.fdir.oput = "%{env.proj}/bin_csh"
	lang_csh.fobj = { } --file_objects--
	lang_csh.fobj.iput = {"%{lang_csh.fdir.iput}/**.csh"}
	lang_csh.fobj.oput = "%{lang_csh.fdir.oput}/**.out"
	--lualangua--
	lang_lua = { } --info_table--
	lang_lua.name = "lua"
	lang_lua.comp = "lua.exe"
	lang_lua.fext = { } --file_extensions--
	lang_lua.fext.iput = "lua"
	lang_lua.fext.oput = "out"
	lang_lua.fdir = { } --file_locations--
	lang_lua.fdir.iput = "%{env.proj}/src_lua"
	lang_lua.fdir.oput = "%{env.proj}/bin_lua"
	lang_lua.fobj = { } --file_objects--
	lang_lua.fobj.iput = {"%{lang_lua.fdir.iput}/**.lua","%{lang_lua.fdir.iput}/**.cxx"}
	lang_lua.fobj.oput = "%{lang_lua.fdir.oput}/**.out"
	--==environment_setting==--
	env.arch = arch_x64
	env.plat = plat_win
	env.osys = osys_win
	env.lang = lang_cxx
	location     (env.fdir.iput)
	filename     (env.fobj.iput)
	architecture (env.arch.name)
	platforms    (env.plat.name)
	system       (env.osys.name)
	language     (env.lang.name)
	--==compiler==--
	ncc = { } --info_table--
	ncc = premake.tools.gcc
	ncc.name = "ncc"
	ncc.warn = "off"
	ncc.getlinks = premake.tools.gcc.getlinks
	ncc.getcflags = premake.tools.gcc.getcflags
	ncc.getcxxflags = premake.tools.gcc.getcxxflags
	ncc.getcppflags = premake.tools.gcc.getcppflags
	ncc.getldflags = premake.tools.gcc.getldflags
	ncc.getdefines = premake.tools.gcc.getdefines
	ncc.getincludedirs = premake.tools.gcc.getincludedirs
	ncc.getlibrarydirectories = premake.tools.gcc.getlibrarydirectories
	ncc.getmakesettings = premake.tools.gcc.getmakesettings
	ncc.getcforceincludes = premake.tools.gcc.getcforceincludes
	ncc.gettoolname = function(cfg, tool)
		if (tool == "cc") then return lang_cxx.comp end
		if (tool == "cxx") then return lang_cpp.comp end
		if (tool == "ar") then return "ar" end
		return "default"
		end
	premake.tools.ncc = ncc
	toolset  (ncc.name)
	warnings (ncc.warn)
	--==debugger==--
	ncd = { }
	ncd.name = "gdb"
	debugger (ncd.name)
	--==filters==--
	--configuration--
	filter ("configurations:work")
		--optimize      ("debug")
		symbols       ("on")
		floatingpoint ("strict")
		runtime       ("debug")
	filter ("configurations:play")
		optimize      ("full")
		symbols       ("off")
		floatingpoint ("fast")
		runtime       ("release")
	--architecture--
	filter ("architecture:%{arch_x32.name}")
	filter ("architecture:%{arch_x64.name}")
	--platform--
	filter ("platforms:%{plat_win.name}")
	--operating_system--
	filter ("system:%{osys_win.name}")
	--language_filters--
	filter ("language:c++")
		--general--
		cppdialect        ("c++14")
		entrypoint        ("main")
		callingconvention ("cdecl")
		rtti ("off")
		clr  ("off")
		--build--
		buildoptions ({"-fpermissive"})
	filter ("language:c")
	--standard--
		cdialect          ("c99")
		entrypoint        ("main")
		callingconvention ("cdecl")
		rtti ("off")
		clr  ("off")
		--build--
		buildoptions ({"-fpermissive"})
	filter ("language:c#")
		--standard--
		csversion ("4.0.0")
		rtti ("off")
		clr  ("on")
		--build--
		buildoptions ({ })
	--file_filters--
	filter ("files:**.%{lang_cpp.fext.iput}")
		--general--
		compileas ("c++")
		pchheader ("nc_pch.hpp")
		pchsource ("%{lang_cpp.fdir.iput}/nc_pch.%{lang_cpp.fext.iput}")
		flags ({"c++14"})
		--build--
		buildaction  ("compile")
		buildmessage ("compiling %{file.relpath};")
		buildinputs  ("%{file.relpath}")
		buildoutputs ("%{lang_cpp.fdir.oput}/%{file.basename}.%{lang_cpp.fext.oput}")
		--buildcommands ({"%{lang_cpp.comp} -o %{cfg.objdir}/%{file.basename}.%{lang_cpp.fext.oput}"})
		rules ({"comp_cpp"})
	filter ("files:**.%{lang_cxx.fext.iput}")
		--general--
		compileas ("c")
		pchheader ("default")
		pchsource ("default")
		flags ({"nopch", "c99"})
		--build--
		buildaction  ("compile")
		buildmessage ("compiling %{file.relpath};")
		buildinputs  ("%{file.relpath}")
		buildoutputs ("%{lang_cxx.fobj.iput}/%{file.basename}.%{lang_cxx.fext.oput}")
		--buildcommands ({"%{lang_cxx.comp} -o %{cfg.objdir}/%{file.basename}.%{lang_cxx.fext.oput}"})
		rules ({"comp_cxx"})
	filter ("files:**.%{lang_csh.fext.iput}")
		--general--
		compileas ("default")
		pchheader ("default")
		pchsource ("default")
		flags ({"nopch", "c99"})
		--build--
		buildaction  ("compile")
		buildmessage ("compiling %{file.relpath};")
		buildinputs  ("%{file.relpath}")
		buildoutputs ("%{lang_csh.fobj.iput}/%{file.basename}.%{lang_csh.fext.oput}")
		--buildcommands ({"%{lang_csh.comp} -o %{cfg.objdir}/%{file.basename}.%{lang_csh.fext.oput}"})
		rules ({"comp_csh"})
	filter ("files:**.%{lang_lua.fext.iput}")
		--general--
		--build--
		buildaction  ("compile")
		buildmessage ("compiling %{file.relpath};")
		buildinputs  ("%{file.relpath}")
		buildoutputs ("%{lang_lua.fdir.iput}/%{file.basename}.%{lang_lua.fext.oput}")
		--buildcommands ({"%{lang_lua.comp} -o %{cfg.objdir}/%{file.basename}.%{lang_lua.fext.oput}"})
		rules ({"comp_lua"})
	--==rules==--
	rule ("comp_cpp")
		display       ("nati_code_compiler")
		fileextension (".%{lang_cpp.fext.iput}")
		buildmessage  ("compile %{filename} with %{lang_cpp.comp}")
		buildcommands ("%{lang_cpp.comp} -c %{fullpath} -o %{intdir}/%{filename}.%{lang_cpp.fext.oput}")
	rule ("comp_cxx")
		display       ("nati_code_compiler")
		fileextension (".%{lang_cxx.fext.iput}")
		buildmessage  ("compile %{filename} with %{lang_cxx.comp}}")
		buildcommands ("%{lang_cxx.comp} -c %{fullpath} -o %{intdir}/%{filename}.%{lang_cxx.fext.oput}")
	rule ("comp_csh")
		display       ("nati_code_compiler")
		fileextension (".%{lang_csh.fext.iput}")
		buildmessage  ("compile %{filename} with %{lang_csh.comp}}")
		buildcommands ("%{lang_csh.comp} -c %{fullpath} -o %(intdir)/%{filename}.%{lang_csh.fext.oput}")
	rule ("comp_lua")
		display       ("nati_code_compiler")
		fileextension (".%{lang_lua.fext.iput}")
		buildmessage  ("compile %{filename} with %{lang_lua.comp}")
		buildcommands ("%{lang_lua.comp} -c %{fullpath} -o %{intdir}/%{filename}.%{lang_lua.fext.oput}")
	--==projects==--
	--nati_code_configuration--
	project ("nc_cfg")
		--==general==--
		kind     ("staticlib")
		location ("%{env.fdir.oput}")
		filename ("%{env.fobj.oput}")
		--==libraries==--
		links   ({ })
		libdirs ({ })
		--==filters==--
		--config_work--
		filter ("configurations:work")
		filter ("configurations:play")
		--architecture--
		filter ("architecture:%{arch_x32.name}")
		filter ("architecture:%{arch_x64.name}")
		--platform--
		filter ("platforms:%{plat_win.name}")
		--operating_system--
		filter ("system:%{osys_win.name}")
		--language--
		filter ("language:c++")
			--source--
			files       (lang_cpp.fobj.iput)
			includedirs (lang_cpp.fdir.iput)
			--binary--
			objdir      (lang_cpp.fdir.oput)
			targetdir   (lang_cpp.fdir.oput)
		filter ("language:c")
			--source--
			files       (lang_cxx.fobj.iput)
			includedirs (lang_cxx.fdir.iput)
			--binary--
			objdir      (lang_cxx.fdir.oput)
			targetdir   (lang_cxx.fdir.oput)
		filter ("language:c#")
			--source--
			files       (lang_csh.fobj.iput)
			includedirs (lang_csh.fdir.iput)
			--binary--
			objdir      (lang_csh.fdir.oput)
			targetdir   (lang_csh.fdir.oput)
		--file_filters--
		filter ("files:**.%{lang_cpp.fext.iput}")
		filter ("files:**.%{lang_cxx.fext.iput}")
		filter ("files:**.%{lang_csh.fext.iput}")
		filter ("files:**.%{lang_lua.fext.iput}")
	--nati_code_console--
	project ("nc_cmd")
	--==general==--
	kind     ("consoleapp")
	location ("%{env.fdir.oput}")
	filename ("%{env.fobj.oput}")
	--==libraries==--
	links   ({ "nc_cfg" })
	libdirs ({ "%{env.root}/nc_cfg" })
	--==filters==--
	--config_work--
	filter ("configurations:work")
	filter ("configurations:play")
	--architecture--
	filter ("architecture:%{arch_x32.name}")
	filter ("architecture:%{arch_x64.name}")
	--platform--
	filter ("platforms:%{plat_win.name}")
	--operating_system--
	filter ("system:%{osys_win.name}")
	--language--
	filter ("language:c++")
		--source--
		files       (lang_cpp.fobj.iput)
		includedirs (lang_cpp.fdir.iput)
		--binary--
		objdir      (lang_cpp.fdir.oput)
		targetdir   (lang_cpp.fdir.oput)
	filter ("language:c")
		--source--
		files       (lang_cxx.fobj.iput)
		includedirs (lang_cxx.fdir.iput)
		--binary--
		objdir      (lang_cxx.fdir.oput)
		targetdir   (lang_cxx.fdir.oput)
	filter ("language:c#")
		--source--
		files       (lang_csh.fobj.iput)
		includedirs (lang_csh.fdir.iput)
		--binary--
		objdir      (lang_csh.fdir.oput)
		targetdir   (lang_csh.fdir.oput)
	--file_filters--
	filter ("files:**.%{lang_cpp.fext.iput}")
	filter ("files:**.%{lang_cxx.fext.iput}")
	filter ("files:**.%{lang_csh.fext.iput}")
	filter ("files:**.%{lang_lua.fext.iput}")
--end_of_file--