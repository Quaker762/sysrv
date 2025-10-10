#
# RV64 CMake Toolchain File
#
# Parts taken from github.com/five-embeddev/riscv-scratchpad/blob/master/cmake/cmake/riscv.cmake
#
# Copyright (C) 2025 Jesse Buhagiar
# Copyright (C) five-embeddev
#

set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_PROCESSOR  rv64gzbb)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

FIND_FILE(RV64_UNKNOWN_ELF_GCC "riscv64-unknown-elf-gcc" PATHS ENV INCLUDE)

if (EXISTS ${RV64_UNKNOWN_ELF_GCC})
    set(RISCV_GCC ${RV64_UNKNOWN_ELF_GCC})
else()
    message(FATAL_ERROR "Cross Compiler not found!")
endif()

get_filename_component(RV64_TOOLCHAIN_BIN_PATH ${RISCV_GCC} DIRECTORY)
get_filename_component(RV64_TOOLCHAIN_BIN_GCC ${RISCV_GCC} NAME_WE)
get_filename_component(RV64_TOOLCHAIN_BIN_EXT ${RISCV_GCC} EXT)

message("Found Cross Compiler @ ${RISCV_GCC}")

# Remove gcc so we have the root "riscv64-unknown-elf" name
STRING(REGEX REPLACE "\-gcc" "-" RV64_COMPILER_TRIPLE ${RV64_TOOLCHAIN_BIN_GCC})

set(CMAKE_ASM_COMPILER {RV64_COMPILER_TRIPLE}gcc)
set(CMAKE_AR ${RV64_COMPILER_TRIPLE}ar)
set(CMAKE_ASM_COMPILER ${RV64_COMPILER_TRIPLE}gcc)
set(CMAKE_C_COMPILER ${RV64_COMPILER_TRIPLE}gcc)
set(CMAKE_CXX_COMPILER ${RV64_COMPILER_TRIPLE}g++)

# We must set the OBJCOPY setting into cache so that it's available to the
# whole project. Otherwise, this does not get set into the CACHE and therefore
# the build doesn't know what the OBJCOPY filepath is
set(CMAKE_OBJCOPY ${RV64_TOOLCHAIN_BIN_PATH}/${RV64_COMPILER_TRIPLE}objcopy CACHE FILEPATH "The toolchain objcopy command " FORCE)

# Set the CMAKE C flags (which should also be used by the assembler!
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mabi=lp64d")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcmodel=medany")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__SYSRV_ARCH_RV64__")

# Set the C standard to C23
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON) # We _must_ follow the ISO C standard!

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -march=${CMAKE_SYSTEM_PROCESSOR} -nostartfiles")