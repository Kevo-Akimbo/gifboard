#!/bin/sh

set -e

docker build -t gifboard-appimage .
mkdir -p "./appimage-out"

docker run \
  -v "$PWD/appimage-out:/out" \
  --rm gifboard-appimage bash -lc '
set -e
. "/root/.cargo/env"
cd /workspace/Gifboard
PKG_CONFIG_PATH=/opt/qt/6.10.0/gcc_64/lib/pkgconfig QMAKE=/opt/qt/6.10.0/gcc_64/bin/qmake cargo build --release

EXTRA_QT_MODULES=waylandcompositor \
 EXTRA_PLATFORM_PLUGINS=libqwayland.so \
 QML_SOURCES_PATHS=/opt/qt/6.10.0/gcc_64/qml/ \
 QMAKE=/opt/qt/6.10.0/gcc_64/bin/qmake \
 LD_LIBRARY_PATH=/opt/qt/6.10.0/gcc_64/lib \
 linuxdeploy -i ./assets/gifboard.png -d ./assets/gifboard.desktop -e ./target/release/gifboard -p qt --appdir AppDir

cp -L /opt/qt/6.10.0/gcc_64/lib/libLayerShellQtInterface.so.6 AppDir/usr/lib/
cp -r /opt/qt/6.10.0/gcc_64/qml/org/ AppDir/usr/qml/
cp -L /opt/qt/6.10.0/gcc_64/lib/libLayerShellQtInterface.so.6 AppDir/usr/lib/
patchelf --set-rpath "\$ORIGIN/../../../../lib" AppDir/usr/qml/org/kde/layershell/libLayerShellQtQml.so
patchelf --set-rpath "\$ORIGIN" AppDir/usr/lib/libLayerShellQtInterface.so.6
appimagetool AppDir/ /out/gifboard-x86_64.AppImage
chown -R $(stat -c "%u:%g" /out) /out
'

printf "\x1b[1;32mAppimage successfully built in ./appimage-out\x1b[0m\n"
