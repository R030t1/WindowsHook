# CMakeList.txt : CMake project for hookprotocol, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

set (PROJECT_NAME "hookprotocol")

set (Protobuf_USE_STATIC_LIBS)
find_package (protobuf REQUIRED)

protobuf_generate_cpp (PROTO_SRCS PROTO_HDRS records.proto)
set_source_files_properties (${PROTO_SRC} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)

set (HookProtocol_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
	CACHE INTERNAL "Path to generated protobuf includes.")

# TODO: Better way?
set (HookProtocol_LIBRARIES "${CMAKE_CURRENT_BINARY_DIR}/hookprotocol.lib"
	CACHE INTERNAL "Path to generated protobuf libraries.")

# Add source to this project's executable.
add_library (hookprotocol ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries (hookprotocol protobuf::libprotobuf)
target_include_directories ("${PROJECT_NAME}" PRIVATE ${Protobuf_INCLUDE_DIRS})

# TODO: Add tests and install targets if needed.
install (
	TARGETS hookprotocol
	CONFIGURATIONS Debug
	RUNTIME DESTINATION .
)