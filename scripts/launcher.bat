@rem ==native_code_launcher/==
@rem ==init==
@set root="../"
@set path=%1\\bin_cxx
@set exec=%1.exe
@set make="make"
@rem ==work==
@pushd %root%
@call make
@call %path%\\%exec%
@popd
@rem ==quit==
@rem ==/native_code_launcher==
@pause