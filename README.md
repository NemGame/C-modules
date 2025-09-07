# C modules
### My C single-header libraries
### I use gcc to compile most of my stuff
## Contents
### [printfw](#printfwh), [table](#tableh)
## The libs
## [printfw.h:](printfw.h)
- Windows only
- It actually prints in UTF-8, unlike wprintf is (for me)
- Stands for PrintfWide (printf because of c [screw formatting], wide because you need wide characters
### Everything it adds:
- printfw(wchar_t *wstr) => prints out wide characters
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
- printCenteredTable()
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
```
                        -----------------------------------------------------------------------
                        | Shadows |  have   |   no    |  speed  |   yet   |  they   |  still  |
                        |  move   |    .    |   now   |  this   |   is    |  just   | yapping |
                        | yipeee  |------------------------------------------------------------
                        -----------
```
