# User can set VERBOSE variable to have all commands echoed to console for debugging purposes.
ifdef VERBOSE
    Q :=
else
    Q := @
endif

# Useful macros
OBJS = $(addprefix $2/,$(addsuffix .o,$(basename $(wildcard $1/*.cpp $1/*.c $1/*.m))))
MAKEDIR = mkdir -p $(dir $@)
REMOVE = rm
REMOVE_DIR = rm -r -f
QUIET = > /dev/null 2>&1 ; exit 0

# Tool flags
CLANG_FLAGS := -g -Wall -Werror -MMD -MP -O0

# .o object files are to be placed in obj/ directory.
# .a lib files are to be placed in lib/ directory.
# Examples programs will be placed in bin/ directory.
OBJDIR := obj
LIBDIR := lib
BINDIR := bin

# Setup variables to use for building lib/libmip.a
LIBMIP     := lib/libmip.a
LIBMIP_OBJ := $(call OBJS,src,$(OBJDIR))
DEPS := $(patsubst %.o,%.d,$(LIBMIP_OBJ))

# Build each of the examples.
EXAMPLES := $(addprefix $(BINDIR)/,$(notdir $(basename $(wildcard examples/*.ino))))
EXAMPLES_OBJ := $(patsubst $(BINDIR)/%,$(OBJDIR)/examples/%.o,$(EXAMPLES))
DEPS += $(patsubst %.o,%.d,$(EXAMPLES_OBJ))
FRAMEWORKS := -framework Foundation -framework AppKit -framework CoreBluetooth

# Rules
.PHONY : clean all

# Don't delete the intermediate examples/*.o object files.
.SECONDARY : $(EXAMPLES_OBJ)

all : $(LIBMIP) $(EXAMPLES)

$(LIBMIP) : $(LIBMIP_OBJ)
	@echo Building $@
	$Q $(MAKEDIR) $(QUIET)
	$Q ar -rc $@ $?

clean :
	@echo Cleaning libmip
	$Q $(REMOVE_DIR) $(OBJDIR) $(QUIET)
	$Q $(REMOVE_DIR) $(LIBDIR) $(QUIET)
	$Q $(REMOVE_DIR) $(BINDIR) $(QUIET)

# *** Pattern Rules ***
$(OBJDIR)/%.o : %.cpp
	@echo Compiling $<
	$Q $(MAKEDIR) $(QUIET)
	$Q clang $(CLANG_FLAGS) -I src -c $< -o $@

$(OBJDIR)/%.o : %.ino
	@echo Compiling $<
	$Q $(MAKEDIR) $(QUIET)
	$Q clang $(CLANG_FLAGS) -I src -x c++ -c $< -o $@

$(OBJDIR)/%.o : %.c
	@echo Compiling $<
	$Q $(MAKEDIR) $(QUIET)
	$Q clang $(CLANG_FLAGS) -I src -c $< -o $@

$(OBJDIR)/%.o : %.m
	@echo Compiling $<
	$Q $(MAKEDIR) $(QUIET)
	$Q clang $(CLANG_FLAGS) -I src -c $< -o $@

$(BINDIR)/% : $(OBJDIR)/examples/%.o $(LIBMIP)
	@echo Building $@
	$Q $(MAKEDIR) $(QUIET)
	$Q clang $(FRAMEWORKS) $^ -o $@

# *** Pull in header dependencies if not performing a clean build. ***
ifneq "$(findstring clean,$(MAKECMDGOALS))" "clean"
    -include $(DEPS)
endif
