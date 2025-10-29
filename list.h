#pragma once
/**
 * list.h
*/
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <my/printfw.h>

void printList(char **list, int size, char *sep) {
    if (!list || size == 0 || !sep) return;
    for (int i = 0; i < size - 1; i++) {
        printf("%s%s", list[i], sep);
    }
    printf("%s", list[size - 1]);
}

void printListW(wchar_t **list, int size, wchar_t *sep) {
    if (!list || size == 0 || !sep) return;
    for (int i = 0; i < size - 1; i++) {
        wchar_t *out = _wcsdup(list[i]);
        wcscat(out, sep);
        printfw(list[i]);
        free(out);
    }
    wchar_t *out = _wcsdup(list[size - 1]);
    wcscat(out, sep);
    printfw(list[size - 1]);
    free(out);
}

void freeList(char **list, int size) {
    if (!list || size == 0) return;
    for (int i = 0; i < size; i++) {
        free(list[i]);
    }
    free(list);
}

void freeListW(wchar_t **list, int size) {
    if (!list || size == 0) return;
    for (int i = 0; i < size; i++) {
        free(list[i]);
    }
    free(list);
}