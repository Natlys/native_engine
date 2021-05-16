@rem ==native_code_deleter==
@rem ==init==
@set sub="nothing"
@rem ==work==
@call git submodule deinit %sub%
@call git rm %sub%
@call git commit-m "%sub% has been removed"
@call del -rf .git/modules/%sub%
@rem ==quit==