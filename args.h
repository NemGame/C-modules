#ifndef ARGS_H
#define ARGS_H

#define MAX_ATTRS 20
#define MAX_OUT 20

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef struct {
    char *name;
    char *value;
} attr;

typedef struct {
    wchar_t *name;
    wchar_t *value;
} attrw;

void printListAttrW(attrw *array, int size) {
    if (size <= 0) return;
    for (int i = 0; i < size; ++i) {
        wprintf(L"  -> %ls = %ls\n", array[i].name, (array[i].value != NULL) ? array[i].value : L"(NULL)");
    }
}

int prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

int prefixW(const wchar_t *pre, const wchar_t *str)
{
    return wcsncmp(pre, str, wcslen(pre)) == 0;
}

void printListAttr(attr *array, int size) {
    if (size <= 0) return;
    for (int i = 0; i < size; ++i) {
        printf("  -> %s = %s\n", array[i].name, (array[i].value != NULL) ? array[i].value : "(NULL)");
    }
}

int listLenNull(char **array) {
    int l = 0;
    for (int i = 0; array[i] != NULL; ++i) ++l;
    return l;
}

int listLenNullW(wchar_t **array) {
    int l = 0;
    for (int i = 0; array[i] != NULL; ++i) ++l;
    return l;
}

int listLenNullAttr(attr *array) {
    int l = 0;
    for (int i = 0; array[i].name != NULL; ++i) ++l;
    return l;
}

int listLenNullAttrW(attrw *array) {
    int l = 0;
    for (int i = 0; array[i].name != NULL; ++i) ++l;
    return l;
}

/**
 * @brief Processes args
 *
 * @param argc length of args
 * @param args argv
 * @param attributes attr array[] 
 * @param attrLen length of attributes
 * @param switchAttributes char *array[]
 * @param swAttrLen length of switchAttributes
 * @param OUTaddedAttributes the switches with were found
 * @param OUTaddedAttributesCounter length of OUTaddedAttributes
 * @param OUTnoidea unknown args
 * @param OUTnoideaCounter length of OUTnoidea
 * @param debugMode You got 2 guesses
 */
void processArgs(
    int argc, 
    char *args[], 
    attr attributes[], 
    int attrLen,
    char* switchAttributes[], 
    int swAttrLen,
    char** OUTaddedAttributes, 
    int *OUTaddedAttributesCounter,
    char** OUTnoidea, 
    int *OUTnoideaCounter,
    int debugMode
) {
    int noidea_count = 0;
    int added_count = 0;
    char *saved = NULL;

    for (int i = 1; i < argc; ++i) {
        char *arg = args[i];
        int found = 0;
        if (saved != NULL) {
            for (int j = 0; j < attrLen; ++j) {
                if (strcmp(saved, attributes[j].name) == 0) {
                    attributes[j].value = arg;
                    if (debugMode) printf("[DEBUG] Érték '%s' hozzáadva a '%s' attribútumhoz\n", arg, attributes[j].name);
                    found = 1;
                    saved = NULL;
                    break;
                }
            }
        }
        if (found) continue;
        for (int j = 0; j < swAttrLen; ++j) {
            if (strcmp(arg, switchAttributes[j]) == 0) {
                if (added_count < *OUTaddedAttributesCounter) {
                    OUTaddedAttributes[added_count++] = arg;
                    if (debugMode) printf("[DEBUG] Kapcsoló: %s felismert\n", arg);
                }
                found = 1;
                saved = NULL;
                break;
            }
        }
        if (found) continue;
        for (int j = 0; j < attrLen; ++j) {
            attr attribute = attributes[j];
            if (strcmp(arg, attribute.name) == 0) {
                saved = attribute.name;
                if (debugMode) printf("[DEBUG] Várható érték a '%s' attribútumhoz\n", arg);
                found = 1;
                break;
            }
        }
        if (found) continue;
        if (noidea_count < *OUTnoideaCounter) {
            OUTnoidea[noidea_count++] = arg;
        }
    }

    *OUTaddedAttributesCounter = added_count;
    *OUTnoideaCounter = noidea_count;
    if (debugMode) {
        printf("\n=== Feldolgozás vége (DEBUG) ===\n");
    }
}

char *valueOfAttr(attr attributes[], char *name) {
    for (int i = 0; attributes[i].name != NULL; ++i) {
        if (strcmp(attributes[i].name, name) == 0) {
            return attributes[i].value;
        }
    }
    return NULL;
}

wchar_t *valueOfAttrW(attrw attributes[], wchar_t *name) {
    for (int i = 0; attributes[i].name != NULL; ++i) {
        if (wcscmp(attributes[i].name, name) == 0) {
            return attributes[i].value;
        }
    }
    return NULL;
}

void processArgsA(int argc, char *argv[], attr attributes[], char **addedAttributes, char **noidea) {
    char *swi = "-";
    char *dat = "--";
    int addedCount = 0;
    int noideaCount = 0;
    int attrCount = 0;
    for (int i = 0; i < argc; ++i) {
        char *arg = argv[i];
        int found = 0;
        if (prefix(dat, arg)) {
            attributes[attrCount].name = arg;
            if (i + 1 < argc) {
                attributes[attrCount].value = argv[i + 1];
                ++i;
            } else attributes[attrCount].value = NULL;
            ++attrCount;
            continue;
        }
        if (prefix(swi, arg)) {
            addedAttributes[addedCount++] = arg;
            continue;
        }
        noidea[noideaCount++] = arg;
    }
    attributes[attrCount].name = NULL;
    addedAttributes[addedCount] = NULL;
    noidea[noideaCount] = NULL;
}

void processArgsAW(int argc, wchar_t **argv, attrw attributes[], wchar_t **addedAttributes, wchar_t **noidea, int debugMode) {
    wchar_t *swi = L"-";
    wchar_t *dat = L"--";
    int addedCount = 0;
    int noideaCount = 0;
    int attrCount = 0;
    for (int i = 0; i < argc; ++i) {
        wchar_t *arg = argv[i];
        if (prefixW(dat, arg)) {
            attributes[attrCount].name = arg;
            if (i + 1 < argc) {
                attributes[attrCount].value = argv[i + 1];
                ++i;
            } else attributes[attrCount].value = L"";
            if (debugMode) printf("Added attr with name \"%ls\" and value \"%ls\"\n", attributes[attrCount].name, attributes[attrCount].value);
            ++attrCount;
            continue;
        }
        if (prefixW(swi, arg)) {
            addedAttributes[addedCount++] = arg;
            if (debugMode) printf("Added switch \"%ls\"\n", arg);
            continue;
        }
        noidea[noideaCount++] = arg;
        if (debugMode) printf("Added noidea \"%ls\"\n", arg);
    }
    attributes[attrCount].name = NULL;
    addedAttributes[addedCount] = NULL;
    noidea[noideaCount] = NULL;
}

#endif

