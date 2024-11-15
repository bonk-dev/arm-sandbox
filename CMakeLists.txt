cmake_minimum_required(VERSION 3.28)
project(arm_sandbox)

set(CMAKE_CXX_STANDARD 20)

add_executable(arm_sandbox main.cpp
        disassembly/A64Decoder.cpp
        disassembly/A64Decoder.h
        emulation/AArch64Cpu.cpp
        emulation/AArch64Cpu.h
        emulation/executors/AddSubImmediateExecutor.cpp
        emulation/executors/AddSubImmediateExecutor.h
        numbers/two_complement.h
        emulation/executors/LoadStoreRegPairExecutor.cpp
        emulation/executors/LoadStoreRegPairExecutor.h
        emulation/executors/ExecutorBase.h
        emulation/executors/ExecutorBase.cpp
        emulation/CpuVirtualMemory.cpp
        emulation/CpuVirtualMemory.h
)
