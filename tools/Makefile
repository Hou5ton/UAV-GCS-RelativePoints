###########################################################################
#
# APX makefile for tools installation
#
###########################################################################
install-tools-gcs: install-tools install-qbs install-qt install-libs

install-tools-fw: install-tools install-qbs install-armgcc

install-tools-release: install-tools-python install-tools-python-deploy install-tools-python-release

install-tools-docs: install-tools-docs-release

include ../Rules.mk

FRAMEWORKS_DIR := $(if $(FRAMEWORKS_DIR),$(FRAMEWORKS_DIR),/Library/Frameworks)

VERSION_QT = 5.14.0
VERSION_QTCREATOR = 4.11.0

VERSION_GST = 1.16.2
VERSION_SPARKLE = 1.22.0
VERSION_SDL2 = 2.0.10

LINUXDEPLOYQT_SRC = https://github.com/probonopd/linuxdeployqt/releases/download/6/linuxdeployqt-6-x86_64.AppImage
APPIMAGETOOL_SRC = https://github.com/AppImage/AppImageKit/releases/download/12/appimagetool-x86_64.AppImage
ARMGCC_SRC = https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/RC2.1/gcc-arm-none-eabi-9-2019-q4-major-${ARMGCC_${HOST_OS}}.tar.bz2

QT_PACKAGES = qtbase qtdeclarative qttools \
				quickcontrols2 serialport multimedia speech svg location \
				graphicaleffects qtcharts

QT_PACKAGES_linux = icu

APT_PACKAGES = build-essential curl git \
					make pkg-config python-pip p7zip-full \
					nano gpg fuse file axel libgl1-mesa-glx \
					automake cmake wget xxd desktop-file-utils

APT_DIST_PACKAGES = libsdl2-2.0-0 libsndio6.1

WGET := axel
PIP := sudo pip install
PIP_user := pip install --user

ARMGCC_linux := x86_64-linux
ARMGCC_osx := mac

ARMGCC_PKG = $(notdir $(ARMGCC_SRC))
ARMGCC_NAME = $(word 1,$(subst ., ,$(ARMGCC_PKG)))

test:
	@echo "Success"

#######################################################
# install command line tools
install-tools: install-tools-$(HOST_OS) install-tools-python install-tools-python-deploy

install-tools-osx: FORCE
	@brew install p7zip axel
	@$(PIP) dmgbuild

install-tools-linux: FORCE
	@sudo apt-get update
	@sudo apt-get install -y --no-install-recommends $(APT_PACKAGES)
	@$(PIP) setuptools
	@$(PIP) wheel

install-tools-python:
	@$(PIP) setuptools
	@$(PIP) wheel
	@$(PIP) requests
	@$(PIP) simplejson pyyaml jinja2

install-tools-python-deploy:
	@$(PIP) GitPython githubrelease

install-tools-python-release:
	@$(PIP) mailjet_rest GitPython

install-tools-docs-release:
	@sudo apt-get install -y --no-install-recommends python-pip
	@$(PIP) setuptools wheel
	@$(PIP) pyparsing pyyaml

#######################################################
# install libraries
install-libs: install-libs-$(HOST_OS)

install-libs-osx: install-sparkle-osx install-sdl-osx install-gst-osx 

install-libs-linux: install-gcc8-ubuntu install-appimageupdate install-dist-linux
	@sudo apt-get install -y --no-install-recommends libz-dev libsdl2-dev zsync
	@sudo apt-get install -y --no-install-recommends \
		libgstreamer1.0-0 \
		gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
		gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly \
		gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools
	@sudo apt-get install -y --no-install-recommends \
		libgstreamer1.0-dev \
		libgstreamer-plugins-base1.0-dev \
		gstreamer1.0-x gstreamer1.0-gl gstreamer1.0-libav gstreamer1.0-pulseaudio libgstreamer-plugins-base1.0-dev \
		libegl1-mesa-dev libgles2-mesa-dev libxkbcommon-x11-dev libspeechd-dev libffi-dev
	
	
install-appimageupdate: install-appimageupdate-libs
	@sudo rm -rf /usr/src/AppImageUpdate
	@sudo git clone https://github.com/AppImage/AppImageUpdate.git /usr/src/AppImageUpdate
	@cd /usr/src/AppImageUpdate && sudo git submodule update --init --recursive --reference .
	@sudo mkdir /usr/src/AppImageUpdate/build
	@cd /usr/src/AppImageUpdate/build && sudo cmake .. && sudo make
	@sudo cp /usr/src/AppImageUpdate/build/src/libappimageupdate.a /usr/local/lib
	@sudo cp /usr/src/AppImageUpdate/build/lib/libappimage/src/libappimage_shared/libappimage_shared.a /usr/local/lib
	@sudo cp /usr/src/AppImageUpdate/build/lib/libcpr.a /usr/local/lib
	@sudo cp /usr/src/AppImageUpdate/build/lib/zsync2/lib/libzsync/libzsync.a /usr/local/lib
	@sudo cp /usr/src/AppImageUpdate/build/lib/zsync2/src/libzsync2.a /usr/local/lib
	@sudo cp /usr/src/AppImageUpdate/build/lib/zsync2/lib/librcksum/librcksum.a /usr/local/lib
	@sudo cp -r /usr/src/AppImageUpdate/include/appimage /usr/local/include
	
install-appimageupdate-libs:
	@sudo apt-get install -y --no-install-recommends libcurl4-gnutls-dev libssl-dev libtool libglib2.0-dev libcairo-dev
	
install-gcc8-debian:
	@echo 'deb http://ftp.debian.org/debian buster main' > /etc/apt/sources.list.d/debian-buster.list
	@sudo apt-get update
	@sudo apt-get -t buster install -y --no-install-recommends g++-8 g++-8-multilib
	@sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 50
	@sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 50
	@cd /usr/include; sudo ln -sf asm-generic/ asm

install-gcc8-ubuntu:
	@sudo apt-get install -y --no-install-recommends software-properties-common
	@sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
	@sudo apt-get update
	@sudo apt-get install -y --no-install-recommends g++-8 g++-8-multilib
	@sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 50
	@sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 50
	@cd /usr/include; sudo ln -sf asm-generic/ asm


#######################################################
# dist packages to add to bundle

install-dist-linux:
	@for d in $(APT_DIST_LIST); do echo $$d; done
	@sudo rm -rf $(LIBS_DIST_DIR) && sudo mkdir -p $(LIBS_DIST_DIR)
	@cd $(LIBS_DIST_DIR) && sudo apt-get download $(APT_DIST_PACKAGES)


#######################################################
# GCC ARM toolchain
install-armgcc: $(ARMGCC_INSTALL_DIR)/$(ARMGCC_NAME)

$(ARMGCC_INSTALL_DIR)/$(ARMGCC_NAME): $(CACHE_DIR)/$(ARMGCC_PKG)
	@echo "Installing GCC ARM toolchain..."
	@rm -rf $@
	@mkdir -p $@
	@echo "Extracting $(ARMGCC_PKG)..."
	@cd $@; \
		bunzip2 -k -c $< | tar --strip-components 1 -xopf -;
	@$@/bin/arm-none-eabi-gcc --version
	@echo "$@/bin" > $(ARMGCC_INSTALL_DIR)/path_armgcc

$(CACHE_DIR)/$(ARMGCC_PKG):
	@mkdir -p $(CACHE_DIR)
	$(WGET) $(ARMGCC_SRC) --output $@


#######################################################
# install QBS through QtCreator - requires p7zip
install-qbs:
	@$(TOOLS_DIR)/install-qt.sh --version $(VERSION_QTCREATOR) --directory $(QT_INSTALL_DIR) qtcreator
	@make $@-$(HOST_OS)

install-qbs-osx:

install-qbs-linux:


#######################################################
# install Qt and app deploy tools
install-qt:
	@$(TOOLS_DIR)/install-qt.sh --version $(VERSION_QT) --directory $(QT_INSTALL_DIR) $(QT_PACKAGES) ${QT_PACKAGES_${HOST_OS}} | tail -n 1 > $(QT_INSTALL_DIR)/path_qt
	@make $@-$(HOST_OS)

install-qt-osx:

install-qt-linux: install-qt-linux-fix
	@mkdir -p $(CACHE_DIR)
	$(WGET) $(LINUXDEPLOYQT_SRC) --output $(CACHE_DIR)/linuxdeployqt && \
		chmod +x $(CACHE_DIR)/linuxdeployqt && \
		mv $(CACHE_DIR)/linuxdeployqt $(dir $(QMAKE_BIN))
	$(WGET) $(APPIMAGETOOL_SRC) --output $(CACHE_DIR)/appimagetool && \
		chmod +x $(CACHE_DIR)/appimagetool && \
		mv $(CACHE_DIR)/appimagetool $(dir $(QMAKE_BIN))
install-qt-linux-fix:
	@cd $(dir $(QMAKE_BIN))/../plugins/sqldrivers && \
		rm -f libqsqlpsql.so libqsqlmysql.so libqsqlodbc.so

#######################################################
# GStreamer framework
install-gst-osx: $(FRAMEWORKS_DIR)/GStreamer.framework/Versions/1.0/GStreamer

GST_PKGS = \
	gstreamer-1.0-$(VERSION_GST)-x86_64.pkg \
	gstreamer-1.0-devel-$(VERSION_GST)-x86_64.pkg

$(FRAMEWORKS_DIR)/GStreamer.framework/%: $(GST_PKGS:%=$(CACHE_DIR)/%)
	@echo "Installing GStreamer..."
	@for pkg in $^ ; do \
		sudo installer -package $$pkg -target / ; \
	done

$(CACHE_DIR)/gstreamer-%:
	@mkdir -p $(CACHE_DIR)
	$(WGET) https://gstreamer.freedesktop.org/data/pkg/osx/$(VERSION_GST)/$(notdir $@) --output $@


#######################################################
# Sparkle framework
install-sparkle-osx: $(FRAMEWORKS_DIR)/Sparkle.framework/Versions/A/Sparkle

SPARKLE_PKG = Sparkle-$(VERSION_SPARKLE).tar.bz2

$(FRAMEWORKS_DIR)/Sparkle.framework/%: $(SPARKLE_PKG:%=$(CACHE_DIR)/%)
	@echo "Installing Sparkle..."
	@mkdir -p $(BUILD_DIR)/sparkle
	@cd $(BUILD_DIR)/sparkle; \
		bunzip2 -k -c $< | tar xopf -;\
		sudo cp -a Sparkle.framework /Library/Frameworks/

$(CACHE_DIR)/Sparkle-%:
	@mkdir -p $(CACHE_DIR)
	$(WGET) https://github.com/sparkle-project/Sparkle/releases/download/$(VERSION_SPARKLE)/$(notdir $@) --output $@


#######################################################
# SDL framework
install-sdl-osx: $(FRAMEWORKS_DIR)/SDL2.framework/Versions/A/SDL2

SDL_PKG = SDL2-$(VERSION_SDL2).dmg

$(FRAMEWORKS_DIR)/SDL2.framework/%: $(SDL_PKG:%=$(CACHE_DIR)/%)
	@echo "Installing SDL2..."
	hdiutil attach -noverify $<
	sudo cp -a /Volumes/SDL2/SDL2.framework /Library/Frameworks/

$(CACHE_DIR)/SDL2-%:
	@mkdir -p $(CACHE_DIR)
	$(WGET) https://www.libsdl.org/release/$(notdir $@) --output $@


.SECONDARY: $(CACHE_FILES:%=$(CACHE_DIR)/%)

CACHE_FILES = \
	$(GST_PKGS) \
	$(SPARKLE_PKG) \
	$(SDL_PKG) \

