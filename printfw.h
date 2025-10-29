#pragma once
/**
 * printfw.h
 */

#include <windows.h>
#include <stdio.h>
#include <wchar.h>

#ifndef WPRINTF_H
#define WPRINTF_H

/**
 * Prints wide characters
 */
static inline void printfw(wchar_t *wstr) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    if (!WriteConsoleW(hConsole, wstr, wcslen(wstr), &written, NULL)) {
        fwprintf(stderr, L"Error\n");
    }
}

/**
 * Prints wide characters, 2 strings
 */
static inline void printfw2(wchar_t *wstr, wchar_t *wstr2) {
    wchar_t buff[wcslen(wstr) + wcslen(wstr2) + 1];
    memcpy(buff, wstr, wcslen(wstr) * sizeof(wchar_t));
    memcpy(buff + wcslen(wstr), wstr2, wcslen(wstr2) * sizeof(wchar_t));
    buff[wcslen(wstr) + wcslen(wstr2)] = L'\0';
    printfw(buff);
}

/**
 * Prints wide characters, 3 strings
 */
static inline void printfw3(wchar_t *wstr, wchar_t *wstr2, wchar_t *wstr3) {
    wchar_t buff[wcslen(wstr) + wcslen(wstr2) + wcslen(wstr3) + 2];
    memcpy(buff, wstr, wcslen(wstr) * sizeof(wchar_t));
    memcpy(buff + wcslen(wstr) + 1, wstr2, wcslen(wstr2) * sizeof(wchar_t));
    memcpy(buff + wcslen(wstr) + wcslen(wstr2) + 1, wstr3, wcslen(wstr3) * sizeof(wchar_t));
    buff[wcslen(wstr) + wcslen(wstr2) + wcslen(wstr3) + 2] = L'\0';
    printfw(buff);
}

static inline void printfwallj(wchar_t *wstrs[], int count, wchar_t *separator) {
    size_t len = 0;
    for (int i = 0; i < count; ++i) len += wcslen(wstrs[i]);
    wchar_t *buff = (wchar_t *) malloc((len + count + 1) * sizeof(wchar_t));
    size_t offset = 0;
    for (int i = 0; i < count; ++i) {
        wcscpy(buff + offset, wstrs[i]);
        offset += wcslen(wstrs[i]);
        if (i < count - 1) wcscat(buff + offset, separator);
        offset += wcslen(separator);
    }
    buff[len + count] = L'\0';
    printfw(buff);
    free(buff);
}

static inline void printfwall(wchar_t *wstrs[], int count) {
    printfwallj(wstrs, count, L"");
}

/**
 * Prints wide characters and a new line, smart
 */
static inline void printfwl(wchar_t *wstr) {
    size_t len = wcslen(wstr);
    if (len > 512) {
        wchar_t *buff = (wchar_t *) malloc(len + 2);
        memcpy(buff, wstr, len * sizeof(wchar_t));
        buff[len] = L'\n';
        buff[len + 1] = L'\0';
        printfw(buff);
        free(buff);
    } else {
        wchar_t buff[len + 2];
        memcpy(buff, wstr, len * sizeof(wchar_t));
        buff[len] = L'\n';
        buff[len + 1] = L'\0';
        printfw(buff);
    }
}

/**
 * Prints a wide character
 */
static inline void putcharw(wchar_t c) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    if (!WriteConsoleW(hConsole, &c, 1, &written, NULL)) {
        fwprintf(stderr, L"Error\n");
    }
}

#endif