# C source files
PROJECT_CSRC 	= 	main.c 								\
					Motor.c 								\
					lcd/src/TFT_8080.c					\
					lcd/src/menu.c						\

	
# C++ source files
PROJECT_CPPSRC 	= 	
# Directories to search headers in
PROJECT_INCDIR	=  ./lcd/inc
# Additional libraries
PROJECT_LIBS	=
# Compiler options
PROJECT_OPT     = -specs=nano.specs -specs=nosys.specs

# Additional .mk files are included here
