# =========================================================================== --- IGNORE ---
# Embedded C++ --- IGNORE ---
# =========================================================================== --- IGNORE ---
# This CMake module configures the project to use embedded C++ features. It sets the appropriate compiler options to disable exceptions and RTTI, which are often not needed in embedded environments and can save valuable resources.
# Usage:
# To enable embedded C++ features, set the USE_EMBEDDED_C++ variable to ON when configuring the project:
# cmake -DUSE_EMBEDDED_C++=ON ..
# ===========================================================================
# Check if embedded C++ is enabled
# The USE_EMBEDDED_C++ variable can be set to ON to enable embedded C++ features.
# If USE_EMBEDDED_C++ is not set or is set to OFF, the embedded C++ configuration will be skipped.
# The compiler options -fno-exceptions and -fno-rtti will be added to the target cp_cxx_options for C++ source files, which will disable exceptions and RTTI support in the compiled code. This can help reduce code size and improve
# performance in embedded environments where these features are not needed.
# ===========================================================================
# if(USE_EMBEDDED_C++)
target_compile_options(cp_cxx_options INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti>
)
