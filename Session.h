#pragma once

#include <Windows.h>

class Session {
public:
    ~Session();
    void Initialize();    
private:
    bool m_initialized = false;
};
