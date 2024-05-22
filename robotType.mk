###############################################################
###                Robot Type Configuration                 ###
###############################################################
# Author; Anissh G

# This variable is the only thing to modify in this file!!!
# Comp-15 Robot = 0
# Comp-24 Robot = 1
# Skills-15 Robot = 2
# Skills-24 Robot = 3
# NOTE: Changing the robot will auto delete the previous robot's binary files
ROBOT_MODE := 0


###############################################################
####         Do not edit anything below this line          ####

# Location of each robot's working code
DIR_COMP15 := $(SRCDIR)/comp-15
DIR_COMP24 := $(SRCDIR)/comp-24
DIR_SKILLS15 := $(SRCDIR)/skills-15
DIR_SKILLS24 := $(SRCDIR)/skills-24

BIN_COMP15 := $(BINDIR)/comp-15
BIN_COMP24 := $(BINDIR)/comp-24
BIN_SKILLS15 := $(BINDIR)/skills-15
BIN_SKILLS24 := $(BINDIR)/skills-24

# Check if the directory names are same as the binary names
ifneq ($(findstring /comp-15,$(DIR_COMP15)),$(findstring /comp-15,$(BIN_COMP15)))
    $(error Directory names are not same for DIR_COMP15 and BIN_COMP15)
else ifneq ($(findstring /comp-24,$(DIR_COMP24)),$(findstring /comp-24,$(BIN_COMP24)))
    $(error Directory names are not same for DIR_COMP24 and BIN_COMP24)
else ifneq ($(findstring /skills-15,$(DIR_SKILLS15)),$(findstring /skills-15,$(BIN_SKILLS15)))
    $(error Directory names are not same for DIR_SKILLS15 and BIN_SKILLS15)
else ifneq ($(findstring /skills-24,$(DIR_SKILLS24)),$(findstring /skills-24,$(BIN_SKILLS24)))
    $(error Directory names are not same for DIR_SKILLS24 and BIN_SKILLS24)
endif

# Default all dir as excluded instead of all included
EXCLUDED_DIR := $(DIR_COMP15) $(DIR_COMP24) $(DIR_SKILLS15) $(DIR_SKILLS24)
EXCLUDED_BIN := $(BIN_COMP15) $(BIN_COMP24) $(BIN_SKILLS15) $(BIN_SKILLS24)

# Using C++ Flags to allow program to know what robot is being 
#  compiled and run via preprocessor macos. Can be used for other
#  program specific operations.
ROBOTFLAGS := -D_NO_ROBOT_SELECTED # This mode, forces build error

ifeq ($(ROBOT_MODE),0)
    ROBOTFLAGS := -D_COMP_15
    EXCLUDED_DIR := $(DIR_COMP24) $(DIR_SKILLS15) $(DIR_SKILLS24)
	EXCLUDED_BIN := $(BIN_COMP24) $(BIN_SKILLS15) $(BIN_SKILLS24)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'comp-15')
else ifeq ($(ROBOT_MODE),1)
    ROBOTFLAGS := -D_COMP_18
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_SKILLS15) $(DIR_SKILLS24)
    EXCLUDED_BIN := $(BIN_COMP15) $(BIN_SKILLS15) $(BIN_SKILLS24)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'comp-24')
else ifeq ($(ROBOT_MODE),2)
    ROBOTFLAGS := -D_SKILLS_15
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_COMP24) $(DIR_SKILLS24)
    EXCLUDED_BIN := $(BIN_COMP15) $(BIN_COMP24) $(BIN_SKILLS24)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'skills_15')
else ifeq ($(ROBOT_MODE),3)
    ROBOTFLAGS := -D_SKILLS_24
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_COMP24) $(DIR_SKILLS15)
    EXCLUDED_BIN := $(BIN_COMP15) $(BIN_COMP24) $(BIN_SKILLS15)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'skills_24')
else
    $(error Robot_Mode is not specified! Double check ./robotType.mk)
endif

$(info Excluding $(EXCLUDED_DIR) directories)