cmake_minimum_required(VERSION 3.20)
project(NovaKernelCore VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(nova_kernel_sim
    src/main.cpp
    src/kernel_simulator.cpp
)

target_compile_options(nova_kernel_sim PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /O2>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -O3 -pthread>
)
