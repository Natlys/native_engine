rem ====
rem generation_has_been_started;
rem ====
@echo off
rem ====
set direct="../"
set cmaker="make"
rem ====
pushd %direct%
call %cmaker%
popd
rem ====
@echo on
rem ====
rem generation_has_been_finished;
rem ====
pause