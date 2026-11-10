#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <random>

struct Food
{
    bool active = false, thin = false, second = false;
    float age = 0, flip = 0;
    float ReadyTime() const { return thin ? 3.0f : 4.0f; }
    float BurnTime() const { return thin ? 4.5f : 6.0f; }
    bool Ready() const { return active && flip <= 0 && age >= ReadyTime() && age < BurnTime(); }
};
struct GameState
{
    std::array<Food, 4> pans{};
    float elapsed = 0, spawn = 0, x = 600;
    int score = 0, failures = 0, completed = 0;
    bool finished = false;
    std::mt19937 random{ std::random_device{}() };
    int Stage() const { return elapsed < 30 ? 0 : elapsed < 60 ? 1 : 2; }
    bool Enabled(int i) const { return Stage() > 0 || i == 1 || i == 2; }
    float Interval() const { return Stage() == 0 ? 4.0f : Stage() == 1 ? 3.0f : 2.5f; }
    static float PanX(int i) { return 280.0f + i * 240.0f; }
    int Selected() const {
        for (int i = 0; i < 4; ++i) if (Enabled(i) && std::fabs(x + 32 - PanX(i)) <= 80) return i;
        return -1;
    }
    void Spawn() {
        std::array<int, 4> empty{}; int n = 0;
        for (int i = 0; i < 4; ++i) if (Enabled(i) && !pans[i].active) empty[n++] = i;
        if (!n) return;
        Food food; food.active = true; food.thin = Stage() > 0 && random() % 2 == 0;
        pans[empty[random() % n]] = food;
    }
    void Reset() { pans = {}; elapsed = spawn = 0; x = 600; score = failures = completed = 0; finished = false; Spawn(); }
    int Act() {
        if (finished) return 0;
        const int i = Selected(); if (i < 0) return 0;
        Food& f = pans[i]; if (!f.active || f.flip > 0) return 0;
        if (!f.Ready()) { f = {}; ++failures; finished = failures >= 5; return -1; }
        if (!f.second) { f.second = true; f.age = 0; f.flip = 0.3f; return 1; }
        f = {}; score += 10; ++completed; return 2;
    }
    void Tick(float dt) {
        if (finished || dt <= 0) return;
        dt = (std::min)(dt, 90.0f - elapsed);
        for (Food& f : pans) {
            if (!f.active) continue;
            float cooking = dt;
            if (f.flip > 0) { cooking = (std::max)(0.0f, dt - f.flip); f.flip = (std::max)(0.0f, f.flip - dt); }
            f.age += cooking;
            // The round boundary discards remaining food without adding failures.
            if (f.age >= f.BurnTime() && elapsed + dt < 90) { f = {}; ++failures; }
        }
        elapsed += dt;
        if (failures >= 5 || elapsed >= 90) { finished = true; return; }
        spawn += dt;
        while (spawn >= Interval()) { spawn -= Interval(); Spawn(); }
    }
    bool Won() const { return finished && elapsed >= 90 && failures < 5 && score >= 200; }
};
