# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.

cmake_minimum_required (VERSION 3.8)
cmake_policy (SET CMP0091 NEW)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
#set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

project ("WindowsHook")

# Include sub-projects.
add_subdirectory ("HookProtocol")
add_subdirectory ("HookPayload")
add_subdirectory ("HookLoader")
add_subdirectory ("Recorder")