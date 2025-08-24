# C modules
### My C single-header libraries
## Contents
### [printfw](#printfwh)
## The libs
### [printfw.h:](printfw.h)
- Windows only
- It actually prints in UTF-8, unlike wprintf is (for me)
- Stands for PrintfWide (printf because of c [screw formatting], wide because you need wide characters
#### Example:
```
#include <printfw.h>

int main() {
    printfw(L"Állat");  // "Animal" in Hungarian
    return 0;
}
```
#### Output:
```
Állat
```
#### Everything it adds:
- printfw(wchar_t *wstr) => prints out wide characters
- printfwl(wchar_t *wstr) => prints out wide characters and a new line (smart)
- putcharw(wchar_t c) => prints out a wide character
