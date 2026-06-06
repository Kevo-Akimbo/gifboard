FROM ubuntu:24.04 AS base

ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /workspace
SHELL ["/bin/bash", "-c"]

RUN <<EOF
apt-get update && apt-get install -y build-essential python3-pip python3-venv python3 wget git clang lld pkg-config curl \
  libglib2.0-0 libxcb1-dev libx11-dev libgl-dev libxkbcommon-x11-dev patchelf libgpgme11-dev libgcrypt20-dev \
  libxcb-cursor0 libgl1-mesa-dev libwayland-dev wayland-protocols libdbus-1-3 libxcb-icccm4 glib2.0 \
  libxcb-keysyms1 libxcb-shape0 libxcb-cursor0 libtiff5-dev squashfs-tools desktop-file-utils cimg-dev
rm -rf /var/lib/apt/lists/*
EOF

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

ADD --chmod=+x \
    https://github.com/Kitware/CMake/releases/download/v3.29.0/cmake-3.29.0-linux-x86_64.sh \
    cmake-3.29.0-linux-x86_64.sh
RUN ./cmake-3.29.0-linux-x86_64.sh --skip-license --prefix=/usr/local

RUN <<EOF
python3 -m venv /opt/aqt-env
source /opt/aqt-env/bin/activate
pip install --no-cache-dir aqtinstall
aqt install-qt linux desktop 6.10.0 linux_gcc_64 --outputdir /opt/qt -m qtimageformats
EOF

FROM base AS stage1

ADD https://github.com/KDE/extra-cmake-modules.git#v6.26.0 extra-cmake-modules
ADD https://invent.kde.org/plasma/layer-shell-qt.git#v6.6.91 layer-shell-qt
ADD https://github.com/linuxdeploy/linuxdeploy.git#a9f929ff0e32d5c4bcb7b5c380adff4802f918ba \
    linuxdeploy
ADD https://github.com/linuxdeploy/linuxdeploy-plugin-qt.git#409307dd1614ff2120e748fdc3442601432c9c3f \
    linuxdeploy-plugin-qt
ADD https://github.com/AppImage/appimagetool.git#continuous \
    appimagetool

RUN cd extra-cmake-modules && \
    cmake -S . -B build  && \
    cmake --build build -j$(nproc)  && \
    cmake --install ./build 

RUN cd layer-shell-qt && \
    PATH="$PATH:/opt/qt/6.10.0/gcc_64/bin/" cmake -S . -B build && \
    PATH="$PATH:/opt/qt/6.10.0/gcc_64/bin/" cmake --build build -j$(nproc) && \
    cmake --install ./build --prefix /opt/layer-shell-qt && \
    mv /opt/layer-shell-qt/include/LayerShellQt/ /opt/qt/6.10.0/gcc_64/include && \
    mv /opt/layer-shell-qt/lib/x86_64-linux-gnu/plugins/wayland-shell-integration/* /opt/qt/6.10.0/gcc_64/plugins/wayland-shell-integration/ && \
    rm -r /opt/layer-shell-qt/lib/x86_64-linux-gnu/plugins && \
    mkdir -p /opt/qt/6.10.0/gcc_64/qml/org/kde/layershell/ && \
    mv /opt/layer-shell-qt/lib/x86_64-linux-gnu/qml/org/kde/layershell/* /opt/qt/6.10.0/gcc_64/qml/org/kde/layershell/ && \
    rm -r /opt/layer-shell-qt/lib/x86_64-linux-gnu/qml && \
    mv /opt/layer-shell-qt/lib/x86_64-linux-gnu/cmake/* /opt/qt/6.10.0/gcc_64/lib/cmake && \
    rm -r /opt/layer-shell-qt/lib/x86_64-linux-gnu/cmake/ && \
    mv /opt/layer-shell-qt/lib/x86_64-linux-gnu/* /opt/qt/6.10.0/gcc_64/lib/ 

RUN cd linuxdeploy/ && \
    cmake -S . -B build && \
    cmake --build build -j$(nproc) && \
    cmake --install ./build/ 

RUN cd linuxdeploy-plugin-qt/ && \
    cmake -S . -B build && \
    cmake --build build -j$(nproc) && \
    cmake --install ./build/ 

RUN cd appimagetool/ && \
    cmake -S . -B build && \
    cmake --build build -j$(nproc) && \
    cmake --install build

# libqtiff6 isnt in the apt repos so I decided to just not include it 
RUN rm /opt/qt/6.10.0/gcc_64/plugins/imageformats/libqtiff.so

ADD . Gifboard
