--Configuration
if(_ACTION == "vs2017") then
local windowsPlatform = string.gsub(os.getenv("WindowsSDKVersion") or "10.0.16299.0", "\\", "")
	if _OPTIONS["with-windows"] then
		windowsPlatform = _OPTIONS["with-windows"]
	end
local action = premake.action.current()
  action.vstudio.windowsTargetPlatformVersion    = windowsPlatform
end
--SOLUTION----------------------------------------------------------------------
solution "TraceSystem"
    configurations {"Debug", "Release"}
    location "build"
    
    debugdir("exe")
    targetdir("exe")
    
    configuration "Debug"
        flags {"Symbols"}
        defines {"NDEBUG"}
    configuration "Release"
        flags {"OptimizeSize"}
        
    configuration{}
    --DEFINES
    configuration "windows"
    targetsuffix "_d"
      defines {
        "WIN32",
        "_WINDOWS"
      }
    configuration "linux"
      defines {

      }
    configuration {}
    --INCLUDE DIRS------
    includedirs {
        path.join(".")
    }

    project "Test1"
      kind "ConsoleApp"
      language "C++"
      files {
        "tests/test_1.cpp"
      }

    project "Test2_Multithread"
      kind "ConsoleApp"
      language "C++"
      files {
        "tests/test_2_multithread.cpp"
      }