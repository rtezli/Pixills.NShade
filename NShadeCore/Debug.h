#pragma once

#include "string"
#include "windows.h"

namespace Debug
{
    static void WriteLine(LPCWSTR message)
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
        //LPCWSTR str = cstr;
        //OutputDebugString(mes->c_str());
#endif
    }

    static void WriteLine(std::wstring message)
    {
#ifdef _DEBUG
        LPCWSTR str = message.c_str();
        OutputDebugString(str);
#endif
    }

    static void WriteLine(float message)
    {
#ifdef _DEBUG
        auto mes = std::to_wstring(message);
        mes.append(L"\n");
        LPCWSTR str = mes.c_str();
        OutputDebugString(str);
#endif
    }

    static void WriteLine(std::wstring m1, float m2)
    {
#ifdef _DEBUG
        auto mes = std::to_wstring(m2);
        m1.append(mes);
        m1.append(L"\n");
        LPCWSTR str = m1.c_str();
        OutputDebugString(str);
#endif
    }
    static void WriteCurrentDir()
    {
#ifdef _DEBUG
        wchar_t wtext[MAX_PATH];
        LPWSTR result = wtext;
        GetCurrentDirectory(MAX_PATH, result);
        OutputDebugString(result);
#endif
    }
}