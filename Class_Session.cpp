#include "pch.h"
#include "Session.h"

MOCK_STDCALL_FUNC(HRESULT, CoInitializeEx, LPVOID, DWORD);
MOCK_STDCALL_FUNC(void, CoUninitialize);

class Class_Session : public ::testing::Test {
public:
    void SetUp() override {
        // Reset the mock function calls before each test
        VERIFY_AND_CLEAR_MODULE_FUNC(CoInitializeEx);
        VERIFY_AND_CLEAR_MODULE_FUNC(CoUninitialize);
    }

    static void TearDownTestSuite() {
        RESTORE_MODULE_FUNC(CoInitializeEx);
        RESTORE_MODULE_FUNC(CoUninitialize);
    }
protected:
    void MockCoInitializeEx(HRESULT hr) {
        using ::testing::_;
        using ::testing::Return;

        ON_MODULE_FUNC_CALL(CoInitializeEx, _, _).WillByDefault(Return(hr));
    }
};

TEST_F(Class_Session, Valid_Scenarios_ShouldInitialize) {
    using ::testing::_;

    Session session{};
    MockCoInitializeEx(S_OK);

    EXPECT_MODULE_FUNC_CALL(CoInitializeEx, _, _).Times(1);
    EXPECT_NO_THROW(session.Initialize());
}

TEST_F(Class_Session, Valid_Scenarios_ShouldInitializeOnce) {
    using ::testing::_;

    Session session{};
    MockCoInitializeEx(S_OK);

    EXPECT_MODULE_FUNC_CALL(CoInitializeEx, _, _).Times(1);
    session.Initialize();
    session.Initialize();
}

TEST_F(Class_Session, Valid_Scenarios_ShouldInitMultithreaded) {
    using ::testing::_;
    
    Session session{};
    MockCoInitializeEx(S_OK);
    
    EXPECT_MODULE_FUNC_CALL(CoInitializeEx, _, COINIT_MULTITHREADED).Times(1);
    session.Initialize();
}

TEST_F(Class_Session, Valid_Scenarios_ShouldUninitialize) {
    using ::testing::_;

    MockCoInitializeEx(S_OK);
    EXPECT_MODULE_FUNC_CALL(CoUninitialize).Times(1);

    {
        Session session{};
        session.Initialize();
    }
}

TEST_F(Class_Session, Error_Handling_ShouldThrowIfCoInitializeExFails) {
    using ::testing::_;

    Session session{};
    MockCoInitializeEx(E_FAIL);

    EXPECT_MODULE_FUNC_CALL(CoInitializeEx, _, _).Times(1);
    EXPECT_THROW(session.Initialize(), std::runtime_error);
}

TEST_F(Class_Session, Edge_Cases_ShouldInitializeIfAlreadyInitializedByAnotherSession) {
    using ::testing::_;

    Session session{};
    MockCoInitializeEx(S_FALSE);
    
    EXPECT_NO_THROW(session.Initialize());
}