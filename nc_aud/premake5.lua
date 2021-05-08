--==<native_code_audio/>==--
project("nc_aud")
	--general--
	--kind("consoleapp")
	kind("staticlib")
	--kind("sharedlib")
	--language--
	language("c")
	toolset("gcc")
	--libraries--
	links{
		"nc_cfg",
		"nc_lib",
		"nc_mem"
	}
	libdirs{
		"../nc_cfg/",
		"../nc_lib/",
		"../nc_mem/"
	}
	--filters--
	----platform_filters----
	filter("platforms:win64")
		system("windows")
	----system_filters----
	filter("system:windows")
		systemversion("latest")
	----language_filters----
	filter("language:c++")
		--standard--
		toolset("gcc")
		premake.tools.gcc.cxx = "g++.exe"
		cppdialect("c++17")
		clr("off")
		rtti("off")
		--source--
		files{"src_cpp/**.cpp","src_cpp/**.hpp"}
		includedirs {"src_cpp/"}
		--binary--
		targetdir ("bin_cpp/")
		objdir ("bin_cpp/")
	filter("language:c")
		--standard--
		toolset("gcc")
		premake.tools.gcc.cc = "gcc.exe"
		cdialect("c99")
		clr("off")
		files{"src_cxx/**.c","src_cxx/**.h"}
		includedirs {"src_cxx/"}
		--binary--
		targetdir ("bin_cxx/")
		objdir ("bin_cxx/")
	----file_filters----
--==</native_code_audio>==--