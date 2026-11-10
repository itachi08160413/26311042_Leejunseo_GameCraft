#include "CApplication.h"
void SceneGameBegin::Render() {
    auto& v=g_app.m_scenePlay;
    v.Backdrop();
    v.Text(90,66,"FRESH OFF THE GRIDDLE",0xFFE9B678,18);
    v.Text(86,101,"PANCAKE CLUB",0xFFFFE6BE,52);
    v.Text(90,181,"팬케이크 주방 · 90초의 달콤한 도전",0xFFF0D4BB);
    v.Panel(90,250,655,260);
    v.Text(119,270,"오늘의 레시피",0xFFFFD99A);
    v.Text(119,322,"01   방향키로 팬 앞으로 이동",0xFFF8ECDD);
    v.Text(119,374,"02   초록색이 되면 SPACE로 뒤집기",0xFFA6EDB8);
    v.Text(119,426,"03   반대쪽도 익으면 SPACE로 완성!",0xFFF8ECDD);
    v.Panel(790,250,400,260,0xFF49342F);
    v.Text(823,274,"TODAY’S CHALLENGE",0xFFE9B678,18);
    v.Text(820,310,"200점",0xFFFFD99A,52);
    v.Text(823,391,"한 접시 +10점 / 기회 5번",0xFFF8ECDD);
    v.Text(823,446,"30초부터 모든 화구 OPEN",0xFFCBAB94,18);
    v.Panel(90,550,655,76,0xFF3C6451);
    v.Text(244,573,"ENTER   주방 오픈!",0xFFFFE6BE);
    v.Text(805,568,"TIP  초록색 구간을 노려 보세요!",0xFFFFD99A,18);
    v.Text(805,602,"너무 이르거나 타면 기회를 잃어요.",0xFFCBAB94,18);
    v.Text(90,664,"HANDMADE PANCAKES  /  GAMECRAFT",0xFFCBAB94,18);
    v.Text(1060,664,"ESC  나가기",0xFFCBAB94,18);
}
void SceneGameBegin::Update() {}
