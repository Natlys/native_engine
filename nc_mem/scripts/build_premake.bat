rem project_generation_is_started
@echo off
@set cmaker="..\ext\premake\premake5.exe"
@set config="premake5.lua"
@set target="gmake"
@rem set target="gmake2";
@rem set target="vs2019";
@pushd "../"
call %cmaker% %target%
@popd
@echo on
@rem project_generation_is_finished
pause