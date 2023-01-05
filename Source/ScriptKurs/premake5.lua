include "../../Premake/extensions.lua"
include "../../Premake/common.lua"

uppgifter = os.matchdirs("Script-*")

workspace "ScriptKurs"
	architecture "x64"
	location "../../"
	
	startproject (_OPTIONS["defaultproject"])

	configurations {
		"Debug",
		"Release",
		"Retail"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include (dirs.external)
include (dirs.engine)

group ("ScriptKurs") -- creates a filter where all tutorial projects are grouped

for _, v in pairs(uppgifter) do
	include (v)
end
