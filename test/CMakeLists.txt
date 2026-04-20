# enable_testing()

# 테스트 실행 파일 생성
add_executable(run_tests
    test_main.cpp
    test_module_a.cpp
    #test_module_vadd.cpp
)

# GTest 및 프로젝트 라이브러리 링크
# gtest_main을 링크하면 별도의 main() 함수 작성 없이도 테스트가 실행됩니다.
target_link_libraries(run_tests
    PRIVATE
        vmath hal hw_sim util ${CMAKE_PROJECT_NAME}_common_settings
)

if(USE_GOOGLETEST)
    target_link_libraries(vmath PRIVATE GTest::gtest)
endif()

if(USE_COVERAGE)
    target_link_libraries(run_tests PRIVATE coverage_config)
endif()

# 테스트 등록 (ctest를 통해 실행 가능)
include(GoogleTest)
gtest_discover_tests(run_tests)
