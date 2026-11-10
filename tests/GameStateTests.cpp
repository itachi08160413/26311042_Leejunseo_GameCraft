#include "../26311042_Leejunseo_GameCraft/GameState.h"
#include <cassert>
#include <cstdio>
int main() {
    GameState g; g.Reset(); g.pans={}; g.x=GameState::PanX(1)-32;
    Food f; f.active=true; g.pans[1]=f;
    assert(g.Act()==-1 && g.failures==1 && !g.pans[1].active);
    assert(g.Act()==0 && g.failures==1);
    f.age=4; g.pans[1]=f; assert(g.Act()==1);
    assert(g.Act()==0); g.Tick(0.3f); assert(g.pans[1].age==0);
    g.Tick(4); assert(g.Act()==2 && g.score==10 && g.completed==1);
    g.pans={}; f.age=5.99f; g.pans[1]=f; g.Tick(0.02f); assert(g.failures==2);
    f.thin=true; f.age=3; assert(f.Ready()); f.age=4.5f; assert(!f.Ready());
    g.elapsed=30; assert(g.Enabled(0) && g.Interval()==3);
    g.elapsed=60; assert(g.Interval()==2.5f);
    for(auto& food:g.pans) food=f; g.Spawn(); for(auto& food:g.pans) assert(food.age==4.5f);
    g.pans={}; g.elapsed=89.9f; g.score=200; g.Tick(0.2f); assert(g.Won());
    const float elapsed=g.elapsed; g.Tick(1); assert(g.elapsed==elapsed && g.Act()==0);
    g.Reset(); assert(!g.finished && g.score==0 && g.failures==0 && g.completed==0 && g.elapsed==0);
    g.pans={}; g.failures=4; f.thin=false; f.age=0; g.pans[1]=f; g.x=GameState::PanX(1)-32;
    assert(g.Act()==-1 && g.finished && !g.Won());
    puts("GameState: all checks passed");
}
