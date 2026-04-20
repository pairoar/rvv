# util/CMakeLists.txt
add_library(util STATIC dbg.c)

# include current directory
target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#
target_link_libraries(util PRIVATE ${CMAKE_PROJECT_NAME}_common_settings)

if(USE_COVERAGE)
    target_link_libraries(util PRIVATE coverage_config)
endif()
