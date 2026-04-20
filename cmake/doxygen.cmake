# ===========================================================================
# Doxygen
# ===========================================================================
# This CMake module configures Doxygen to generate API documentation for the project. It checks if Doxygen is available and sets up the necessary variables and targets to create the
# documentation from the source code. The generated documentation will be placed in the build directory under "docs".
# Usage:
# To enable Doxygen documentation generation, set the USE_DOXYGEN variable to ON when configuring the project:
# cmake -DUSE_DOXYGEN=ON ..
# ===========================================================================
# Check if Doxygen is enabled
# The USE_DOXYGEN variable can be set to ON to enable Doxygen documentation generation.
# If USE_DOXYGEN is not set or is set to OFF, the Doxygen configuration will be skipped.
# The Doxygen documentation will be generated from the source code in the specified directories (inc, hal, app, util).
# The generated documentation will be placed in the build directory under "docs".
# ===========================================================================
# if(USE_DOXYGEN)

# find doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND)
    # configure doxygen variables
    set(DOXYGEN_PROJECT_NAME "Hardware Accelerator & Simulator")
    set(DOXYGEN_PROJECT_BRIEF "Custom Hardware Accelerator and Software Stack API")
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs")

    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)

    # find directory recursive
    set(DOXYGEN_RECURSIVE YES)

    # add custom target for building doxygen document (target name : docs)
    doxygen_add_docs(
        docs
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/hal
        ${CMAKE_CURRENT_SOURCE_DIR}/application
        ${CMAKE_CURRENT_SOURCE_DIR}/util
        COMMENT "Generating API documentation with Doxygen..."
    )
else()
    message(WARNING "Doxygen need to be installed to generate the doxygen documentation")
endif()

