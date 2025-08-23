# C modules
### My C single-header libraries
## The libs
### [printfw.h:](printfw.h)
- Windows only
- Adds the function printfw(wchar_t wstr)
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
