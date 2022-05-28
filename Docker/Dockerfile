# TODO:
# The "docker way" is to update the container image, never code from inside the container.
# There is no need for build tools or runtime update support inside the container.


# Test in docker shell:
# docker run -it --rm debian:bullseye /bin/bash
# export DEBIAN_FRONTEND=noninteractive

# Test Docker build:
# docker build ./Docker
# docker build --no-cache ./Docker


# We are building in multiple stages to minimize the final image size
# https://docs.docker.com/develop/develop-images/multistage-build/


# Base image
# https://www.debian.org/releases/bullseye/
FROM debian:bullseye AS base

# Set default timezone to UTC
# TODO: FPP config ignores TZ when doing initial setup
ENV TZ=Etc/UTC

# Prevent EULA and confirmation prompts in installers
ARG DEBIAN_FRONTEND=noninteractive

# Install utilitites and configure locale
RUN \
    && apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y --no-install-recommends \
        locales \
        locales-all \
        mc \
        nano \
    # Generate UTF-8 locale
    && locale-gen --no-purge en_US en_US.UTF-8 \
    # Cleanup
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set default locale to en_US.UTF-8
ENV LANG=en_US.UTF-8 \
    LANGUAGE=en_US:en \
    LC_ALL=en_US.UTF-8



# VLC builder image
FROM base AS vlcbuilder

# Build and install VLC
# https://wiki.videolan.org/Category:Building/
# https://wiki.videolan.org/UnixCompile/
RUN \
    # Add sources
    cp /etc/apt/sources.list /etc/apt/sources.list.d/sources-src.list \
    && sed -i 's|deb http|deb-src http|g' /etc/apt/sources.list.d/sources-src.list \
    && apt-get update \
    && apt-get upgrade -y \
    # Install VLC prerequisites
    && apt-get install -y --no-install-recommends \
        git \
        g++ \
        make \
        libtool \
        automake \
        autopoint \
        pkg-config \
        flex \
        bison \
        lua5.2 \
    # Cleanup
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Build VLC
RUN \
    # Clone git repo
    && cd /opt \
    && git clone git://git.videolan.org/vlc.git \
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
    # Make and install
    && make install \
    # Configure runtime bindings
    && ldconfig \
    # Cleanup
    && make clean \
    && cd ~ \
    && rm -rf /opt/vlc \
    # Print VLC version info
    && vlc --version


# Build and install libhttpserver
# https://github.com/etr/libhttpserver
# https://github.com/etr/libhttpserver#building

# Use version 0.18.2
ENV LIBHTTPSERVER_VER="0.18.2"

RUN \
    # Clone git repo
    && cd /opt \
    && git clone https://github.com/etr/libhttpserver.git \
    && cd libhttpserver \
    && git checkout ${LIBHTTPSERVER_VER} \
    # Bootstrap
    && ./bootstrap \
    && mkdir build \
    && cd build \
    # Configure, use /usr
    && ../configure --prefix=/usr \
    # Make and install
    && make install \
    # Cleanup        
    && cd ~ \
    && rm -rf /opt/libhttpserver


# Build and install FPP

#RUN yes | /root/FPP_Install.sh --skip-apt-install --skip-vlc \
#    && rm -rf /tmp/* \
#    && cp /opt/fpp/etc/asoundrc.plain /root/.asoundrc

# Install FPP prerequisites
# TODO: Create a master list of files to install and remove
RUN \
    && apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y --no-install-recommends \
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
        # samba \
        # samba-common-bin \
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
    # Cleanup
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# TODO: Configure PHP
# TODO: Configure Apache

# Set FPP versions, to be updated at build time
ARG FPPIMAGEVER="2022-05" \
    FPPCFGVER="71"

# Create config dir and write versions to config
RUN \
    mkdir -p /etc/fpp \
    echo "docker" > /etc/fpp/platform \
    echo "v${FPPIMAGEVER}" > /etc/fpp/rfs_version \
    echo "${FPPCFGVER}" > /etc/fpp/config_version

# Copy source from context to image
# COPY SD/FPP_Install.sh /root/FPP_Install.sh
COPY ./ /opt/fpp/

# Build FPP
RUN \
    cd /opt/fpp/src/ \
    make optimized



FROM base AS fpp

# Image labels, version to be updated at build time
ARG LABEL_VERSION="1.0.0.0"
LABEL name="FPP" \
    version=${LABEL_VERSION} \
    description="Falcon Player" \
    maintainer="Falcon Christmas <fpp@users.noreply.github.com>"

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
