project "Acheron-Test"
	kind "SharedLib"
	language "C++"

	--- OUTPUT
	location "%{OutputDirs.Solution}"
	targetdir "%{OutputDirs.Bin}/%{cfg.buildcfg}/"
	debugdir "%{OutputDirs.Bin}/%{cfg.buildcfg}/"
	objdir "%{OutputDirs.BinInt}/%{prj.name}-%{cfg.buildcfg}"

	--- GLOBAL INCLUDES
	includedirs "%{IncludeDirs.Acheron}"

	externalincludedirs "%{IncludeDirs.Acheron}"

	--- GLOBAL LINKS
	links "Acheron"

	--- CONFIGURATION
	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "RELEASE" }
		runtime "Release"
		optimize "On"
		symbols "On"

	filter "configurations:Dist"
		defines { "DIST" }
		runtime "Release"
		optimize "On"
		symbols "Off"

	--- WINDOWS
	filter "system:windows"
		systemversion "latest"
		cppdialect "C++20"
		staticruntime "off"
		
		--- DEFINES
		defines { 
			"WINDOWS",
			"_CRT_SECURE_NO_WARNINGS" 
		}

		--- SOURCES FILES
		files {
			"%{IncludeDirs.Test}/Windows/**.h",
			"%{IncludeDirs.Test}/Windows/**.cpp"
		}

	--- LINUX
	filter "system:linux"
		systemversion "latest"
		defines { "LINUX" }

		--- SOURCES FILESs
		files {
			"%{IncludeDirs.Test}/Linux/**.h",
			"%{IncludeDirs.Test}/Linux/**.cpp"
		}
