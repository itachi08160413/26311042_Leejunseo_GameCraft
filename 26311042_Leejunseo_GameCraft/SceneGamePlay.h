#pragma once
#include "GameState.h"
#include <chrono>
class SceneGamePlay
{
public:
    void Init(); void Destroy(); void Render(); void Update(); void Reset();
    void Text(int x, int y, const char* text, unsigned long color = 0xFFFFFFFF, int size = 24);
    void Box(float x, float y, float w, float h, unsigned long color);
    void Panel(float x, float y, float w, float h, unsigned long color = 0xFF352A30);
    void Backdrop();
    GameState game;
private:
    int m_titleFont=-1, m_smallFont=-1;
    int m_background=-1, m_player=-1, m_playerLeft=-1, m_music=-1;
    int m_white=-1, m_pan=-1, m_raw=-1, m_cooked=-1, m_font=-1;
    int m_flipSound=-1, m_successSound=-1, m_failSound=-1;
    bool m_facingLeft=false, m_space=false, m_moving=false;
    float m_feedbackTime=0;
    const char* m_feedback="";
    std::chrono::steady_clock::time_point m_lastTime;
    void Image(int id, float x, float y, float w, float h, unsigned long color=0xFFFFFFFF);
    void Feedback(int event);
};
