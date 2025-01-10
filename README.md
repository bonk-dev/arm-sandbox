# arm-sandbox
This repository contains an AArch64 emulator, which is also a Linux sandbox.

It was made for a school project.

## Build
### Linux
```UNIX
git clone https://github.com/bonk-dev/arm-sandbox
cd arm-sandbox
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -S ../ -B .
make
```

### Windows (Visual Studio)
- TODO: add info about tools
- Visual Studio -> Open a folder
- Build

