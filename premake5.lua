workspace "Anarchy"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

output_dir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

project "Anarchy"
    location "projects"
    kind "WindowedApp"
    language "C++"

    targetdir ("builds/bin/" .. output_dir .. "/%{prj.name}")
    objdir ("builds/obj/" .. output_dir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.c",
        "src/**.hpp",
        "src/**.cpp"
    }

    links {
        "src/engine/raylib35/lib/raylib",
        "winmm"
    }
