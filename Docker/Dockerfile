# TODO:
# - FPP ignores TZ environment variable when doing initial setup.
#   Default to the TZ as set in environment variables from docker.
# - Use environemnt variables for compelte config, no need to run initial config on boot.
# - The "docker way" is to update the container image, never code from inside the container.
#   There is no need for build tools or runtime update support inside the container.
#   Disable automatic updates when running in docker.
# - Publish compiled binaries for direct consumption in embedded devices.
# - Trim down installed packages to only current and in use.
# - Be explicit with packge selection and use --no-install-recommends.
# - After compiling FPP move the binaries to e.g ./usr/bin/fpp.
#   We want to delete the source and only keep the binaries.
# - Use using FIND_PATH/FIND_LIBRARY to find the correct path for VLC and libhttpserver.
#   The path is assumed to be /usr/local/ while normally the path would be /usr.

# Test in docker shell:
# docker run -it --rm debian:bullseye /bin/bash
# export DEBIAN_FRONTEND=noninteractive

# Test Docker build:
# docker build -t fpp_dev_test --file ./Docker/Dockerfile .
# docker build -t fpp_dev_test --no-cache --file ./Docker/Dockerfile .


# Building in multiple stages ot multiple layers
# https://docs.docker.com/develop/develop-images/multistage-build/


# Base image
# https://www.debian.org/releases/bullseye/
FROM debian:bullseye AS base

# Set default timezone to UTC
ENV TZ=Etc/UTC

# Prevent EULA and confirmation prompts in installers
ARG DEBIAN_FRONTEND=noninteractive

RUN \
    # Upgrade packages
    apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y \
        locales \
        locales-all \
    # Testing
        mc \
        nano \
    # Generate UTF-8 locale
    && locale-gen --no-purge en_US en_US.UTF-8

# Set default locale to en_US.UTF-8
ENV LANG=en_US.UTF-8 \
    LANGUAGE=en_US:en \
    LC_ALL=en_US.UTF-8



# Build tools image
FROM base AS buildtools

# Install common build tools
RUN \
    # Add source repos by converting deb to deb-src from default sources file
    cp /etc/apt/sources.list /etc/apt/sources.list.d/sources-src.list \
    && sed -i 's|deb http|deb-src http|g' /etc/apt/sources.list.d/sources-src.list \
    && apt-get update \
    && apt-get install -y \
        git \
        g++ \
        make \
        libtool \
        automake \
        pkg-config \
        flex \
        bison



# VLC builder image
FROM buildtools AS vlcbuilder

# Invalidate cache when git changes
# https://code.videolan.org/api/v4/projects/ -> 2516
ADD https://code.videolan.org/api/v4/projects/2516/repository/tags/ version.json

# Build VLC
# https://code.videolan.org/videolan/vlc
# https://wiki.videolan.org/Category:Building/
# https://wiki.videolan.org/UnixCompile/
RUN \
    # Install prerequisites
    apt-get install -y \
        automake \
        autopoint \
        git \
        g++ \
        make \
        libtool \
        pkg-config \
        flex \
        bison \
        flex \
        bison \
        lua5.2 \
    # Clone git repo
    && cd /opt \
    && git clone https://code.videolan.org/videolan/vlc.git \
    && cd vlc \
    # Bootstrap
    && ./bootstrap \
    # Get build dependencies, requires deb-src entries in sources.list
    && apt-get build-dep -y vlc \
    # Configure
    && ./configure \
        --disable-a52 \
        --disable-addonmanagermodules \
        --disable-aom \
        --disable-aribb25 \
        --disable-aribsub \
        --disable-avahi \
        --disable-bluray \
        --disable-libcddb \
        --disable-chromecast \
        --disable-chromaprint  \
        --disable-dbus \
        --disable-decklink \
        --disable-dc1394 \
        --disable-dv1394 \
        --disable-dsm \
        --disable-dvdread \
        --disable-dvdnav \
        --disable-ebur128 \
        --disable-jack \
        --disable-fluidlite \
        --disable-fluidsynth \
        --disable-freerdp \
        --disable-gles2 \
        --disable-goom \
        --disable-gst-decode \
        --disable-kms \
        --disable-linsys \
        --disable-live555 \
        --disable-lua \
        --disable-macosx-avfoundation \
        --disable-medialibrary \
        --disable-microdns \
        --disable-mfx \
        --disable-mmal \
        --disable-nfs \
        --disable-opencv \
        --disable-projectm \
        --disable-rav1e \
        --disable-rnnoise \
        --disable-sid \
        --disable-schroedinger \
        --disable-screen \
        --disable-sftp \
        --disable-shout \
        --disable-smb2 \
        --disable-smbclient \
        --disable-pulse \
        --disable-qt \
        --disable-skins2 \
        --disable-srt \
        --disable-v4l2 \
        --disable-vcd \
        --disable-vnc \
        --disable-wayland \
        --disable-xcb \
        --enable-run-as-root \
        # use /builder for output so we can copy from layer
        --prefix=/builder \
    # Make and install
    && make install \
    # Configure runtime bindings
    && ldconfig \
    # Cleanup
    && make clean \
    && cd ~ \
    && rm -rf /opt/vlc
    # Print VLC version info
    # && vlc --version



# libhttpserver builder image
FROM buildtools AS libhttpbuilder

# Use version 0.18.2
ENV LIBHTTPSERVER_VER="0.18.2"

# Invalidate cache when git changes
ADD "https://api.github.com/repos/etr/libhttpserver/git/refs/tags/${LIBHTTPSERVER_VER}" version.json

# Build libhttpserver
# https://github.com/etr/libhttpserver
# https://github.com/etr/libhttpserver#building
RUN \
    # Install prerequisites
    apt-get install -y \
        automake \
        autopoint \
        git \
        g++ \
        gnutls-bin \
        libmicrohttpd-dev \
        make \
    # Clone git repo
    && cd /opt \
    && git clone -b "${LIBHTTPSERVER_VER}" https://github.com/etr/libhttpserver.git \
    && cd libhttpserver \
    # Bootstrap
    && ./bootstrap \
    && mkdir build \
    && cd build \
    # Configure, use /builder for output so we can copy from layer
    && ../configure --prefix=/builder \
    # Make and install
    && make install \
    # Cleanup        
    && cd ~ \
    && rm -rf /opt/libhttpserver



# FPP builder image
FROM buildtools AS fppbuilder

# Copy content from build layers
# TODO: Why /user/local vs. /usr, consider using FIND_PATH/FIND_LIBRARY
COPY --from=vlcbuilder /builder /usr/local
COPY --from=libhttpbuilder /builder /usr/local

# Copy source from git root context to image
# Make sure that .git is not in .gitignore as the build uses "git describe" to get the version info
COPY . /opt/fpp/

# Build FPP
RUN \
    # Install FPP prerequisites
    apt-get install -y \
        # TODO: This is the complete list from the combined scripts, trim trim trim
        alsa-utils \
        apache2 \
        apache2-bin \
        apache2-data \
        apache2-utils \
        apt-transport-https \
        apt-utils \
        arping \
        at \
        avahi-daemon \
        avahi-utils \
        bash-completion \
        bc \
        bison \
        bridge-utils \
        btrfs-progs \
        build-essential \
        bzip2 \
        ca-certificates \
        ccache \
        curl \
        device-tree-compiler \
        dh-autoreconf \
        dhcp-helper \
        dnsmasq \
        dos2unix \
        ethtool \
        exfat-fuse \
        fbi \
        fbset \
        file \
        flex \
        flite \
        g++ \
        gcc \
        gdb \
        gettext \
        git \
        git-core \
        gpiod \
        graphicsmagick-libmagick-dev-compat \
        haveged \
        hdparm \
        hostapd \
        i2c-tools \
        ifplugd \
        iputils-ping \
        less \
        libapache2-mod-php \
        libasound2-dev \
        libavcodec-dev \
        libavdevice-dev \
        libavfilter-dev \
        libavformat-dev \
        libcurl4-openssl-dev \
        libgpiod-dev \
        libgraphicsmagick++1-dev \
        libiio-utils \
        libjsoncpp-dev \
        liblog4cpp5-dev \
        libmicrohttpd-dev \
        libmosquitto-dev \
        libsdl2-dev \
        libssl-dev \
        libswresample-dev \
        libswscale-dev \
        libtag1-dev \
        libtheora-dev \
        libvorbis-dev \
        libx265-dev \
        libzstd-dev \
        locales \
        locales-all \
        lsb-release \
        lshw \
        lsof \
        lzma \
        mailutils \
        mesa-common-dev \
        mosquitto \
        mosquitto-clients \
        mp3info \
        nano \
        net-tools \
        ntp \
        parprouted \
        php \
        php-cli \
        php-common \
        php-curl \
        php-pear \
        php-sqlite3 \
        php-xml \
        php-zip \
        pkg-config \
        rsync \
        samba \
        samba-common-bin \
        shellinabox \
        software-properties-common \
        sqlite3 \
        sudo \
        sysstat \
        tcpdump \
        time \
        unzip \
        usb-modeswitch \
        usbutils \
        vim \
        vim-common \
        vorbis-tools \
        vsftpd \
        wget \
        wireless-tools \
        x265 \
        zip \
        zstd \
    && cd /opt/fpp/src/ \
    && mkdir -p /usr/bin/fpp \
    && make optimized



# FPP runtime image
FROM base AS fpp

# Image labels, version to be updated at build time
ARG LABEL_VERSION="1.0.0.0"
LABEL name="FPP" \
    version=${LABEL_VERSION} \
    description="Falcon Player" \
    maintainer="Falcon Christmas <fpp@users.noreply.github.com>"

# Set FPP versions, to be updated at build time
ARG FPPPLATFORM="docker:debian:bullseye" \
    FPPIMAGEVER="2022-05" \
    FPPCFGVER="71"


# TODO: Configure PHP
# TODO: Configure Apache

# Copy content from build layers
COPY --from=vlcbuilder /builder /usr
COPY --from=libhttpbuilder /builder /usr
#COPY --from=fppbuilder /usr/bin/fpp /usr/bin/fpp

RUN \
    # Install prerequisites
    apt-get install -y \
        apache2 \
        mosquitto \
        php \
    # Create FPP version configs
    && mkdir -p /etc/fpp \
    && echo "${FPPPLATFORM}" > /etc/fpp/platform \
    && echo "v${FPPIMAGEVER}" > /etc/fpp/rfs_version \
    && echo "${FPPCFGVER}" > /etc/fpp/config_version \
    # Create media directory
    && mkdir -p "/home/fpp/media" \
    # Cleanup
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Exposed ports
# HTTP Web UI
EXPOSE 80/tcp
# DDP
EXPOSE 4048/udp
# sACN / E1.31
EXPOSE 5568/udp
# MultiSync
EXPOSE 32320/udp
# FPPD API
EXPOSE 32322/tcp
# HTTP Virtual Display
EXPOSE 32328/tcp

# Mount point
VOLUME [ "/home/fpp/media" ]

# TODO: Upgrade config on every start
# sh scripts/upgrade_config -notee

# Entrypoint
ENTRYPOINT [ "/opt/fpp/Docker/runDocker.sh" ]
