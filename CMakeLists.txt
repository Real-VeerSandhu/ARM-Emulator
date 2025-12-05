cmake_minimum_required(VERSION 3.10)
project(arm_emulator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable all warnings and treat them as errors
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Enable address sanitizer in Debug mode
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address,undefined")

# Add executable
add_executable(arm_emulator 
    src/main.cpp
    src/cpu.cpp
    src/registers.cpp
    src/memory.cpp
    src/decoder.cpp
    src/repl.cpp
)

# Include directories
target_include_directories(arm_emulator PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add tests if needed
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
