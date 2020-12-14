# CMakeList.txt : CMake project for HookPayload, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set (PROJECT_NAME hookpayload64)
else ()
	set (PROJECT_NAME hookpayload32)
endif ()

# Add source to this project's executable.
add_library ("${PROJECT_NAME}" SHARED "hookpayload.cpp" "hookpayload.h")

# TODO: Add tests and install targets if needed.
install (
	TARGETS "${PROJECT_NAME}"
	CONFIGURATIONS Debug
	RUNTIME DESTINATION .
)