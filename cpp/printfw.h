#pragma once
/**
 * printfw.h
 */

#ifndef WPRINTF_H
#define WPRINTF_H

#include <Windows.h>
#include <string>
#include <string_view>
#include <vector>

/**
* Prints wide characters
*/
static inline void printfw(const std::wstring_view wstr) {
    if (wstr.empty()) return;
    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    if (!WriteConsoleW(hConsole, wstr.data(), (DWORD)wstr.length(), &written, NULL)) {
        fwprintf(stderr, L"Error\n");
    }
}

/**
 * Prints wide characters and a new line, smart
 */
static inline void printfwl(const std::wstring_view wstr) {
    std::wstring out;
	out.reserve(wstr.length() + 1);
    out.assign(wstr);
    out.append(L"\n");
	printfw(out);
}

/*
* {1, 2, 3}, ";" -> "1;2;3"
*/
static inline void printfwallj(const std::vector<std::wstring>& wstrs, const std::wstring& separator) {
    size_t len = 0;
    for (const std::wstring& wstr : wstrs) len += wstr.length();
    std::wstring out;
	out.reserve(len + separator.length() * (wstrs.size() - 1));
    for (size_t i = 0; i < wstrs.size(); ++i) {
        out += wstrs[i];
        if (i != wstrs.size() - 1) out += separator;
    }
	printfw(out);
}

static inline void printfwall(const std::vector<std::wstring>& wstrs) {
    printfwallj(wstrs, L"");
}

/**
 * Prints a wide character
 */
static inline void putcharw(const wchar_t& c) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    if (!WriteConsoleW(hConsole, &c, 1, &written, NULL)) {
        fwprintf(stderr, L"Error\n");
    }
}

#endif