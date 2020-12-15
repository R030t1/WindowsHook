# CMakeList.txt : CMake project for record, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set (PROJECT_NAME "recorder")

set (Boost_USE_STATIC_LIBS ON)
find_package (Boost REQUIRED COMPONENTS program_options)

set (Protobuf_USE_STATIC_LIBS)
find_package (protobuf REQUIRED)
#set_source_files_properties (${PROTO_SRC} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)

# Add source to this project's executable.
add_executable ("${PROJECT_NAME}" "recorder.cpp" "recorder.h")

target_link_libraries ("${PROJECT_NAME}" Boost::program_options protobuf::libprotobuf)
target_link_libraries ("${PROJECT_NAME}" ${HookProtocol_LIBRARIES})

target_include_directories ("${PROJECT_NAME}" PRIVATE {Boost_INCLUDE_DIRS})
target_include_directories ("${PROJECT_NAME}" PRIVATE {Protobuf_INCLUDE_DIRS})
target_include_directories ("${PROJECT_NAME}" PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories ("${PROJECT_NAME}" PRIVATE ${HookProtocol_INCLUDE_DIRS})

# TODO: Add tests and install targets if needed.
install (
	TARGETS recorder
	CONFIGURATIONS Debug
	RUNTIME DESTINATION .
)