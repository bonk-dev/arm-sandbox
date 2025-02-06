![logo](assets/arm-sandbox.png)

**arm-sandbox** is an ARM emulator which creates a virtual Linux environment for apps.
It's able to run programs built for AArch64, packaged in ELF format.

## Features
- incomplete A64 set emulation support
- A64 disassembler
- dynamic linker (supports only emulating library methods, doesn't actually link to real libraries - yet)
- filesystem emulation
- basic debugger (register reading & breakpoints)

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

