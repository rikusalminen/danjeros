set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_TOOLCHAIN_PREFIX x86_64-pc-elf)

set(CMAKE_ASM-ATT-COMPILER as)

include(CMakeForceCompiler)
cmake_force_c_compiler(${CMAKE_TOOLCHAIN_PREFIX}-gcc GNU)
set(CMAKE_C_FLAGS "-std=c99 -m64 -mcmodel=large -mno-red-zone -nostdlib -nostartfiles -nodefaultlibs -ffreestanding -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow")
set(CMAKE_C_FLAGS_DEBUG "-ggdb -W -Wall -Wextra -Winline")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/toolchain/linker.ld)
set(CMAKE_EXE_LINKER_FLAGS "-N -T${LINKER_SCRIPT}")

set(CMAKE_FIND_ROOT_PATH ${CMAKE_SOURCE_DIR})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

