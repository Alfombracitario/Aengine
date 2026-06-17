#pragma once
#include <stdbool.h>

void openTextConsole();

#ifdef DEBUG_MODE
    void _aAssert(bool cond, const char* msg, const char* file, int line);
    #define aAssert(cond, msg) _aAssert(cond, msg, __FILE__, __LINE__)
#else
    #define aAssert(cond, msg)
#endif