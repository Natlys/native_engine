@rem ==native_code_launcher/==
@rem ==init==
@set root="../"
@set path=%1\\bin_cxx
@set exec=%1.exe
@set make="scripts/build_makefile.bat"
@rem ==work==
@pushd %root%
call %path%\\%exec%
@popd
@rem ==quit==
@rem ==/native_code_launcher==
@pause