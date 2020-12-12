
--
-- If premake command is not supplied an action (target compiler), exit!
--
-- Targets of interest:
--     vs2017     (Visual Studio 2017)
--     vs2019     (Visual Studio 2019)
--
if (_ACTION == nil) then
    return
end

workspace "6502"

   -- destination directory for generated solution/project files
   location ("../" .. _ACTION)

   -- C++ code in all projects
   language "C++"
   cppdialect "C++14"

   --
   -- Build (solution) configuration options:
   --     Release        (Runtime library is Multi-threaded DLL)
   --     Debug          (Runtime library is Multi-threaded Debug DLL)
   --
   configurations { "Release", "Debug" }
   platforms { "Win32", "Win64" }

   -- visual studio options and warnings
   -- /wd4351 (C4351 warning) - disable warning associated with array brace initialization
   -- /wd4996 (C4996 warning) - disable deprecated declarations
   -- /wd4005 (C4005 warning) - disable macro redefinition
   -- /wd4100 (C4100 warning) - disable unreferenced formal parameter
   -- /Oi - generate intrinsic functions
   buildoptions( { "/wd4351", "/wd4996", "/wd4005", "/wd4100", "/Oi" } )

   -- common release configuration flags and symbols
   filter { "configurations:Release" }
      optimize "On"
      -- favor speed over size
      buildoptions { "/Ot" }
      defines { "WIN32", "_LIB", "NDEBUG" }

   -- common debug configuration flags and symbols
   filter { "configurations:Debug" }
      targetsuffix "_d"
      symbols "On"
      defines { "WIN32", "_LIB", "_DEBUG" }

   filter { "platforms:Win32" }
      system "Windows"
      architecture "x32"

   filter { "platforms:Win64" }
      system "Windows"
      architecture "x64"

   -- 6502 core library
   project "cpu"
      kind "StaticLib"    -- creating static libraries
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../cpu/**.**",
      }
      targetname "cpu"

   -- 6502 test
   project "test"
      kind "ConsoleApp"    -- create a test program
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../test/**.**",
      }
      targetname "test"
