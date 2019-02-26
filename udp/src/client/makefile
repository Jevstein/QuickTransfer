#**********************************************************
#
# by jevstein
# 2018/12/11
#
# DEBUG	: DEBUG can be set to YES to include debug info, or NO otherwise(RELEASE)
#		  you can also use like this: make DEBUG:=NO
#**********************************************************

# 1.CONFIG
DEBUG 		:= YES
APP			:= client
APP_OUT		:= client
ROOT_DIR	:= ../..
PROJ_DIR	:= ..
OUT_DIR		:= $(ROOT_DIR)/bin
DEFS_DIR	:= 
INCS_DIR	:= -I$(ROOT_DIR)/inc
LIBS_DIR	:= $(ROOT_DIR)/lib
LIBS		:= -ldl -lpthread \
			   -L$(LIBS_DIR)/ -lpiece
# VPATH       := $(PROJ_DIR)/common
RAW_SRCS	:= $(wildcard *.c) $(wildcard *.cpp)
			   #$(wildcard $(PROJ_DIR)/common/*.cpp) $(wildcard $(PROJ_DIR)/common/*.c)

# 2.TOOL CHAIN
CXX				:= g++
COMMON_FLAGS	:= -g -std=c++11
CXX_FLAGS		:= $(COMMON_FLAGS)
MKDIR_P     	:= mkdir -p
CP          	:= cp
RM				:= rm
INSTALL			:= $(CP)
EXPORET			:= $(CP)

ifeq ($(DEBUG), YES)
    OBJ_DIR		:= Debug
else
    OBJ_DIR		:= Release
endif

# 3.FILES
SRCS    := $(notdir $(RAW_SRCS))
OBJS	:= $(addsuffix .o, $(basename $(SRCS)))
OBJS	:= $(patsubst %.o, $(OBJ_DIR)/%.o, $(OBJS))

# 4.OUTPUT
all: prog install
	$(info $(APP_OUT)\makefile -> all success!)

prog: $(OBJ_DIR)/$(APP_OUT)
	$(info $(APP_OUT)\makefile -> $(OBJ_DIR)/$(APP_OUT))

install: $(OUT_DIR)/$(APP_OUT)
	$(info $(APP)\makefile -> $(OUT_DIR)/$(APP_OUT))

clean:
	$(RM) -rf $(OBJ_DIR)
	$(info $(APP)\makefile -> clean success!)

cleaninstall:
	$(RM) $(OUT_DIR)/$(APP_OUT)
	$(info $(APP)\makefile -> clean install success!)

cleanall: clean cleaninstall
	$(info $(APP)\makefile -> clean all success!)

# 5.COMMON RULE
$(OBJ_DIR)/%.o : %.cpp
	$(info creating $@ ...)
	@$(MKDIR_P) $(OBJ_DIR) ..
	$(CXX) -c $(CXXFLAGS) $(INCS_DIR) $< -o $@

$(OBJ_DIR)/%.o : %.c
	$(info creating $@ ...)
	@$(MKDIR_P) $(OBJ_DIR) ..
	$(CXX) -c $(CXXFLAGS) $(INCS_DIR) $< -o $@

$(OBJ_DIR)/$(APP_OUT): $(OBJS)
	$(info linking ...)
	$(CXX) $(CXX_FLAGS) -o $(OBJ_DIR)/$(APP_OUT) $(OBJS) $(LIBS)

$(OUT_DIR)/$(APP_OUT): $(OBJ_DIR)/$(APP_OUT)
	$(info installing ...)
	@$(MKDIR_P) $(OUT_DIR) ..
	@$(CP) $< $@

#include $(subst .cpp,.d,$(SRCS))

# 6.PHONY
.PHONY: all prog install clean cleaninstall cleanall