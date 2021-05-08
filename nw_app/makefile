#==project_makefile==#
#
#==recursive_wildcard==#
#$1 = list of directories;
#$2 = list of patterns to match;
#recursive = $(foreach d, $(wildcard $(1:=/*)), $(call recursive, $d, $2) $(filter $(subst *, %, $2), $d))
recursive = $(foreach d, $(wildcard $1*),$(call recursive,$d/,$2) $(filter $(subst *,%,$2),$d))
set_bname = $(addprefix bin_c99/,$(addsuffix .o,$(basename $(notdir $1)) ) )
#src_files := $(call recursive,$(src_direc)/,*.c)
#==config==#
proj_name := nw_app
src_direc := src_c99
src_files := $(wildcard $(src_direc)/*.c) $(wildcard $(src_direc)/*/*.c) $(wildcard $(src_direc)/*/*/*.c)
bin_direc := bin_c99
bin_files := $(call set_bname,$(src_files))
comp_name := F:/dev/gcc/bin/gcc.exe
comp_flag := -Wall
link_name := F:/dev/gcc/bin/gcc.exe
link_flag := -g
proj_file := $(bin_direc)/$(proj_name).exe
#==rules==#
#$(bin_files): $(src_files)
$(bin_direc)/%.o: */%.c
	@echo "[$<]->comp->[$@]"
	$(comp_name) $(comp_flag) -c $< -o $@
$(bin_direc)/%.o: */*/%.c
	@echo "[$<]->comp->[$@]"
	$(comp_name) $(comp_flag) -c $< -o $@
$(proj_file): $(bin_files)
	@echo "[$^]->link->[$@]"
	$(link_name) $(link_flag) -o $@ $^
#==execution==#
script: init work quit
init: $(bin_files)
	@echo "==init_process=="
	@echo "src_files: $(src_files)"
	@echo "bin_files: $(bin_files)"
	@echo "========"
work: $(proj_file)
	@echo "==work_process=="
	@echo "file $(proj_file) is done: let's launch it:"
	$(proj_file)
	@echo "========"
quit:
	@echo "==quit_process=="
#	$(shell del "$(bin_files)")
#	$(shell del "$(proj_file)")
	@echo "========"
#==setting==#
.DEFAULT_GOAL := script