# CMakeList.txt : CMake project for hookprotocol, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

set (Protobuf_USE_STATIC_LIBS)
find_package (protobuf REQUIRED)
include_directories (${Protobuf_INCLUDE_DIRS})
protobuf_generate_cpp (PROTO_SRCS PROTO_HDRS records.proto)

set (Protocol_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
	CACHE INTERNAL "Path to generated protobuf files.")

# Add source to this project's executable.
add_library (hookprotocol ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries (hookprotocol protobuf::libprotobuf)

# TODO: Add tests and install targets if needed.
install (
	TARGETS hookprotocol
	CONFIGURATIONS Debug
	RUNTIME DESTINATION .
)