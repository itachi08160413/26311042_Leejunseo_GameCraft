#pragma once
#include <chrono>

class SceneGamePlay
{
public:
    void Init();
    void Destroy();
    void Render();
    void Update();
private:
    int m_background = -1;
    int m_player = -1;
    int m_playerLeft = -1;
    int m_music = -1;
    float m_x = 600.0f;
    bool m_facingLeft = false;
    std::chrono::steady_clock::time_point m_lastTime;
};
