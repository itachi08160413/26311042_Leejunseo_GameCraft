#include "SceneGamePlay.h"
#include "glc2d.h"
#include <algorithm>
#include <cstdio>

void SceneGamePlay::Init()
{
    m_background = g2_TextureLoad("rsc/images/kitchen-alternative/background.png", 0);
    m_player = g2_TextureLoad("rsc/images/chef/player.png", 0);
    m_playerLeft = g2_TextureLoad("rsc/images/chef/player_left.png", 0);
    m_music = g2_SoundLoad("rsc/audio/music/Carefree.mp3");
    if (m_background < 0 || g2_TextureWidth(m_background) <= 0 || m_player < 0 || g2_TextureWidth(m_player) <= 0 || m_playerLeft < 0 || g2_TextureWidth(m_playerLeft) <= 0 || m_music < 0)
        std::fprintf(stderr, "Failed to load a game resource. Check the rsc folder.\n");
    if (m_music >= 0) g2_SoundPlay(m_music, true);
    m_lastTime = std::chrono::steady_clock::now();
}

void SceneGamePlay::Update()
{
    const auto now = std::chrono::steady_clock::now();
    const float dt = (std::min)(0.1f, std::chrono::duration<float>(now - m_lastTime).count());
    m_lastTime = now;
    if (GetForegroundWindow() != g2_GetHwnd()) return;
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) PostMessage(g2_GetHwnd(), WM_CLOSE, 0, 0);
    const int direction = ((GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0) - ((GetAsyncKeyState(VK_LEFT) & 0x8000) != 0);
    if (direction != 0) m_facingLeft = direction < 0;
    m_x = (std::max)(0.0f, (std::min)(1216.0f, m_x + direction * 300.0f * dt));
}

void SceneGamePlay::Render()
{
    const VEC2 backgroundPos{ 0, 0 }, backgroundScale{ 2, 2 };
    if (m_background >= 0) g2_Draw2D(m_background, nullptr, &backgroundPos, &backgroundScale, nullptr, 0, 0xFFFFFFFF);
    const VEC2 playerPos{ m_x, 480 }, playerScale{ 4, 4 };
    const int player = m_facingLeft ? m_playerLeft : m_player;
    if (player >= 0) g2_Draw2D(player, nullptr, &playerPos, &playerScale, nullptr, 0, 0xFFFFFFFF);
}

void SceneGamePlay::Destroy()
{
    if (m_music >= 0) { g2_SoundStop(m_music); g2_SoundRelease(m_music); }
    if (m_background >= 0) g2_TextureRelease(m_background);
    if (m_player >= 0) g2_TextureRelease(m_player);
    if (m_playerLeft >= 0) g2_TextureRelease(m_playerLeft);
}
