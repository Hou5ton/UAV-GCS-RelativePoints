###########################################################################
#
# APX project makefile rules
#
###########################################################################

# Parameters
APX_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

BUILD_DIR := $(if $(BUILD_DIR),$(BUILD_DIR),$(APX_DIR)/build-apx-release)
CACHE_DIR := $(if $(CACHE_DIR),$(CACHE_DIR),$(APX_DIR)/build-cache)

LIBS_DIST_DIR := /dist

# compiled packages and archives are put here
BUILD_DIR_OUT = $(BUILD_DIR)/packages

# functions
dirname = $(patsubst %/,%,$(dir $(1)))

#detect OS: osx, linux
HOST_OS_LIST = linux osx
ifndef HOST_OS
UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Linux)
HOST_OS = linux
endif
ifeq ($(UNAME_S),Darwin)
HOST_OS = osx
endif
endif
ifeq ($(filter $(HOST_OS),$(HOST_OS_LIST)),)
$(error Bad HOST_OS = $(HOST_OS) [$(HOST_OS_LIST)])
endif


QT_INSTALL_DIR_osx := $(HOME)/QtX
QT_INSTALL_DIR_linux := /opt/Qt
QT_INSTALL_DIR := $(if $(QT_INSTALL_DIR),$(QT_INSTALL_DIR),${QT_INSTALL_DIR_${HOST_OS}})


ARMGCC_INSTALL_DIR_osx := $(HOME)/gcc
ARMGCC_INSTALL_DIR_linux := /opt/gcc
ARMGCC_INSTALL_DIR := $(if $(ARMGCC_INSTALL_DIR),$(ARMGCC_INSTALL_DIR),${ARMGCC_INSTALL_DIR_${HOST_OS}})


TOOLS_DIR := $(realpath $(APX_DIR)/tools)




#######################################################
# detect version
GIT_VERSION_CMD = git describe --tags --match="v*.*" 2> /dev/null |sed 's/^v//;s/\-/\./;s/\(.*\)-\(.*\)/\1/'
GIT_VERSION = $(shell $(GIT_VERSION_CMD))
GIT_BRANCH = $(shell git rev-parse --abbrev-ref HEAD)

version: FORCE
	@echo "Current version: $(shell $(GIT_VERSION_CMD))"

branch: FORCE
	@echo "Current branch: $(GIT_BRANCH)"


#######################################################
# detect Qt installation (QTDIR)
ifndef QTDIR
QT_INSTALL_DIR_PATH = $(shell cat $(QT_INSTALL_DIR)/path_qt)
QMAKE_BIN = $(shell echo "$(if $(QT_INSTALL_DIR_PATH),$(QT_INSTALL_DIR_PATH)/qmake,$(shell which qmake))" | sed 's: :\\ :g')
else
QMAKE_BIN = $(QTDIR)/bin/qmake
endif
QMAKE = $(if $(wildcard $(QMAKE_BIN)),$(QMAKE_BIN),$(error Error: Qt not found in QTDIR: $(QTDIR)))

qt-version: FORCE
	@echo $(shell "$(QMAKE)" -v)


#######################################################
# detect QBS installation (QBSDIR)
ifndef QBSDIR
ifeq ($(HOST_OS),linux)
QT_TOOLS_DIR := $(QT_INSTALL_DIR)/Tools/QtCreator/bin
endif
ifeq ($(HOST_OS),osx)
QT_TOOLS_DIR := $(QT_INSTALL_DIR)/Qt Creator.app/Contents/MacOS
endif
QBS_BIN_1 = $(shell echo "$(if $(QT_TOOLS_DIR),$(QT_TOOLS_DIR)/qbs,$(shell which qbs))" | sed 's: :\\ :g')
QBS_BIN_2 = $(shell which qbs | sed 's: :\\ :g')
QBS_BIN = $(if $(shell which $(QBS_BIN_1)),$(QBS_BIN_1),$(QBS_BIN_2))
else
QBS_BIN = $(QBSDIR)/qbs
endif
QBS = $(if $(wildcard $(QBS_BIN)),$(QBS_BIN),$(error Error: Qbs not found in QBSDIR: $(QBSDIR)))

qbs-version: FORCE
	@echo "QBS version $(shell $(QBS) --version) in $(QBS)"


#######################################################
# detect ARMGCC installation (PATH)
ARMGCC_INSTALL_DIR_PATH = $(shell cat $(ARMGCC_INSTALL_DIR)/path_armgcc)
ARMGCC_BIN = $(shell echo "$(if $(ARMGCC_INSTALL_DIR_PATH),$(ARMGCC_INSTALL_DIR_PATH)/arm-none-eabi-gcc,$(shell which arm-none-eabi-gcc))" | sed 's: :\\ :g')
ARMGCC_DIR = $(call dirname,$(ARMGCC_BIN))

armgcc-version: FORCE
	@echo "ARM GCC from $(ARMGCC_DIR)"
	@$(ARMGCC_BIN) --version









#######################################################
FORCE:

.PHONY: FORCE

