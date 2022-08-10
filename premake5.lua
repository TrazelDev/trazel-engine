workspace "trazel_engine"
    architecture "x64"

    configurations { 
        "Debug",            -- this the configuration that I devlope in it
        "Release",          -- this is the configuration that I run the code to be fast
        "Client"            -- this is the final prduct that is going to be created
    }

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "trazel_engine"
    location "trazel_engine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "trazel_engine/src/pch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/trazel_engine/**.cpp",
        "%{prj.name}/src/trazel_engine/window/**.cpp",
        "%{prj.name}/src/trazel_engine/window/mac_linux**.cpp",
        "%{prj.name}/src/trazel_engine/window/windows**.cpp"
    }

    includedirs {
        "%{prj.name}/3d_party_extentions",
        "%{prj.name}/3d_party_extentions/spdlog/include",
        "%{prj.name}/3d_party_extentions/spdlog",
        "%{prj.name}/3d_party_extentions/glfw",
        "%{prj.name}/3d_party_extentions/glfw/include/GLFW",
        "%{prj.name}/3d_party_extentions/vulkan_sdk/Include",
        "%{prj.name}/3d_party_extentions/imgui",
        "%{prj.name}/src",
        "imGui",
        "imGui/backends",
        "{prj.name}/3d_party_extentions/glm-master/glm/detail",
        "{prj.name}/3d_party_extentions/glm-master/glm"
    }

    links {
        "imGui"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "off"
        runtime "Debug"
        systemversion "latest"

        defines {
            "TZE_PLATFORM_WINDOWS",
            "TZE_BUILD_DLL",
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. output_dir .. "/client")
        }

    filter "configurations:Debug"
        defines "DEBUG_MODE"
        symbols "On"
    
    filter "configurations:Release"
        defines "REALSE_MODE"
        optimize "On"

    filter "configurations:Client"
        defines "Client_MODE"
        optimize "On"


project "client"
    location "client"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"
    runtime "Debug"
    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "trazel_engine/3d_party_extentions",
        "trazel_engine/3d_party_extentions/spdlog/include",
        "trazel_engine/3d_party_extentions/spdlog",
        "trazel_engine/3d_party_extentions/glfw",
        "trazel_engine/3d_party_extentions/glfw/include/GLFW",
        "trazel_engine/3d_party_extentions/vulkan_sdk/Include",
        "trazel_engine/src",
        "imGui",
        "imGui/backends",
        "trazel_engine/3d_party_extentions/glm-master/glm/detail"
    }

    links {
        "trazel_engine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "TZE_PLATFORM_WINDOWS"
        }

        

    filter "configurations:Debug"
        defines "DEBUG_MODE"
        symbols "On"
    
    filter "configurations:Release"
        defines "REALSE_MODE"
        optimize "On"

    filter "configurations:Client"
        kind "WindowedApp"
        defines "Client_MODE"
        symbols "On"

project "imGui"
    location "trazel_engine"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    files {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    removefiles {"%{prj.name}/misc/freetype/imgui_freetype.cpp", "%{prj.name}/misc/freetype/imgui_freetype.h"}

    includedirs {
        "%{prj.name}",
        "%{prj.name}/backends",
        "trazel_engine/3d_party_extentions",
        "trazel_engine/3d_party_extentions/spdlog/include",
        "trazel_engine/3d_party_extentions/spdlog",
        "trazel_engine/3d_party_extentions/glfw",
        "trazel_engine/3d_party_extentions/glfw/include",
        "trazel_engine/3d_party_extentions/glfw/include/GLFW",
        "trazel_engine/3d_party_extentions/vulkan_sdk/Include",
        "trazel_engine/3d_party_extentions/imgui",
        "trazel_engine/src",
        "trazel_engine/3d_party_extentions/glm-master/glm/detail",
        "trazel_engine/3d_party_extentions/glm-master/glm"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "off"
        runtime "Debug"
        systemversion "latest"

        defines {
            "TZE_PLATFORM_WINDOWS",
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. output_dir .. "/client")
        }

    filter "configurations:Debug"
        defines "DEBUG_MODE"
        symbols "On"
    
    filter "configurations:Release"
        defines "DEBUG_MODE"
        defines "REALSE_MODE"
        optimize "On"

    filter "configurations:Client"
        defines "Client_MODE"
        optimize "On"