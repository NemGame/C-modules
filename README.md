# C modules
### My C single-header libraries
### I use gcc to compile most of my stuff
## Contents
### [printfw](#printfwh), [table](#tableh), [args](#argsh)
## The libs
## [printfw.h:](printfw.h)
- Windows only
- It actually prints in UTF-8, unlike wprintf is (for me)
- Stands for PrintfWide (printf because of c [screw formatting], wide because you need wide characters)
### Everything it adds:
- printfw(wchar_t *wstr) => prints out wide characters
- printfw2(wchar_t *wstr, wchar_t *wstr2) => prints out wide characters, faster than individually calling printfw for each one
- printfw3(wchar_t *wstr, wchar_t *wstr2, wchar_t *wstr3) => prints out wide characters, faster than individually calling printfw for each one
- printfwallj(wchar_t *wstrs[], int count, wchar_t *separator) => prints out wide characters, faster than individually calling printfw for each one
- printfwall(wchar_t *wstrs[], int count) => prints out wide characters, faster than individually calling printfw for each one, separator is \n
- printfwl(wchar_t *wstr) => prints out wide characters and a new line (smart)
- putcharw(wchar_t c) => prints out a wide character
### Example:
```c
#include <printfw.h>

int main() {
    printfw(L"Állat");  // "Animal" in Hungarian
    return 0;
}
```
### Output:
```
Állat
```
## [table.h](table.h)
- Windows only
- It prints the given element into a centered table-like structure
### Everything it adds:
- printCenteredTable(char *items[], int len, float widthPercent) => does the above mentioned stuff, returns void
- printCenteredTableW(wchar_t *items[], int len, float widthPercent) => does the above mentioned stuff, returns void
- centeredText(char *text, int width) => returns the text as another text, but centered inside width amount of spaces, you need to free it later on
- centeredTextW(wchar_t *text, int width) => returns the text as another text, but centered inside width amount of spaces, you need to free it later on
- centerText(char **text, int width) => throws out the contents of text, then replaces it with a centered version of it, you need to free it later on, returns void
- centerTextW(wchar_t **text, int width) => throws out the contents of text, then replaces it with a centered version of it, you need to free it later on, returns void
### Example:
```c
#include "table.h"

int main(int argc, char const *argv[])
{
    char *items[] = {
        "Shadows", "have", "no", "speed", "yet", "they", "still", "move", ".", "now", "this", "is", "just", "yapping", "yipeee"
    };
    int itemCount = sizeof(items) / sizeof(items[0]);
    printCenteredTable(items, itemCount, 60);
    return 0;
}
```
### Output: (terminal width is 119)
```terminal
                        -----------------------------------------------------------------------
                        | Shadows |  have   |   no    |  speed  |   yet   |  they   |  still  |
                        |  move   |    .    |   now   |  this   |   is    |  just   | yapping |
                        | yipeee  |------------------------------------------------------------
                        -----------
```
## [args.h](args.h)
- makes the arguments easier to work with
### Everything it adds:
- 2 defined: MAX_ATTRS, MAX_OUT, both are 20
- 2 typedef: attr, attrw
- prefix(const char *pre, const char *str) => returns 1 if `str` starts with `pre`
- prefixW(const wchar_t *pre, const wchar_t *str) => `prefix`, but with wide characters
- printListAttr(attr *array, int size) => prints out a list of `attr`s
- listLenNull(char **array) => lenth of a `char**` with NULL as the last element
- listLenNullW(wchar_t **array) => lenth of a `wchar_t**` with NULL as the last element
- listLenNullAttr(attr *array) => lenth of a `attr *` with .name = NULL as the last element
- processArgs => Processes args
```c
processArgs(int argc, // length of args
char *args[], // argv
attr attributes[], // attr array[] 
int attrLen, // length of attributes
char* switchAttributes[], // char *array[]
int swAttrLen, // length of switchAttributes
char** OUTaddedAttributes, // the switches with were found
int *OUTaddedAttributesCounter, // length of OUTaddedAttributes
char** OUTnoidea, // unknown args
int *OUTnoideaCounter, // length of OUTnoidea
int debugMode // You got 2 guesses
)
```
- valueOfAttr(attr attributes[], char *name) => if .name is in `attributes`, .value is return, otherwise NULL
- valueOfAttrW(attrw attributes[], wchar_t *name) => if .name is in `attributes`, .value is return, otherwise NULL
- processArgsA => Processes args automatically
```c
processArgsA(int argc,  // length of argv
char *argv[], // argv
attr attributes[], // "--foo bar" => .name = foo, .value = bar
char **addedAttributes, // "-yay" => "-yay"
char **noidea // "bruh" => "bruh"
)
```
- processArgsAW => The wide character version `processArgsA`
### Example:
```c
#include <stdio.h>
#include <stdlib.h>
#include <my/args.h>
#include <windows.h>

void printList(char **list, int count) {
    for (int i = 0; i < count; ++i) printf("%s\n", list[i]);
}

void printListW(wchar_t **list, int count) {
    for (int i = 0; i < count; ++i) {
        wprintf(L"%ls\n", list[i]);
    }
}

int wmain(int argc, wchar_t *argv[]) {
    attrw attributes3[MAX_ATTRS];
    wchar_t *added3[MAX_OUT] = {NULL};
    wchar_t *noidea3[MAX_OUT] = {NULL};

    processArgsAW(argc, argv, attributes3, added3, noidea3, 0);

    printf("Added:\n");
    printListW(added3, listLenNullW(added3));
    printf("Noidea:\n");
    printListW(noidea3, listLenNullW(noidea3));
    printf("Attributes2:\n");
    printListAttrW(attributes3, listLenNullAttrW(attributes3));

    return 0;
}

int main(int argc, char **argv) {
    LPWSTR *args = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (args == NULL) return 1;
    int res = wmain(argc, args);
    LocalFree(args);

    return res;
}
```
### Terminal: `.\q --file wasd -wasd ye`
### Output:
```
Added:
-wasd
Noidea:
C:\MyCommands\test\args.h\q.exe
ye
Attributes2:
  -> --file = wasd
```
