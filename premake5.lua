-- premake5.lua
workspace "coldcpu"
    architecture "x64"
    configurations { "Debug", "Release",  }
    startproject "coldemu"

outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "coldemu"
include "coldasm"
include "colddsm"
