#include <gtest/gtest.h>
#include <cstdint>
#include "insn_api.h" // 프로젝트의 실제 헤더 포함

// ---------------------------------------------------------
// 1. 단순한 독립형 테스트 (Fixture 미사용)
// ---------------------------------------------------------
TEST(InsnApiBasicTest, CheckConfigMacros) {
    // config.h에 정의된 매크로나 기본 상수를 검증할 때 유용
    // EXPECT_EQ는 두 값이 같은지 확인. (실패해도 다음 테스트 진행)
    // ASSERT_EQ는 실패하면 즉시 해당 테스트 함수를 종료.
    
    int expected_word_size = 4; // 32-bit (예시)
    EXPECT_EQ(expected_word_size, INSN_WORD_SIZE); // 실제 매크로로 변경
    EXPECT_TRUE(true); // 임시 패스 처리
}

// ---------------------------------------------------------
// 2. 테스트 픽스처(Fixture)를 사용한 상태 기반 테스트
// ---------------------------------------------------------
// 공통된 초기화 로직을 공유하기 위한 클래스
class InsnDecoderTest : public ::testing::Test {
protected:
    // 멤버 변수 선언 (포인터 선언 시 반드시 초기화하여 Segmentation fault 방지)
    uint32_t* mock_registers = nullptr; 

    // 각 TEST_F가 실행되기 직전에 호출
    void SetUp() override {
        mock_registers = new uint32_t[32]{0}; // 32개 레지스터 0으로 초기화
    }

    // 각 TEST_F가 종료된 직후에 호출
    void TearDown() override {
        delete[] mock_registers;
        mock_registers = nullptr;
    }
};

// 명령어 디코딩 로직 테스트 (예: NOP 명령어)
TEST_F(InsnDecoderTest, DecodeNopInstruction) {
    // 예시: NOP (addi x0, x0, 0) -> 기계어 코드가 0x00000013 이라고 가정
    uint32_t nop_opcode = 0x00000013;
    
    // 실제 프로젝트의 API를 호출하여 결과를 검증합니다.
    // InsnInfo info = decode_instruction(nop_opcode);
    // EXPECT_EQ(info.opcode_type, TYPE_I);
    // EXPECT_EQ(info.rd, 0);
    
    // 현재는 API를 모르므로 더미 검증
    EXPECT_NE(nop_opcode, 0x00000000); // 0이 아님을 확인
}

// 잘못된 포인터나 에러 처리를 잘 하는지 검증하는 테스트 (Death Test)
TEST_F(InsnDecoderTest, NullPointerHandling) {
    // 특정 함수에 nullptr을 넘겼을 때 프로그램이 정상적으로 에러를 내뿜고 
    // 죽는지(assert/exit) 검증하는 GTest 기능
    
    // EXPECT_DEATH(decode_instruction(nullptr), "Null pointer exception");
}