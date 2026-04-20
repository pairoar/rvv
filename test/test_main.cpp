#include <gtest/gtest.h>
#include <iostream>

// 전역(Global) 테스트 환경 설정이 필요한 경우 사용
class AcceleratorEnvironment : public ::testing::Environment {
public:
    ~AcceleratorEnvironment() override {}

    // 모든 테스트가 시작되기 전에 딱 한 번 호출
    void SetUp() override {
        std::cout << "[Global Setup] 하드웨어 시뮬레이터 가상 메모리 초기화..." << std::endl;
        // 예: hw_sim_init();
    }

    // 모든 테스트가 끝난 후 딱 한 번 호출
    void TearDown() override {
        std::cout << "[Global Teardown] 리소스 해제 중..." << std::endl;
        // 예: hw_sim_cleanup();
    }
};

int main(int argc, char **argv) {
    std::cout << "Starting Test for Accelerator Project...\n";
    
    // GTest 초기화
    ::testing::InitGoogleTest(&argc, argv);
    
    // 전역 환경 등록
    ::testing::AddGlobalTestEnvironment(new AcceleratorEnvironment());
    
    // 등록된 모든 테스트 실행
    return RUN_ALL_TESTS();
}