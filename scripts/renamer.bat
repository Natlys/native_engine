@rem ==renamer==
@rem ==init==
@set root="../"
@set iput="nw_"
@set oput="nc_"
@set comd="powershell get-childitem %iput% | foreach { }"
@rem ==work==
@pushd %root%
powershell $files=get-childitem %iput% -recurse
powershell foreach ($file in $files) {
    if ($file.name -ne $file.name.tolower()) {
        rename-item -newname { $_.name.tolower() }
    }
}
powershell get-childitem %iput% | foreach { rename-item $_ $_.name.replace(%iput%, %oput%) }
@popd
@rem ==quit==
@pause