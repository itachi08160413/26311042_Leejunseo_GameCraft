#include "CApplication.h"

CApplication g_app;

int main()
{
    g_app.Init();
    g2_Run();
    g_app.Destroy();
    return 0;
}
