# ---------------------------------------------------------
# Verification
# ---------------------------------------------------------
if(USE_VERIFICATION)
    message(STATUS "Enable Verification")
    #add_compile_definitions(CONFIG_VERIFICATION)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_common_settings INTERFACE
            CONFIG_VERIFICATION              # #define CONFIG_VERIFICATION
    )
endif()

# ---------------------------------------------------------
# Debug
# ---------------------------------------------------------
if(USE_DEBUG)
    message(STATUS "Enable Debugging Feature")
    target_compile_definitions(${CMAKE_PROJECT_NAME}_common_settings INTERFACE
            CONFIG_DEBUG              # #define CONFIG_DEBUG
    )
endif()

