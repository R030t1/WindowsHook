# CMakeList.txt : CMake project for HookPayload, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set (Boost_USE_STATIC_LIBS ON)
find_package (Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

find_package (protobuf REQUIRED)
include_directories (${Protobuf_INCLUDE_DIRS})
# TODO: Generate files.

# Add source to this project's executable.
add_executable (recorder "recorder.cpp" "recorder.h")
target_link_libraries (recorder Boost::program_options protobuf::libprotobuf)

# TODO: Add tests and install targets if needed.
install (
	TARGETS recorder
	CONFIGURATIONS Debug
	RUNTIME DESTINATION .
)