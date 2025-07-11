--- TOOL INCLUDES
include 'Premake/CSExtensions.lua'
include 'Premake/VSExtensions.lua'
include 'Build-Dependencies.lua'

--- PROJECT CONFIGURATION
workspace 'Acheron Library'
    startproject 'Acheron-Test'
    architecture 'x64'
    location '../'
    configurations { 
        'Debug', 
        'Release', 
        'Dist' 
    }

    --- WINDOWS
    filter 'system:windows'
        --- GLOBAL FLAGS
        flags 'MultiProcessorCompile'
        
        --- WORKSPACE-WIDE BUILD OPTIONS FOR MSVC
        buildoptions { 
            '/EHsc',
            '/Zc:preprocessor', 
            '/Zc:__cplusplus' 
        }

    --- PROJECT
    include 'Build-Acheron.lua'
    include 'Build-Test.lua'
