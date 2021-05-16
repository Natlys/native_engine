@rem ==native_code_generator/==
@rem ==init==
@set root="../"
@set make="../ext\premake\premake5.exe"
@set comp="gmake"
@rem set comp="gmake2";
@rem set comp="vs2019";
@rem ==work==
pushd %root%
call %make% %comp%
popd
@rem ==quit==
@rem ==/native_code_generator==
@pause