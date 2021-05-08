@rem ==native_code_launcher/==
@rem ==init==
@set root="../"
@set path=nc_prj.exe\\bin_cxx
@set exec=nc_prj.exe
@set make="scripts/build_makefile.bat"
@rem ==work==
@pushd %root%
call %path%\\%exec%
@popd
@rem ==quit==
@rem ==/native_code_launcher==
@pause