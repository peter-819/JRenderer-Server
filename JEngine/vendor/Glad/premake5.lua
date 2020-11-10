project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c",
	}
	
	includedirs
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"

	filter {"configurations:Debug","system:windows"}
		runtime "Debug"
		symbols "on"

	filter {"configurations:Release","system:windows"}
		runtime "Release"
		optimize "on"
