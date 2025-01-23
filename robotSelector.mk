###############################################################
###                      Robot Selector                     ###
###############################################################
# Author; Anissh G

# Change the variable "ROBOT_BUILD" to select which robot you are compiling
#  the program for

# Comp-15 Robot = 0
# Comp-24 Robot = 1
# Skills-15 Robot = 2
# Skills-24 Robot = 3
# NOTE: Changing the robot will auto delete the previous robot's binary files
ROBOT_BUILD := 1


###############################################################
####         Do not edit anything below this line          ####

# Location of each robot's working code
DIR_COMP15 := $(SRCDIR)/comp-15
DIR_COMP24 := $(SRCDIR)/comp-24 $(SRCDIR)/opening_may_crash_vscode
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

# Using C++ Flags to allow program to know what robot is being compiled
ROBOTFLAGS := -D_NO_ROBOT_SELECTED # This mode, forces build error

ifeq ($(ROBOT_BUILD),0)
    ROBOTFLAGS := -D_COMP_15
    EXCLUDED_DIR := $(DIR_COMP24) $(DIR_SKILLS15) $(DIR_SKILLS24)
	EXCLUDED_BIN := $(BIN_COMP24) $(BIN_SKILLS15) $(BIN_SKILLS24)
    $(info ROBOT_BUILD is set to $(ROBOT_BUILD) aka 'comp-15')
else ifeq ($(ROBOT_BUILD),1)
    ROBOTFLAGS := -D_COMP_18
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_SKILLS15) $(DIR_SKILLS24)
    EXCLUDED_BIN := $(BIN_COMP15) $(BIN_SKILLS15) $(BIN_SKILLS24)
    $(info ROBOT_BUILD is set to $(ROBOT_BUILD) aka 'comp-24')
else ifeq ($(ROBOT_BUILD),2)
    ROBOTFLAGS := -D_SKILLS_15
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_COMP24) $(DIR_SKILLS24)
    EXCLUDED_BIN := $(BIN_COMP15) $(BIN_COMP24) $(BIN_SKILLS24)
    $(info ROBOT_BUILD is set to $(ROBOT_BUILD) aka 'skills_15')
else ifeq ($(ROBOT_BUILD),3)
    ROBOTFLAGS := -D_SKILLS_24
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_COMP24) $(DIR_SKILLS15)
    EXCLUDED_BIN := $(BIN_COMP15) $(BIN_COMP24) $(BIN_SKILLS15)
    $(info ROBOT_BUILD is set to $(ROBOT_BUILD) aka 'skills_24')
else
    $(error ROBOT_BUILD is not specified! Double check ./robotType.mk)
endif

$(info Excluding $(EXCLUDED_DIR) directories)