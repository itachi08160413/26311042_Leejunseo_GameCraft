#include "CApplication.h"
#include <cstdio>
void SceneGameResult::Render() {
    auto& v=g_app.m_scenePlay; const auto& g=v.game;
    v.Backdrop();
    v.Text(452,49,"PANCAKE CLUB / KITCHEN REPORT",0xFFE9B678,18);
    v.Text(g.Won()?390:422,91,g.Won()?"오늘의 스타 셰프!":"오늘의 영업 종료",g.Won()?0xFFA6EDB8:0xFFFFE6BE,52);
    v.Panel(290,197,700,318);
    v.Text(332,222,g.Won()?"MISSION CLEAR  /  목표 달성":"SHIFT COMPLETE  /  다시 도전해요",0xFFE9B678,18);
    char line[160]; std::snprintf(line,sizeof(line),"%03d",g.score);
    v.Text(330,257,line,0xFFFFD99A,52);
    v.Text(545,287,"/ 200점",0xFFCBAB94);
    v.Box(332,350,616,2,0xFF785846);
    std::snprintf(line,sizeof(line),"완성한 팬케이크                         %d 접시",g.completed);
    v.Text(332,374,line,0xFFF8ECDD);
    std::snprintf(line,sizeof(line),"놓친 주문                                  %d / 5",g.failures);
    v.Text(332,432,line,0xFFF8ECDD);
    v.Text(300,533,g.Won()?"완벽한 한 판! 다음 영업도 부탁해요.":g.failures>=5?"다음엔 초록색 타이밍에 맞춰 뒤집어 보세요.":"한 접시씩 차근차근, 다음엔 200점에 도전!",0xFFF0D4BB);
    v.Panel(380,588,520,70,0xFF3C6451);
    v.Text(468,608,"ENTER   다시 주방으로",0xFFFFE6BE);
    v.Text(1060,672,"ESC  나가기",0xFFCBAB94,18);
}
void SceneGameResult::Update() {}
