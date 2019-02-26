#**********************************************************
#
# by jevstein
# 2019/01/27
#
# DEBUG	: DEBUG can be set to YES to include debug info, or NO otherwise(RELEASE)
#		  you can also use like this: make DEBUG:=NO
#
# from : https://blog.csdn.net/yychuyu/article/details/79950414
#**********************************************************

# 1.CONFIG
DEBUG 		= YES
ROOT_DIR	= ../..
PROJ_DIR	= .
OUT_DIR		= $(ROOT_DIR)/bin
SRC 		= $(wildcard *.c)
PROGS 		= $(patsubst %.c, %, $(SRC))
OUT_APPS    = $(patsubst %.c, $(OUT_DIR)/%, $(SRC))
LIBS_DIR	= $(ROOT_DIR)/lib
LIBS		= -ldl -lpthread  				\
			  -L$(LIBS_DIR)/ -lpiece

# 2.TOOL CHAIN
CC 			= gcc
CFLAGS 		= -Wall -g
MKDIR_P     = mkdir -p
CD          = cd
CP          = cp
RM			= rm
INSTALL		= $(CP) -r
EXPORET		= $(CP)

ifeq ($(DEBUG), YES)
    OBJ_DIR	= Debug
else
    OBJ_DIR	= Release
endif

# 3.OUTPUT
all: $(PROGS) install
	$(info makefile all -> all success!)

install: $(OUT_APPS)
	$(info makefile install -> all success)

clean:
	$(RM) -rf $(OBJ_DIR)

cleanall:
	$(RM) -rf $(OBJ_DIR)
	$(RM) -rf $(OUT_DIR)/*

# 4.RULE
$(PROGS) : %:%.c
	@$(MKDIR_P) $(OBJ_DIR) ..
	$(CC) $< $(CFLAGS) -o $(OBJ_DIR)/$@ $(LIBS)

$(OUT_APPS): $(PROGS)
	$(info install $@)
	@$(MKDIR_P) $(OUT_DIR) ..
	@$(CP) $(OBJ_DIR)/$(notdir $@) $@

# .PHONY
.PHONY:clean all
