# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "d:/RaspberryPiPico2/pico-sdk")

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(udp_test1 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(udp_test1 
	entry.c 
	async_main.c 
	async_lwip.c 
	async_console.c 
	async_cyw43.c 
	panic.c
)

pico_set_program_name(udp_test1 "udp_test1")
pico_set_program_version(udp_test1 "0.1")

pico_enable_stdio_uart(udp_test1 1)
pico_enable_stdio_usb(udp_test1 1)

# Add the standard library to the build
target_link_libraries(udp_test1
	pico_stdlib
)

# add async context polling
# target_link_libraries(udp_test1 async_context)

# lightweight ip isolated polling mode
target_link_libraries(udp_test1 pico_lwip)
target_link_libraries(udp_test1 pico_lwip_nosys)
target_link_libraries(udp_test1 pico_lwip_arch)

# driver for cyw43
target_link_libraries(udp_test1 cyw43_driver)
target_link_libraries(udp_test1 pico_cyw43_arch)
target_link_libraries(udp_test1 pico_cyw43_arch_lwip_poll)


# Add the standard include files to the build
target_include_directories(udp_test1 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR} # for our common lwipopts or any other standard includes, if required
)

pico_add_extra_outputs(udp_test1)



