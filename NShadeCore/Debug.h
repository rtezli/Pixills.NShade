#pragma once

#include "string"
#include "windows.h"

namespace Debug
{
    static void WriteLine(wchar_t *message)
    {
#ifdef _DEBUG
        OutputDebugString(message);
#endif
    }

    static void WriteLine(const char* m1, const char* m2)
    {
#ifdef _DEBUG
        //auto mes = new wstring(message);
        //auto cstr = mes->c_str();
        //wchar_t *str = cstr;
        //OutputDebugString(mes->c_str());
#endif
    }

    static void WriteLine(std::wstring message)
    {
#ifdef _DEBUG
        const wchar_t *str = message.c_str();
        OutputDebugString(str);
#endif
    }

    static void WriteLine(float message)
    {
#ifdef _DEBUG
        auto mes = std::to_wstring(message);
        mes.append(L"\n");
        const wchar_t *str = mes.c_str();
        OutputDebugString(str);
#endif
    }

    static void WriteLine(std::wstring m1, float m2)
    {
#ifdef _DEBUG
        auto mes = std::to_wstring(m2);
        m1.append(mes);
        m1.append(L"\n");
        const wchar_t *str = m1.c_str();
        OutputDebugString(str);
#endif
    }
    static void WriteCurrentDir()
    {
#ifdef _DEBUG
        wchar_t text[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, text);
        OutputDebugString(text);
#endif
    }
}