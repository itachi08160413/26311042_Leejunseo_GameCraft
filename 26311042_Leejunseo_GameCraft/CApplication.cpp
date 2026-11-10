#include "CApplication.h"

namespace
{
    int UpdateAPP() { return g_app.Update(); }
    int RenderAPP() { return g_app.Render(); }
}

int CApplication::Init()
{
    g2_InitSdk();
    g2_CreateWin(100, 100, 1280, 720, "Pancake - GameCraft", true);
    g2_SetFrameMove(UpdateAPP);
    g2_SetRender(RenderAPP);
    g2_SetClearColor(0xFF101923);
    g2_SetStateShow(false);
    m_scenePlay.Init();
    // Music begins with the first round, not on the title screen.

    return 0;
}

int CApplication::Update()
{
    const bool enter = (GetAsyncKeyState(VK_RETURN) & 0x8000) != 0;
    if (GetForegroundWindow() == g2_GetHwnd()) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) PostMessage(g2_GetHwnd(), WM_CLOSE, 0, 0);
        if (enter && !m_enter && m_screen != Screen::Play) { m_scenePlay.Reset(); m_screen = Screen::Play; }
    }
    m_enter = enter;
    switch (m_screen)
    {
    case Screen::Title: m_sceneBegin.Update(); break;
    case Screen::Play: m_scenePlay.Update(); break;
    case Screen::Result: m_sceneResult.Update(); break;
    }
    return 0;
}

int CApplication::Render()
{
    switch (m_screen)
    {
    case Screen::Title: m_sceneBegin.Render(); break;
    case Screen::Play: m_scenePlay.Render(); break;
    case Screen::Result: m_sceneResult.Render(); break;
    }
    return 0;
}

int CApplication::Destroy()
{
    m_scenePlay.Destroy();
    g2_DestroyWin();
    return 0;
}
