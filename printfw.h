#pragma once
/**
 * printfw.h
 */

#include <windows.h>
#include <stdio.h>
#include <wchar.h>

#ifndef WPRINTF_H
#define WPRINTF_H

static inline void printfw(wchar_t *wstr) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    if (!WriteConsoleW(hConsole, wstr, wcslen(wstr), &written, NULL)) {
        fwprintf(stderr, L"Error\n");
    }
}

#endif