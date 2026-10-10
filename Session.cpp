#include "pch.h"
#include <stdexcept>
#include "Session.h"

Session::~Session() {
    if (m_initialized) {
        CoUninitialize();
        m_initialized = false;
    }
}

void Session::Initialize() {
    if (m_initialized) {
        return;
    }

    switch (CoInitializeEx(nullptr, COINIT_MULTITHREADED)) {
        case S_OK:
        case S_FALSE:
            break;
        default:
            throw std::runtime_error("Failed to initialize");
    }

    m_initialized = true;
}
