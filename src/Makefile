#
# OMNeT++/OMNEST Makefile for AdaptiveThreshold
#
# This file was generated with the command:
#  opp_makemake -f --deep
#

# Name of target to be created (-o option)
TARGET = AdaptiveThreshold$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -Iadaptivecontrol \
    -Ibackoff \
    -Icontrolapp \
    -Imessages \
    -Inetworks \
    -Irach \
    -Iresults

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/adaptivecontrol/Subsystem.o \
    $O/adaptivecontrol/Server.o \
    $O/backoff/BackoffSubsystem.o \
    $O/backoff/WLANServer.o \
    $O/controlapp/ControlLoop.o \
    $O/rach/BaseClient.o \
    $O/messages/ErrorPkt_m.o

# Message files
MSGFILES = \
    messages/ErrorPkt.msg

# SM files
SMFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f AdaptiveThreshold AdaptiveThreshold.exe libAdaptiveThreshold.so libAdaptiveThreshold.a libAdaptiveThreshold.dll libAdaptiveThreshold.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h ./*_sm.cc ./*_sm.h
	$(Q)-rm -f adaptivecontrol/*_m.cc adaptivecontrol/*_m.h adaptivecontrol/*_sm.cc adaptivecontrol/*_sm.h
	$(Q)-rm -f backoff/*_m.cc backoff/*_m.h backoff/*_sm.cc backoff/*_sm.h
	$(Q)-rm -f controlapp/*_m.cc controlapp/*_m.h controlapp/*_sm.cc controlapp/*_sm.h
	$(Q)-rm -f messages/*_m.cc messages/*_m.h messages/*_sm.cc messages/*_sm.h
	$(Q)-rm -f networks/*_m.cc networks/*_m.h networks/*_sm.cc networks/*_sm.h
	$(Q)-rm -f rach/*_m.cc rach/*_m.h rach/*_sm.cc rach/*_sm.h
	$(Q)-rm -f results/*_m.cc results/*_m.h results/*_sm.cc results/*_sm.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES) $(SM_CC_FILES)  ./*.cc adaptivecontrol/*.cc backoff/*.cc controlapp/*.cc messages/*.cc networks/*.cc rach/*.cc results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/adaptivecontrol/Server.o: adaptivecontrol/Server.cc \
	adaptivecontrol/Server.h \
	messages/ErrorPkt_m.h
$O/adaptivecontrol/Subsystem.o: adaptivecontrol/Subsystem.cc \
	adaptivecontrol/Server.h \
	adaptivecontrol/Subsystem.h \
	controlapp/ControlLoop.h \
	messages/ErrorPkt_m.h
$O/backoff/BackoffSubsystem.o: backoff/BackoffSubsystem.cc \
	adaptivecontrol/Subsystem.h \
	backoff/BackoffSubsystem.h \
	controlapp/ControlLoop.h \
	messages/ErrorPkt_m.h
$O/backoff/WLANServer.o: backoff/WLANServer.cc \
	backoff/WLANServer.h \
	messages/ErrorPkt_m.h
$O/controlapp/ControlLoop.o: controlapp/ControlLoop.cc \
	controlapp/ControlLoop.h
$O/messages/ErrorPkt_m.o: messages/ErrorPkt_m.cc \
	messages/ErrorPkt_m.h
$O/rach/BaseClient.o: rach/BaseClient.cc \
	adaptivecontrol/Server.h \
	adaptivecontrol/Subsystem.h \
	controlapp/ControlLoop.h \
	rach/BaseClient.h

