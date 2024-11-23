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
        emulation/emu_types.h
        emulation/executors/FormPcRelAddressExecutor.cpp
        emulation/executors/FormPcRelAddressExecutor.h
        disassembly/prettify.h
        disassembly/prettify.cpp
        emulation/executors/UnconditionalBranchImmediateExecutor.cpp
        emulation/executors/UnconditionalBranchImmediateExecutor.h
        disassembly/instructions/data_proc_imm/AddImmediate.h
        disassembly/instructions/data_proc_imm/AddImmediate.cpp
        disassembly/instructions/data_proc_imm/FormPcRelAddress.h
        disassembly/instructions/data_proc_imm/FormPcRelAddress.cpp
        disassembly/instructions/UnconditionalBranchImmediate.h
        disassembly/instructions/UnconditionalBranchImmediate.cpp
        disassembly/instructions/loads_and_stores/LoadRegisterPair.h
        disassembly/instructions/loads_and_stores/LoadRegisterPair.cpp
        disassembly/instructions/data_proc_imm/MoveWideImmediate.h
        disassembly/instructions/data_proc_imm/MoveWideImmediate.cpp
        emulation/executors/MoveWideImmediateExecutor.cpp
        emulation/executors/MoveWideImmediateExecutor.h
)
