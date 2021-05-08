#=project=#
proj_name = native_world
#==c98==#
#==cpp==#
#==lua==#
#==execution==#
script: init work quit
init: nw_app/makefile
	@echo "==init_process=="
	@echo "========"
work:
	@echo "==work_process=="
	@echo "========"
quit:
	@echo "==quit_process=="
	@echo "========"
#==setting==#
.DEFAULT_GOAL = script
.VPATH = ${c98_src_direc}