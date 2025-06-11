
Build Instructions for Windows: 

git clone --recurse-submodules git@github.com:deeprey-marine/opencpn-plugin.git

cd opencpn-plugin

Run buildwin\win_deps.bat

mkdir build

cd build

Configure the build environment ( use your own path instead of C:/Projects/):
 cmake -A Win32 -G "Visual Studio 17 2022" -DCMAKE_GENERATOR_PLATFORM=Win32 -DwxWidgets_LIB_DIR=C:/Projects/opencpn-plugin/cache/wxWidgets/lib/vc_dll -DwxWidgets_ROOT_DIR=C:/Projects/opencpn-plugin/cache/wxWidgets  ..

Build the plugin:
 cmake --build . --config Release --target tarball