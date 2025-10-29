#ifndef ARGS_H
#define ARGS_H

#define MAX_ATTRS 20
#define MAX_OUT 20

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/**
 * @brief Egy kulcs-érték párt reprezentáló struktúra.
 */
typedef struct {
    char *name;
    char *value;
} attr;

typedef struct {
    wchar_t *name;
    wchar_t *value;
} attrw;

// Nem használt a kódban, de a leírásban szerepel:
// typedef int (*function)(char *);


int prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

int prefixW(const wchar_t *pre, const wchar_t *str)
{
    return wcsncmp(pre, str, wcslen(pre)) == 0;
}

/**
 * @brief Kiír egy attr struktúra tömböt.
 */
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

/**
 * @brief Parancssori argumentumok feldolgozása.
 *
 * @param argc A bemeneti args tömb mérete.
 * @param args A parancssori argumentumok.
 * @param attributes Az értékkel rendelkező (pl. --file <név>) attribútumok tömbje.
 * @param attrLen Az attributes tömb mérete.
 * @param switchAttributes A kapcsoló (pl. -v, --debug) attribútumok tömbje.
 * @param swAttrLen A switchAttributes tömb mérete.
 * @param OUTaddedAttributes Az elfogadott kapcsolók (switch) mutatóinak tömbje.
 * @param OUTaddedAttributesCounter A felismert kapcsolók számának mutatója.
 * @param OUTnoidea Az ismeretlen argumentumok mutatóinak tömbje.
 * @param OUTnoideaCounter Az ismeretlen argumentumok számának mutatója.
 * @param debugMode Ha debug mód be van kapcsolva.
 */
void processArgs(
    int argc, 
    char *args[], 
    attr attributes[], 
    int attrLen, // JAVÍTÁS: A tömb mérete külön paraméterként
    char* switchAttributes[], 
    int swAttrLen, // JAVÍTÁS: A tömb mérete külön paraméterként
    char** OUTaddedAttributes, 
    int *OUTaddedAttributesCounter, // JAVÍTÁS: Mutató a számlálóra
    char** OUTnoidea, 
    int *OUTnoideaCounter, // JAVÍTÁS: Mutató a számlálóra
    int debugMode
) {
    // A számlálókat nullázzuk a hívó oldalon, de itt használjuk a pointereket.
    int noidea_count = 0;
    int added_count = 0;
    char *saved = NULL; // "" helyett NULL, vagy a hívó oldalnak kell az első elemeket NULL-ra inicializálnia.

    for (int i = 1; i < argc; ++i) { // 0-tól indulva az args[0] maga a program neve
        char *arg = args[i];
        int found = 0;

        // 1. Érték hozzárendelése az előzőleg talált attribútumhoz
        if (saved != NULL) {
            for (int j = 0; j < attrLen; ++j) {
                if (strcmp(saved, attributes[j].name) == 0) {
                    attributes[j].value = arg;
                    if (debugMode) printf("[DEBUG] Érték '%s' hozzáadva a '%s' attribútumhoz\n", arg, attributes[j].name);
                    found = 1;
                    saved = NULL; // Visszaállítjuk
                    break;
                }
            }
        }
        if (found) continue;

        // 2. Kapcsoló (Switch) keresése
        for (int j = 0; j < swAttrLen; ++j) {
            if (strcmp(arg, switchAttributes[j]) == 0) {
                // Helyes mutató kezelés a kimeneti tömbben
                if (added_count < *OUTaddedAttributesCounter) { // Ellenőrzés a túlcsordulás ellen
                    OUTaddedAttributes[added_count++] = arg;
                    if (debugMode) printf("[DEBUG] Kapcsoló: %s felismert\n", arg);
                }
                found = 1;
                saved = NULL;
                break;
            }
        }
        if (found) continue;

        // 3. Értékre váró attribútum keresése (következő elem lesz az érték)
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

        // 4. Nem felismert attribútum
        if (noidea_count < *OUTnoideaCounter) { // Ellenőrzés a túlcsordulás ellen
            OUTnoidea[noidea_count++] = arg;
        }
    }

    // A hívó oldalon lévő számlálók frissítése
    *OUTaddedAttributesCounter = added_count;
    *OUTnoideaCounter = noidea_count;
    
    // A kimeneti listák kiírása a hívó kód elé
    if (debugMode) {
        printf("\n=== Feldolgozás vége (DEBUG) ===\n");
    }
}

char *valueOfAttr(attr attributes[], char *name) {
    for (int i = 0; attributes[i].name != NULL; ++i) {
        if (strcmp(attributes[i].name, name) == 0) {
            return attributes[i].value != NULL;
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
            } else attributes[attrCount].value = "";
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