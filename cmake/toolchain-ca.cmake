# toolchain-host.cmake

# -g: include debug symbols
# -Og: optimize for debugging

set(CMAKE_CXX_FLAGS "${ARCH_FLAGS} -g -Og -Wall -Wextra -fno-exceptions -fno-rtti" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS   "${ARCH_FLAGS} -g -Og -Wall -Wextra " CACHE STRING "" FORCE)
