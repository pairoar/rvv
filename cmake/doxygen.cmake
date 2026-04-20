# ===========================================================================
# 5. Doxygen
# ===========================================================================
if(USE_DOXYGEN)
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
endif()
