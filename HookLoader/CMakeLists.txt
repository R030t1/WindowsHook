# CMakeList.txt : CMake project for HookPayload, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set (PROJECT_NAME hookloader64)
else ()
	set (PROJECT_NAME hookloader32)
endif ()

# Add source to this project's executable.
add_executable ("${PROJECT_NAME}" WIN32 "hookloader.cpp" "hookloader.h")

# TODO: Add tests and install targets if needed.
install (
	TARGETS "${PROJECT_NAME}"
	CONFIGURATIONS Debug
	RUNTIME DESTINATION .
)