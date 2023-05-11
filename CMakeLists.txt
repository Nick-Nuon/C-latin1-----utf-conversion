cmake_minimum_required(VERSION 3.22.1) # version can be different


project(Codedump) #name of your project

#add_subdirectory(googletest) # add googletest subdirectory

#CMake complains if I don't include this. 
#cmake_policy(SET CMP0135 NEW)

#include_directories(simdutf) # this is so we can #include <gtest/gtest.h>

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

add_executable(code_dump main.cpp tests.h latin1_to_utf32.h) #simdutf.cpp simdutf.h)# ./tracy/public/TracyClient.cpp) # add this executable

# set options before add_subdirectory
# available options : TRACY_ENABLE , TRACY_ON_DEMAND , TRACY_NO_BROADCAST ,
#TRACY_NO_CODE_TRANSFER , ...
#option ( TRACY_ENABLE "" ON)
#option ( TRACY_ON_DEMAND "" ON)
#add_subdirectory (tracy) # target : TracyClient or alias Tracy :: TracyClient
#Link Tracy::TracyClient to any target where you use Tracy for profiling:
#target_link_libraries (matrices PRIVATE Tracy::TracyClient gtest)