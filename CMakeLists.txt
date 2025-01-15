cmake_minimum_required(VERSION 3.10)
project(ieloader)

enable_language(C)
enable_language(ASM)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})
set(CMAKE_C_STANDARD 11)

set(ARCHS32 "i386")   # TODO: add support later
set(ARCHS64 "x86_64")

# Configure arch
set(ARCH "x86_64")
if(CMAKE_TARGET_ARCH)
    set(ARCH ${CMAKE_TARGET_ARCH})
endif()

if(${ARCH} IN_LIST ARCHS32)
    set(ARCH_TYPE "32")
elseif(${ARCH} IN_LIST ARCHS64)
    set(ARCH_TYPE "64")
else()
    string(REPLACE ";"  ", " archs "${ARCHS64};${ARCHS32}")
    message(FATAL_ERROR
        " Unsupported arch: ${ARCH}."
        " Supported archs: ${archs}"
    )
endif()
message(STATUS "Selected arch: ${ARCH} (${ARCH_TYPE}-bit)")


# Configure target binary
set(SRCS
    ${SRC_DIR}/loader.c
    ${SRC_DIR}/syscalls.c
    ${SRC_DIR}/utils.c
    ${SRC_DIR}/arch/${ARCH}/syscall.S
    ${SRC_DIR}/arch/${ARCH}/trampo.S
)

add_executable(loader ${SRCS})
target_compile_options(loader
    PUBLIC
    -Wall -Wextra -fPIE
    -fno-stack-protector
    -z noexecstack
    -m${ARCH_TYPE}
    -DELFCLASS=ELFCLASS${ARCH_TYPE}
    $<$<CONFIG:DEBUG>:-g3>
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:RELEASE>:-Os>
)
target_link_options(loader
    PUBLIC
    -z noexecstack
    -nostartfiles
    -nodefaultlibs
    -nostdlib
    -static-pie
    -T ${SRC_DIR}/loader.ld
)
add_custom_command(
    TARGET loader POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} --dump-section=.text=${CMAKE_CURRENT_BINARY_DIR}/loader.bin $<TARGET_FILE:loader>
    COMMENT "Exporting loader as a shellcode"
)

# Configure tests
enable_testing()
add_subdirectory(tests)
