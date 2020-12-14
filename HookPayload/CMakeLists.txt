# CMakeList.txt : CMake project for HookPayload, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set (PROJECT_NAME libhookpayload64)
else ()
	set (PROJECT_NAME libhookpayload32)
endif ()

set (Protobuf_USE_STATIC_LIBS)
find_package (protobuf REQUIRED)

# TODO: Ideally this should be done once, but the generated files are put into the build
# tree which makes them hard to pull in.
# TODO: Also need to fix autocomplete.
protobuf_generate_cpp (PROTO_SRCS PROTO_HDRS records.proto)
set_source_files_properties (${PROTO_SRC} ${PROTO_HDRS} PROPERTIES GENERATED TRUE)

add_library ("${PROJECT_NAME}" SHARED "hookpayload.cpp" "hookpayload.h" ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries ("${PROJECT_NAME}" protobuf::libprotobuf)
target_include_directories ("${PROJECT_NAME}" PRIVATE ${Protobuf_INCLUDE_DIRS})
target_include_directories ("${PROJECT_NAME}" PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

#include (GenerateExportHeader)
#generate_export_header ("${PROJECT_NAME}")

# TODO: Add tests and install targets if needed.
install (
	TARGETS "${PROJECT_NAME}"
	CONFIGURATIONS Debug
	RUNTIME DESTINATION .
)