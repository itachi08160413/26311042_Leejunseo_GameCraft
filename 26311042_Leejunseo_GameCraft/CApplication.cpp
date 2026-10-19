#include "CApplication.h"

namespace
{
    int UpdateAPP() { return g_app.Update(); }
    int RenderAPP() { return g_app.Render(); }
}

int CApplication::Init()
{
    g2_InitSdk();
    g2_CreateWin(100, 100, 1280, 720, "Game Project", true);
    g2_SetFrameMove(UpdateAPP);
    g2_SetRender(RenderAPP);
    g2_SetClearColor(0xFF101923);
    g2_SetStateShow(false);
    return 0;
}

int CApplication::Update()
{
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
    g2_DestroyWin();
    return 0;
}
