if(BUILD_32)
    if (APPLE)
        message(FATAL_ERROR "We only support 32bit building under Linux.")
    else()
        message(STATUS "Build 32bit")

        include(CheckCSourceCompiles)
        include(CheckCXXSourceCompiles)

        message(STATUS "Build Info: Checking if 32-bit (-m32) compilation is supported on Host...")

        # -m32 : temporarily set -m32 flags and then test compile
        set(CMAKE_REQUIRED_FLAGS "-m32")
        
        check_c_source_compiles("int main() { return 0; }" C_MULTILIB_WORKS)
        check_cxx_source_compiles("int main() { return 0; }" CXX_MULTILIB_WORKS)
        
        # unset temporarily flag
        unset(CMAKE_REQUIRED_FLAGS)

        # verify
        if(NOT C_MULTILIB_WORKS OR NOT CXX_MULTILIB_WORKS)
            # fatal error and stop cmake
            message(FATAL_ERROR 
                "32-bit compilation (-m32) failed!\n"
                "To build the 32-bit simulator on a 64-bit host, multilib support is required.\n"
                "If you are on Ubuntu/Debian, please run:\n"
                "  sudo apt install gcc-multilib g++-multilib\n"
            )
        else()
            message(STATUS "Build Info: 32-bit compilation (-m32) is fully supported.")

            target_compile_options(${CMAKE_PROJECT_NAME}_common_settings INTERFACE -m32)
            target_link_options(${CMAKE_PROJECT_NAME}_common_settings INTERFACE -m32)

            # define macro (-D)
            target_compile_definitions(${CMAKE_PROJECT_NAME}_common_settings INTERFACE
                CONFIG_SUPPORT_32              # #define CONFIG_SUPPORT_32
            )
            
        endif()
    endif()
endif()