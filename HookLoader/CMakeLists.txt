# CMakeList.txt : CMake project for HookPayload, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (HookLoader "HookLoader.cpp" "HookLoader.h")

# TODO: Add tests and install targets if needed.
install (
	TARGETS HookLoader
	CONFIGURATIONS Debug
	RUNTIME DESTINATION .
)