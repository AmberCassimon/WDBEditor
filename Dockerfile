FROM ubuntu:22.04

ARG QT_VERSION
ARG QT_INSTALLER_JWT_TOKEN

# Install some tools
# We need curl to download the Qt installer
# We need build-essential and cmake to actually build our application
# The others are dependencies for the Qt installer
RUN apt-get update -y --no-install-recommends	&&\
    apt-get install -y\
    	build-essential		\
    	cmake				\
    	curl				\
    	libdbus-1-3			\
    	libfontconfig-dev	\
    	libgl-dev			\
    	libxcb-glx0-dev		\
    	libxcb-icccm4-dev	\
    	libxcb-image0-dev	\
    	libxcb-keysyms1-dev	\
    	libxcb-randr0-dev	\
    	libxcb-render-util0-dev\
    	libxcb-shape0-dev	\
    	libxcb-sync-dev		\
    	libxcb-xfixes0-dev	\
    	libxcb-xinerama0-dev\
    	libxcb-xkb-dev		\
    	libxext-dev			\
    	libxkbcommon-x11-dev\
    	libx11-xcb-dev

# Download and run the installer
RUN curl -L "https://d13lb3tujbc8s0.cloudfront.net/onlineinstallers/qt-unified-linux-x64-4.6.1-online.run" --output "/tmp/qt-unified-linux-x64-4.6.1-online.run"						&&\
	chmod u+x "/tmp/qt-unified-linux-x64-4.6.1-online.run"	&&\
	export QT_INSTALLER_JWT_TOKEN="${QT_INSTALLER_JWT_TOKEN}"	&&\
	"/tmp/qt-unified-linux-x64-4.6.1-online.run"\
    	--root "/root/Qt/${QT_VERSION}"\
    	--accept-licenses\
    	--accept-obligations\
    	--accept-messages\
    	--auto-answer telemetry-question=No\
    	--confirm-command\
    	install qt.qt5.$(echo "${QT_VERSION}" | sed "s/\.//g").gcc_64
