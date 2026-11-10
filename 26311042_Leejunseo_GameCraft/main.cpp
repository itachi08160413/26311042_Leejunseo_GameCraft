#include "CApplication.h"
#include <cwchar>
CApplication g_app;
int main()
{
    wchar_t path[32768]{};
    if (GetModuleFileNameW(nullptr,path,32768)) {
        wchar_t* slash = std::wcsrchr(path, L'\\');
        if (slash) { *slash = L'\0'; if (!SetCurrentDirectoryW(path)) return 1; }
    }
    g_app.Init();
    g2_Run();
    g_app.Destroy();
    return 0;
}
