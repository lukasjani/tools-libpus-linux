#Generated by VisualGDB project wizard. 
#Note: VisualGDB will automatically update this file when you add new sources to the project.
#All other changes you make in this file will be preserved.
#Visit http://visualgdb.com/makefiles for more details

#VisualGDB: AutoSourceFiles		#<--- remove this line to disable auto-updating of SOURCEFILES and EXTERNAL_LIBS

TARGETNAME := tools-libpus-linux
#TARGETTYPE can be APP, STATIC or SHARED
TARGETTYPE := APP

to_lowercase = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

macros-to-preprocessor-directives = $(foreach __macro, $(strip $1), \
	$(if $(findstring =,$(__macro)), \
		--pd "$(firstword $(subst =, ,$(__macro))) SETA $(subst ",\",$(lastword $(subst =, ,$(__macro))))", \
		--pd "$(__macro) SETA 1")) \

CONFIG ?= DEBUG

CONFIGURATION_FLAGS_FILE := $(call to_lowercase,$(CONFIG)).mak
TOOLCHAIN_ROOT ?= $(toolchain_root)

include $(CONFIGURATION_FLAGS_FILE)

#LINKER_SCRIPT defined inside the configuration file (e.g. debug.mak) should override any linker scripts defined in shared .mak files
CONFIGURATION_LINKER_SCRIPT := $(LINKER_SCRIPT)

include $(ADDITIONAL_MAKE_FILES)

ifneq ($(CONFIGURATION_LINKER_SCRIPT),)
LINKER_SCRIPT := $(CONFIGURATION_LINKER_SCRIPT)
endif

ifneq ($(LINKER_SCRIPT),)
LDFLAGS += -T$(LINKER_SCRIPT)
endif

ifeq ($(AS),)
AS := as
endif

ifeq ($(AS),as)
AS := $(CC)
ASFLAGS := $(CFLAGS) $(ASFLAGS)
endif

ifeq ($(TOOLCHAIN_SUBTYPE),armcc)
	CC_DEPENDENCY_FILE_SPECIFIER := --depend
	ASM_DEPENDENCY_FILE_SPECIFIER := --depend
	USING_KEIL_TOOLCHAIN := 1
else ifeq ($(TOOLCHAIN_SUBTYPE),armclang)
	CC_DEPENDENCY_FILE_SPECIFIER := -MD -MF
	ASM_DEPENDENCY_FILE_SPECIFIER := --depend
	USING_KEIL_TOOLCHAIN := 1
else
	CC_DEPENDENCY_FILE_SPECIFIER := -MD -MF
	ASM_DEPENDENCY_FILE_SPECIFIER := -MD -MF
endif


ifeq ($(BINARYDIR),)
error:
	$(error Invalid configuration, please check your inputs)
endif

SOURCEFILES := src/dataview-uniq.c src/pus_apid.c src/pus_error.c src/pus_events.c src/pus_event_action.c src/pus_file_management_linux.c src/pus_housekeeping.c src/pus_notify.c src/pus_packet.c src/pus_packet_queues.c src/pus_packet_reduced.c src/pus_parameter_management.c src/pus_parameter_monitoring.c src/pus_st01_packets.c src/pus_st03_packets.c src/pus_st05_packets.c src/pus_st08_packets.c src/pus_st09_packets.c src/pus_st11_packets.c src/pus_st12_packets.c src/pus_st13_packets.c src/pus_st17_packets.c src/pus_st18_packets.c src/pus_st19_packets.c src/pus_st20_packets.c src/pus_st23_packets.c src/pus_stored_param.c src/pus_test.cpp src/pus_threads.c src/pus_time.c src/pus_timebased_scheduling.c src/pus_encoder.cpp tools-libpus-linux.cpp
EXTERNAL_LIBS := 
EXTERNAL_LIBS_COPIED := $(foreach lib, $(EXTERNAL_LIBS),$(BINARYDIR)/$(notdir $(lib)))

CFLAGS += $(COMMONFLAGS)
CXXFLAGS += $(COMMONFLAGS)
ASFLAGS += $(COMMONFLAGS)
LDFLAGS += $(COMMONFLAGS)

CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))
CXXFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

CFLAGS += $(addprefix -D,$(PREPROCESSOR_MACROS))
CXXFLAGS += $(addprefix -D,$(PREPROCESSOR_MACROS))

ifeq ($(USING_KEIL_TOOLCHAIN),1)
	ASFLAGS +=$(call macros-to-preprocesor-directives, $(PREPROCESSOR_MACROS))
else
	ASFLAGS += $(addprefix -D,$(PREPROCESSOR_MACROS))
endif

CXXFLAGS += $(addprefix -framework ,$(MACOS_FRAMEWORKS))
CFLAGS += $(addprefix -framework ,$(MACOS_FRAMEWORKS))
LDFLAGS += $(addprefix -framework ,$(MACOS_FRAMEWORKS))

LDFLAGS += $(addprefix -L,$(LIBRARY_DIRS))

ifeq ($(GENERATE_MAP_FILE),1)
LDFLAGS += -Wl,-Map=$(BINARYDIR)/$(basename $(TARGETNAME)).map
endif

LIBRARY_LDFLAGS = $(addprefix -l,$(LIBRARY_NAMES))

ifeq ($(IS_LINUX_PROJECT),1)
	RPATH_PREFIX := -Wl,--rpath='$$ORIGIN/../
	LIBRARY_LDFLAGS += $(EXTERNAL_LIBS)
	LIBRARY_LDFLAGS += -Wl,--rpath='$$ORIGIN'
	LIBRARY_LDFLAGS += $(addsuffix ',$(addprefix $(RPATH_PREFIX),$(dir $(EXTERNAL_LIBS))))
	
	ifeq ($(TARGETTYPE),SHARED)
		CFLAGS += -fPIC
		CXXFLAGS += -fPIC
		ASFLAGS += -fPIC
		LIBRARY_LDFLAGS += -Wl,-soname,$(TARGETNAME)
	endif
	
	ifneq ($(LINUX_PACKAGES),)
		PACKAGE_CFLAGS := $(foreach pkg,$(LINUX_PACKAGES),$(shell pkg-config --cflags $(pkg)))
		PACKAGE_LDFLAGS := $(foreach pkg,$(LINUX_PACKAGES),$(shell pkg-config --libs $(pkg)))
		CFLAGS += $(PACKAGE_CFLAGS)
		CXXFLAGS += $(PACKAGE_CFLAGS)
		LIBRARY_LDFLAGS += $(PACKAGE_LDFLAGS)
	endif	
else
	LIBRARY_LDFLAGS += $(EXTERNAL_LIBS)
endif

LIBRARY_LDFLAGS += $(ADDITIONAL_LINKER_INPUTS)

all_make_files := $(firstword $(MAKEFILE_LIST)) $(CONFIGURATION_FLAGS_FILE) $(ADDITIONAL_MAKE_FILES)

ifeq ($(STARTUPFILES),)
	all_source_files := $(SOURCEFILES)
else
	all_source_files := $(STARTUPFILES) $(filter-out $(STARTUPFILES),$(SOURCEFILES))
endif

source_obj1 := $(all_source_files:.cpp=.o)
source_obj2 := $(source_obj1:.c=.o)
source_obj3 := $(source_obj2:.s=.o)
source_obj4 := $(source_obj3:.S=.o)
source_obj5 := $(source_obj4:.cc=.o)
source_objs := $(source_obj5:.cxx=.o)

all_objs := $(addprefix $(BINARYDIR)/, $(notdir $(source_objs)))

PRIMARY_OUTPUTS :=

ifeq ($(GENERATE_BIN_FILE),1)
PRIMARY_OUTPUTS += $(BINARYDIR)/$(basename $(TARGETNAME)).bin
endif

ifeq ($(GENERATE_IHEX_FILE),1)
PRIMARY_OUTPUTS += $(BINARYDIR)/$(basename $(TARGETNAME)).ihex
endif

ifeq ($(PRIMARY_OUTPUTS),)
PRIMARY_OUTPUTS := $(BINARYDIR)/$(TARGETNAME)
endif

all: $(PRIMARY_OUTPUTS)

ifeq ($(USING_KEIL_TOOLCHAIN),1)

$(BINARYDIR)/$(basename $(TARGETNAME)).bin: $(BINARYDIR)/$(TARGETNAME)
	$(OBJCOPY) --bin $< --output $@

$(BINARYDIR)/$(basename $(TARGETNAME)).ihex: $(BINARYDIR)/$(TARGETNAME)
	$(OBJCOPY) --i32 $< --output $@

else

$(BINARYDIR)/$(basename $(TARGETNAME)).bin: $(BINARYDIR)/$(TARGETNAME)
	$(OBJCOPY) -O binary $< $@

$(BINARYDIR)/$(basename $(TARGETNAME)).ihex: $(BINARYDIR)/$(TARGETNAME)
	$(OBJCOPY) -O ihex $< $@

endif
	
EXTRA_DEPENDENCIES :=

ifneq ($(LINKER_SCRIPT),)
EXTRA_DEPENDENCIES += $(LINKER_SCRIPT)
endif

ifeq ($(TARGETTYPE),APP)
$(BINARYDIR)/$(TARGETNAME): $(all_objs) $(EXTERNAL_LIBS) $(EXTRA_DEPENDENCIES)
	$(LD) -o $@ $(LDFLAGS) $(START_GROUP) $(all_objs) $(LIBRARY_LDFLAGS) $(END_GROUP)
endif

ifeq ($(TARGETTYPE),SHARED)
$(BINARYDIR)/$(TARGETNAME): $(all_objs) $(EXTERNAL_LIBS) $(EXTRA_DEPENDENCIES)
	$(LD) -shared -o $@ $(LDFLAGS) $(START_GROUP) $(all_objs) $(LIBRARY_LDFLAGS) $(END_GROUP)
endif
	
ifeq ($(TARGETTYPE),STATIC)
$(BINARYDIR)/$(TARGETNAME): $(all_objs) $(EXTRA_DEPENDENCIES)
	$(AR) -r $@ $^
endif

-include $(all_objs:.o=.dep)

clean:
ifeq ($(USE_DEL_TO_CLEAN),1)
	cmd /C del /S /Q $(BINARYDIR)
else
	rm -rf $(BINARYDIR)
endif

$(BINARYDIR):
	mkdir $(BINARYDIR)

#VisualGDB: FileSpecificTemplates		#<--- VisualGDB will use the following lines to define rules for source files in subdirectories
$(BINARYDIR)/%.o : %.cpp $(all_make_files) |$(BINARYDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)

$(BINARYDIR)/%.o : %.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)

$(BINARYDIR)/%.o : %.S $(all_make_files) |$(BINARYDIR)
	$(AS) $(ASFLAGS) -c $< -o $@ $(ASM_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)

$(BINARYDIR)/%.o : %.s $(all_make_files) |$(BINARYDIR)
	$(AS) $(ASFLAGS) -c $< -o $@ $(ASM_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)

$(BINARYDIR)/%.o : %.cc $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)

$(BINARYDIR)/%.o : %.cxx $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER)F $(@:.o=.dep)

#VisualGDB: GeneratedRules				#<--- All lines below are auto-generated. Remove this line to suppress auto-generation of file rules.


$(BINARYDIR)/dataview-uniq.o : src/dataview-uniq.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_apid.o : src/pus_apid.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_error.o : src/pus_error.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_events.o : src/pus_events.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_event_action.o : src/pus_event_action.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_file_management_linux.o : src/pus_file_management_linux.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_housekeeping.o : src/pus_housekeeping.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_notify.o : src/pus_notify.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_packet.o : src/pus_packet.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_packet_queues.o : src/pus_packet_queues.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_packet_reduced.o : src/pus_packet_reduced.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_parameter_management.o : src/pus_parameter_management.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_parameter_monitoring.o : src/pus_parameter_monitoring.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st01_packets.o : src/pus_st01_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st03_packets.o : src/pus_st03_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st05_packets.o : src/pus_st05_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st08_packets.o : src/pus_st08_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st09_packets.o : src/pus_st09_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st11_packets.o : src/pus_st11_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st12_packets.o : src/pus_st12_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st13_packets.o : src/pus_st13_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st17_packets.o : src/pus_st17_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st18_packets.o : src/pus_st18_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st19_packets.o : src/pus_st19_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st20_packets.o : src/pus_st20_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_st23_packets.o : src/pus_st23_packets.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_stored_param.o : src/pus_stored_param.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_test.o : src/pus_test.cpp $(all_make_files) |$(BINARYDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_threads.o : src/pus_threads.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_time.o : src/pus_time.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_timebased_scheduling.o : src/pus_timebased_scheduling.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)


$(BINARYDIR)/pus_encoder.o : src/pus_encoder.cpp $(all_make_files) |$(BINARYDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(CC_DEPENDENCY_FILE_SPECIFIER) $(@:.o=.dep)

