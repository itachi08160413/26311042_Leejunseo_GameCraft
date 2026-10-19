#pragma once
#include "glc2d.h"
#include "SceneGameBegin.h"
#include "SceneGamePlay.h"
#include "SceneGameResult.h"

enum class Screen { Title, Play, Result };

class CApplication
{
public:
    int Init();
    int Update();
    int Render();
    int Destroy();
    Screen m_screen = Screen::Title;
    SceneGameBegin m_sceneBegin;
    SceneGamePlay m_scenePlay;
    SceneGameResult m_sceneResult;
};
extern CApplication g_app;
