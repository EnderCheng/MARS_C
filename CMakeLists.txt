cmake_minimum_required(VERSION 3.12)

project(MARS)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES mars.cpp zk.cpp accu.cpp groth.cpp pairing.cpp bn_pair.cpp zzn12a.cpp ecn2.cpp zzn2.cpp zzn4.cpp big.cpp zzn.cpp ecn.cpp)

set(INC_DIR /Users/cheng/Code/miracl)

include_directories(${INC_DIR})

find_library(
        LIBMIRACL
        NAMES miracl.a
        PATHS /Users/cheng/Code/miracl
        NO_DEFAULT_PATH
)

add_executable(MARS ${SOURCE_FILES})
target_link_libraries(MARS ${LIBMIRACL})