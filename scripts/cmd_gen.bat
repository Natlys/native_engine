@rem ==command_generate/==
@rem ==init==
@set root="../"
@set make="prm.exe"
@set comp="gmake"
@set iput="--file=premake5.lua"
@rem oput="--to=nc_gen.make"
@set oput=
@set args=%comp% %iput% %oput%
@rem ==work==
@pushd %root%
@call %make% %args%
@popd
@rem ==quit==
@rem ==/command_generate==
@pause