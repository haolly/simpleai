#!/bin/bash

set -e
set -x

clang --version
gcc --version
cmake --version
lsb_release -a
pkg-config --version
doxygen --version

if [ -z "$BUILD_MODE" ]; then
	BUILD_MODE=Debug
fi

if [ "$BUILD_TYPE" == "cmake" ]; then
	mkdir -p simpleai-build
	cd simpleai-build
	cmake -DSIMPLEAI_DEBUGGER=ON -DCMAKE_BUILD_TYPE=$BUILD_MODE -G"Eclipse CDT4 - Unix Makefiles" ..
	make
fi
