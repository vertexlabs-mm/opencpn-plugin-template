git clone --recurse-submodules -b main https://github.com/cbensassi2/cleanOcpnPlugin
cd cleanOcpnPlugin
mkdir build
cd build
cmake -A Win32 -G "Visual Studio 17 2022" .. && cmake --build . --config Release --target tarball
