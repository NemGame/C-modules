#pragma once

/**
 * table.h
 */

#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <my/printfw.h>

char *centeredText(char *text, int width) {
    int len = strlen(text);
    if (len >= width) {
        char *copy = (char*)calloc((len + 1), sizeof(char));
        strcpy(copy, text);
        return copy;
    }
    int spaces = width - len;
    int left = spaces / 2, right = spaces - left;
    int i;
    char *result = (char*)calloc(width + 1, sizeof(char));
    int resultLen = 0;
    for (int i = 0; i < left; ++i) {
        result[resultLen++] = ' ';
    }
    memcpy(result + resultLen, text, len);
    resultLen += len;
    for (int i = 0; i < right; ++i) {
        result[resultLen++] = ' ';
    }
    return result;
}

wchar_t *centeredTextW(wchar_t *text, int width) {
    int len = wcslen(text);
    if (len >= width) {
        wchar_t *copy = (wchar_t*)calloc((len + 1), sizeof(wchar_t));
        wcscpy(copy, text);
        return copy;
    }
    int spaces = width - len;
    int left = spaces / 2, right = spaces - left;
    int i;
    wchar_t *result = (wchar_t*)calloc(width + 1, sizeof(wchar_t));
    int resultLen = 0;
    for (int i = 0; i < left; ++i) {
        result[resultLen++] = L' ';
    }
    wcscpy(result + resultLen, text);
    resultLen += len;
    for (int i = 0; i < right; ++i) {
        result[resultLen++] = L' ';
    }
    return result;
}

void centerText(char **text, int width) {
    char *tmp = centeredText(*text, width);
    free(*text);
    *text = tmp;
}

void centerTextW(wchar_t **text, int width) {
    wchar_t *tmp = centeredTextW(*text, width);
    free(*text);
    *text = tmp;
}

void printCommandTable(char *itemsPtr[], int len, float widthPercent) {
    char **items = (char**)calloc(len, sizeof(char*));
    for (int i = 0; i < len; ++i) {
        items[i] = (char*)calloc((strlen(itemsPtr[i]) + 1), sizeof(char));
        strcpy(items[i], itemsPtr[i]);
    }
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width, height;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int containerLength = 0;
    int minPadding = 2;
    for (int i = 0; i < len; ++i) {
        if (strlen(items[i]) + minPadding > containerLength) {
            containerLength = strlen(items[i]) + minPadding;
        }
    }
    float margin = 1 - widthPercent / 100.0;
    int containerPerLine = round(width * (1 - margin) / containerLength);
    int lineWidth = containerLength * (containerPerLine - 1) + containerPerLine;

    int leftPaddingLen = (width - lineWidth) / 2;

    if (len < containerPerLine - 1) containerPerLine = len + 1, lineWidth = containerLength * (containerPerLine - 1) + containerPerLine, leftPaddingLen = (width - lineWidth) / 2;

    putchar('\n');
    for (int y = 0; y < leftPaddingLen; ++y) putchar(' ');
    for (int y = 0; y < lineWidth; ++y) putchar('-');

    for (int i = 0; i < len; ++i) {
        if (i % (containerPerLine - 1) == 0) {
            putchar('\n');
            for (int y = 0; y < leftPaddingLen; ++y) putchar(' ');
            putchar('|');
        }
        char *name = items[i];
        centerText(&name, containerLength);
        printf("%s|", name);
        free(name);
    }
    int remainingContainers = len % (containerPerLine - 1);
    int bottomBorderLen = remainingContainers * containerLength + remainingContainers + 1;
    if (lineWidth - bottomBorderLen > 0 && bottomBorderLen > 1) {
        for (int y = 0; y < lineWidth - bottomBorderLen; ++y) putchar('-');
    }
    putchar('\n');
    for (int y = 0; y < leftPaddingLen; ++y) putchar(' ');
    for (int y = 0; y < bottomBorderLen; ++y) putchar('-');
    
    if (bottomBorderLen < 2 || lineWidth - bottomBorderLen < 0) for (int y = 0; y < lineWidth - bottomBorderLen; ++y) putchar('-');
    putchar('\n');
}

void printCommandTableW(wchar_t *itemsPtr[], int len, float widthPercent) {
    wchar_t **items = (wchar_t**)calloc(len, sizeof(wchar_t*));
    for (int i = 0; i < len; ++i) {
        items[i] = (wchar_t*)calloc((wcslen(itemsPtr[i]) + 1), sizeof(wchar_t));
        wcscpy(items[i], itemsPtr[i]);
    }
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width, height;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int containerLength = 0;
    int minPadding = 2;
    for (int i = 0; i < len; ++i) {
        if (wcslen(items[i]) + minPadding > containerLength) {
            containerLength = wcslen(items[i]) + minPadding;
        }
    }
    float margin = 1 - widthPercent / 100.0;
    int containerPerLine = round(width * (1 - margin) / containerLength);
    int lineWidth = containerLength * (containerPerLine - 1) + containerPerLine;

    int leftPaddingLen = (width - lineWidth) / 2;

    if (len < containerPerLine - 1) containerPerLine = len + 1, lineWidth = containerLength * (containerPerLine - 1) + containerPerLine, leftPaddingLen = (width - lineWidth) / 2;

    putchar('\n');
    for (int y = 0; y < leftPaddingLen; ++y) putchar(L' ');
    for (int y = 0; y < lineWidth; ++y) putchar(L'-');

    for (int i = 0; i < len; ++i) {
        if (i % (containerPerLine - 1) == 0) {
            putchar('\n');
            for (int y = 0; y < leftPaddingLen; ++y) putchar(L' ');
            putchar(L'|');
        }
        wchar_t *name = items[i];
        centerTextW(&name, containerLength);
        printfw2(name, L"|");
        free(name);
    }
    int remainingContainers = len % (containerPerLine - 1);
    int bottomBorderLen = remainingContainers * containerLength + remainingContainers + 1;
    if (lineWidth - bottomBorderLen > 0 && bottomBorderLen > 1) {
        for (int y = 0; y < lineWidth - bottomBorderLen; ++y) putchar(L'-');
    }
    putchar('\n');
    for (int y = 0; y < leftPaddingLen; ++y) putchar(L' ');
    for (int y = 0; y < bottomBorderLen; ++y) putchar(L'-');
    
    if (bottomBorderLen < 2 || lineWidth - bottomBorderLen < 0) for (int y = 0; y < lineWidth - bottomBorderLen; ++y) putchar(L'-');
    putchar('\n');
}
