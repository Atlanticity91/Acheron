project "Acheron"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	--- PROJECT OUTPUT
	location "%{OutputDirs.Solution}"
	targetdir "%{OutputDirs.Bin}%{cfg.buildcfg}/"
	debugdir "%{OutputDirs.Bin}%{cfg.buildcfg}/"
	objdir "%{OutputDirs.BinInt}%{prj.name}-%{cfg.buildcfg}"

	--- PROJECT INCLUDE DIRS
	includedirs "%{IncludeDirs.Acheron}"

	--- PROJECT EXTERNAL INCLUDE DIRS
	externalincludedirs "%{IncludeDirs.Acheron}"

	--- PRECOMPILED HEADER
	pchheader "_acheron_pch.h"
	pchsource "../Acheron/_acheron_pch.cpp"

	--- PROJECT SOURCE FILES
	files { 
		"%{IncludeDirs.Acheron}**.h", 
		"%{IncludeDirs.Acheron}**.cpp" 
	}

	-- LINUX
	filter "system:linux"
		systemversion "latest"
		buildoptions { "-fPIC" }

		--- LINUX SPECIFIC DEFINES
		defines { "LINUX" }

	-- WINDOWS
	filter "system:windows"
		systemversion "latest"
		cppdialect "C++20"
		
		--- WINDOWS SPECIFIC DEFINES
		defines { 
			"WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}

	--- CONFIGURATION
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

		--- DEFINES
		defines { "DEBUG" }

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		symbols "On"

		--- DEFINES
		defines { "RELEASE" }

	filter "configurations:Dist"
		runtime "Release"
		optimize "On"
		symbols "Off"

		--- DEFINES
		defines { "DIST" }
