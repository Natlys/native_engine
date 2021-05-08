@rem ==native_code_generator==
@rem ==init==
@set root="../"
@set make="make"
@rem ==work==
@pushd %root%
call %make%
@popd
@rem ==quit==
@rem ==/native_code_generator==
@pause