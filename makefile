#**********************************************************
#
# by jevstein
# 2018/12/14
#
# DEBUG	: DEBUG can be set to YES to include debug info, or NO otherwise(RELEASE)
#		  you can also use like this: make DEBUG:=NO
#**********************************************************

# 1.config
ROOR_DIR = .
SRC_DIRS = src/helper src/net src/client src/server
INSTALL_DIR = $(ROOR_DIR)/bin

# 2.OUTPUT
all: prog install
	$(info all success!)

prog :
	@for dir in $(SRC_DIRS); \
	do \
		cd $$dir; $(MAKE) $(THREADS_FLAG); cd -;\
	done

install :
	# mkdir -p $(INSTALL_DIR) $(INSTALL_DIR)/bin $(INSTALL_DIR)/lib;
	# cp -f $(ROOR_DIR)/bin/* $(INSTALL_DIR)/bin/;
	# cp -f $(ROOR_DIR)/lib/* $(INSTALL_DIR)/lib/;
	$(info install ...)
	
clean :
	@for dir in $(SRC_DIRS); \
	do \
		cd $$dir; $(MAKE) clean; cd -;\
	done

cleanall :
	@for dir in $(SRC_DIRS); \
	do \
		cd $$dir; $(MAKE) cleanall; cd -;\
	done;\
	rm -rf $(INSTALL_DIR)/bin $(INSTALL_DIR)/lib;

# 3.PHONY
.PHONY : all prog install clean cleanall