# lib/CMakeLists.txt
add_library(vmath STATIC vmath.c nn_layers.c dsp_fir.c dsp_iir.c image_filter.c)

if(TARGET_ARCH STREQUAL "host")
    target_sources(vmath PRIVATE vmath_driver_host.c)
elseif(TARGET_ARCH STREQUAL "riscv32")
    target_sources(vmath PRIVATE vmath_driver_riscv32.c)
endif()

# current directory
target_include_directories(vmath PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)


# set vmath.h as a public header
set_target_properties(vmath PROPERTIES PUBLIC_HEADER "vmath.h")

# link target 'a' to reference target 'hal'
target_link_libraries(vmath PRIVATE
    hal ${CMAKE_PROJECT_NAME}_common_settings
)

if(USE_GOOGLETEST)
    target_link_libraries(vmath PRIVATE GTest::gtest)
endif()

if(USE_COVERAGE)
    target_link_libraries(vmath PRIVATE coverage_config)
endif()


install(TARGETS vmath
        EXPORT CaMathLibTargets
        ARCHIVE DESTINATION lib/${TARGET_ARCH}
        PUBLIC_HEADER DESTINATION include
)
