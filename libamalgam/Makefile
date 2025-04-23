# vim: nospell ft=make foldlevel=99
.EXTRA_PREREQS = Makefile
# Overall convention:
# All source files in the src directory each represent a module in the library.
# And is subject to unit testing. (Many units resides in the same library.
#
# command macros
MKDIR = mkdir -p
CC = gcc

# ---------------------------------------------------------
# libtool
# ---------------------------------------------------------
# !Todo: ifequ origin.
CUR_VER=0:0:0
# (current:revision:age)

# 1. Start with version information of ‘0:0:0’ for each libtool library.

# 2. Update the version information only immediately before a public release of
#   your software. More frequent updates are unnecessary, and only guarantee that
#   the current interface number gets larger faster.

# 3. If the library source code has changed at all since the last update, then
#   increment revision (‘c:r:a’ becomes ‘c:r + 1:a’).

# 4. If any interfaces have been added, removed, or changed since the last update,
#   increment current, and set revision to 0.

# 5. If any interfaces have been added since the last public release, then increment age.

# 6. If any interfaces have been removed or changed since the last public release,
#   then set age to 0
# Don't forget to git-tag when updating the current version.

# Compiler directives, CPPFLAGS is defined in .bashrc
#
ifeq ($(origin BUILD),undefined)
	# https://stackoverflow.com/questions/38801796/how-to-conditionally-set-makefile-variable-to-something-if-it-is-empty
	BUILD := debug
endif
cflags.common := -Wall -Wextra -fPIC
cflags.debug := -g3 -O0  
cflags.sanitize := -g3 -O0 -static-libasan -fsanitize=address,undefined 
cflags.release = -O2 
CFLAGS  := $(cflags.$(BUILD)) $(cflags.common)
CPPFLAGS += -Isrc
RPATH = /usr/local/lib/x86_64-linux-gnu

PATHS = src/
PATHT = test/

SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.c)
INCLUDE_DIR = src

OBJECTS = $(SRC:$(SRC_DIR)/%.c=%.o)
LT_OBJECTS := $(OBJECTS:.o=.lo)
# -----------------------------
LIBRARY = libamalgam.la
# LIBRARY_HEADER = src/strxtra.h
# -----------------------------
# The library needs the .la extension for it to work properly.
# The setup for Unity. ----------------------------------
CLEANUP = rm -f
TARGET_EXTENSION=out
PATHU = ../Unity/
# PATHS = src/
# PATHT = test/
PATHB = build/
PATHO = build/test_objs/
PATHD = build/test_depends/
PATHR = build/test_results/
BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)
SRCT = $(wildcard $(PATHT)*.c)
SRCD := $(SRC:%.c=$(PATHD)/%.d)
LINK = gcc
DEPFLAGS = -MT $@ -MMD -MP -MF $(PATHD)$*.d
COMPILE = gcc -std=c99 $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS)  -c


PASSED = `grep -s PASS $(PATHR)*.txt`
FAIL = `grep -s FAIL $(PATHR)*.txt`
IGNORE = `grep -s IGNORE $(PATHR)*.txt`

RESULTS = $(patsubst $(PATHT)Test%.c,$(PATHR)Test%.txt,$(SRCT) )

# libtool rules
%.lo: $(SRC_DIR)/%.c 
	libtool --mode=compile gcc $(CPPFLAGS) $(CFLAGS) -c $< 

$(SRC_DIR)/%.c: $(INCLUDE_DIR)/%.h
	touch $@

all: $(LIBRARY)

$(LIBRARY): $(LT_OBJECTS) 
	libtool --mode=link gcc $(CPPFLAGS) $(CFLAGS) -o $@ $(LT_OBJECTS) -rpath $(RPATH) -version-info $(CUR_VER) -Wl,-e,mu_main
# I pass arguments here to specificy the entry point. I may need to specify -Wc or -Xcompiler to pass them through

# I pass in the parameters here for the libtool to send into the variable.
#
# I have a special rule for thatt object..
#
# I passs in the address of the loader into that file on a special rules.

licence.lo: src/licence.c | $(PATHB)ld_loader.txt 
	libtool --mode=compile gcc $(CPPFLAGS) $(CFLAGS) -c $< -Wc,-DCUR_VER="\""$(CUR_VER)"\"",-DDL_LOADER="\"$(shell cat build/ld_loader.txt)\""

$(PATHB)ld_loader.txt: $(PATHB)ldloadprobe

$(PATHB)ldloadprobe: env_config/ldloadprobe
	gcc -o $(PATHB)ldloadprobe env_config/ldloadprobe.c
	objcopy --dump-section .interp=/dev/stdout $(PATHB)ldloadprobe >$(PATHB)ld_loader.txt 

install:
	echo "here goes installing."
	echo "including copying out any changed includes"
	sudo libtool --mode=install install -c $(LIBRARY) $(RPATH)/$(LIBRARY)
	sudo ldconfig
	ln -f -s `pwd`/src/*.h $(HOME)/.local/include
	make_source_index	
	@echo "-------- run 	'mmkid' if you need to update the id-database. "

# Unity production rules for automated testing.

test: CPPFLAGS += -I. -I$(PATHU) -I$(PATHS) -DTEST 
test: $(BUILD_PATHS) $(RESULTS)
	@echo $(COMPILE)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"


$(PATHR)%.txt: $(PATHB)%.$(TARGET_EXTENSION) | test_clean
	-./$< > $@ 2>&1

$(PATHB)Test%.$(TARGET_EXTENSION): $(PATHO)Test%.o $(PATHO)%.o 
	$(LINK) -o $@ $^  $(LDLIBS)

# above is where we add any missing libraries

$(PATHO)Test%.o: CPPFLAGS += -I. -I$(PATHU) -I$(PATHS) -DTEST
$(PATHO)Test%.o: $(PATHT)Test%.c
	$(COMPILE)  $< -o $@

$(PATHT)Test%.c:  $(PATHO)%.o
	touch $@

$(PATHO)%.o: CPPFLAGS += -I. -I$(PATHU) -I$(PATHS) -DTEST
$(PATHO)%.o:: $(PATHS)%.c
	$(COMPILE) $< -o $@

$(PATHO)%.o:: $(PATHU)%.c 
	$(COMPILE)  $< -o $@

$(PATHS)%.c: $(PATHU)%.c $(PATHS)%.h
	touch $@

$(PATHU)%.c: $(PATHU)%.h
	touch $@

$(SRCD):

include $(wildcard $(SRCD))

$(PATHB):
	$(MKDIR) $(PATHB)

$(PATHD):
	$(MKDIR) $(PATHD)

$(PATHO):
	$(MKDIR) $(PATHO)

$(PATHR):
	$(MKDIR) $(PATHR)

test_clean:
	$(CLEANUP) $(PATHB)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATHR)*.txt
#	$(CLEANUP) $(PATHO)*.o

clean:
	rm -fr .libs/
	rm -f $(LIBRARY)
	rm -f *.lo
	rm -f *.o

# This is the first command to be executed after the makefile is copied into a new directory.
setup:
	mkdir -p src
	mkdir -p build/test_depends
	mkdir -p build/test_objs
	mkdir -p build/test_results
# for private as we go along tests.
	mkdir -p tests

.PRECIOUS: $(PATHB)Test%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATHO)%.o
.PRECIOUS: $(PATHR)%.txt


