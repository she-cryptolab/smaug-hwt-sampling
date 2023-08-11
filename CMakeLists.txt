cmake_minimum_required(VERSION 3.18)
project(smaug-hwt LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)

if(MSVC)
  set(C_FLAGS /nologo /O2 /W4 /wd4146 /wd4244)
else()
  set(C_FLAGS -O3 -fomit-frame-pointer -fPIC -Wall -Wextra)
endif()

set(srcs
    ${PROJECT_SOURCE_DIR}/src/hwt-smaug.c
    ${PROJECT_SOURCE_DIR}/src/hwt-ntru.c
    ${PROJECT_SOURCE_DIR}/src/hwt-bike.c
    ${PROJECT_SOURCE_DIR}/src/rng.c
    ${PROJECT_SOURCE_DIR}/src/fips202.c
)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(smaug1-hwt SHARED ${srcs})
target_compile_definitions(smaug1-hwt PUBLIC SMAUG_MODE=1)
target_compile_options(smaug1-hwt PRIVATE ${C_FLAGS})

add_library(smaug3-hwt SHARED ${srcs})
target_compile_definitions(smaug3-hwt PUBLIC SMAUG_MODE=3)
target_compile_options(smaug3-hwt PRIVATE ${C_FLAGS})

add_library(smaug5-hwt SHARED ${srcs})
target_compile_definitions(smaug5-hwt PUBLIC SMAUG_MODE=5)
target_compile_options(smaug5-hwt PRIVATE ${C_FLAGS})


add_subdirectory(benchmark)
add_subdirectory(test)