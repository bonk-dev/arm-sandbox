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
- Clone this repo
- Make sure you have the "Desktop development with C++" workload installed 
- Run Visual Studio -> Open a local folder
- Build

