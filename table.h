#pragma once

/**
 * table.h
 */

#include <stdio.h>
#include <windows.h>
#include <math.h>

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

void centerText(char **text, int width) {
    char *tmp = centeredText(*text, width);
    free(*text);
    *text = tmp;
}

void printCenteredTable(char *items[], int len, float widthPercent) {
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
