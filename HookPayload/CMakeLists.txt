# CMakeList.txt : CMake project for hookpayload, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set (PROJECT_NAME libhookpayload64)
else ()
	set (PROJECT_NAME libhookpayload32)
endif ()

set (Boost_USE_STATIC_LIBS ON)
find_package (Boost REQUIRED COMPONENTS thread)

set (Protobuf_USE_STATIC_LIBS)
find_package (protobuf REQUIRED)

add_library ("${PROJECT_NAME}" SHARED "hookpayload.cpp" "hookpayload.h")

target_link_libraries ("${PROJECT_NAME}" Boost::thread protobuf::libprotobuf)
target_link_libraries ("${PROJECT_NAME}" ${HookProtocol_LIBRARIES})

target_include_directories ("${PROJECT_NAME}" PRIVATE ${Protobuf_INCLUDE_DIRS})
target_include_directories ("${PROJECT_NAME}" PRIVATE ${HookProtocol_INCLUDE_DIRS})

#include (GenerateExportHeader)
#generate_export_header ("${PROJECT_NAME}")

# TODO: Add tests and install targets if needed.
install (
	TARGETS "${PROJECT_NAME}"
	CONFIGURATIONS Debug
	RUNTIME DESTINATION .
)