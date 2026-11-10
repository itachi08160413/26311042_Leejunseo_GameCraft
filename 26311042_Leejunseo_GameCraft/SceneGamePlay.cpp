#include "CApplication.h"
#include <cstdio>
#include <cmath>
#include <string>

void SceneGamePlay::Image(int id,float x,float y,float w,float h,unsigned long color) {
    if(id<0 || g2_TextureWidth(id)<=0 || g2_TextureHeight(id)<=0) return;
    const VEC2 pos{x,y}, scale{w/g2_TextureWidth(id),h/g2_TextureHeight(id)};
    g2_Draw2D(id,nullptr,&pos,&scale,nullptr,0,color);
}
void SceneGamePlay::Box(float x,float y,float w,float h,unsigned long color) { Image(m_white,x,y,w,h,color); }
void SceneGamePlay::Text(int x,int y,const char* text,unsigned long color,int size) {
    // glc2d uses the Windows ANSI text API; source strings are UTF-8.
    const int count=MultiByteToWideChar(CP_UTF8,0,text,-1,nullptr,0);
    if(count<=0) return;
    std::wstring wide(count,L'\0');
    MultiByteToWideChar(CP_UTF8,0,text,-1,&wide[0],count);
    const int bytes=WideCharToMultiByte(CP_ACP,0,wide.c_str(),-1,nullptr,0,nullptr,nullptr);
    if(bytes<=0) return;
    std::string local(bytes,'\0');
    WideCharToMultiByte(CP_ACP,0,wide.c_str(),-1,&local[0],bytes,nullptr,nullptr);
    RECT rc{x,y,1260,y+80}; g2_FontDrawText(size>=40?m_titleFont:size<24?m_smallFont:m_font,rc,color,"%s",local.c_str());
}
void SceneGamePlay::Panel(float x,float y,float w,float h,unsigned long color) {
    Box(x+5,y+7,w,h,0x70000000);
    Box(x,y,w,h,0xFFAA7955);
    Box(x+2,y+2,w-4,h-4,color);
    Box(x+2,y+2,w-4,3,0xFFDBAC76);
}
void SceneGamePlay::Backdrop() {
    Image(m_background,0,0,1280,720);
    Box(0,0,1280,720,0xDA211C27);
    for(int x=0;x<1280;x+=80) Box((float)x,0,40,9,0xFFDC9C62);
    Box(0,711,1280,9,0xFFDC9C62);
}
void SceneGamePlay::Init() {
    m_background=g2_TextureLoad("rsc/images/kitchen-alternative/background.png",0);
    m_player=g2_TextureLoad("rsc/images/chef/player.png",0);
    m_playerLeft=g2_TextureLoad("rsc/images/chef/player_left.png",0);
    m_white=g2_TextureLoad("rsc/images/ui/white.bmp",0);
    m_pan=g2_TextureLoad("rsc/images/kitchen/pack/Container/4pan.png",0);
    m_raw=g2_TextureLoad("rsc/images/kitchen/pack/Ingredient/food/9pancake.png",0);
    m_cooked=g2_TextureLoad("rsc/images/kitchen/pack/Ingredient/food/9pancake-cooked.png",0);
    m_font=g2_FontCreate("Malgun Gothic",24);
    m_titleFont=g2_FontCreate("Malgun Gothic",52);
    m_smallFont=g2_FontCreate("Malgun Gothic",18);
    m_music=g2_SoundLoad("rsc/audio/music/Carefree.mp3");
    m_flipSound=g2_SoundLoad("rsc/audio/interface-sounds/Audio/click_001.wav");
    m_successSound=g2_SoundLoad("rsc/audio/interface-sounds/Audio/confirmation_001.wav");
    m_failSound=g2_SoundLoad("rsc/audio/interface-sounds/Audio/back_001.wav");
    for(int id : {m_background,m_player,m_playerLeft,m_white,m_pan,m_raw,m_cooked})
        if(id<0 || g2_TextureWidth(id)<=0) std::fprintf(stderr,"Missing game texture: check rsc folder.\n");
    game.Reset(); m_lastTime=std::chrono::steady_clock::now();
}
void SceneGamePlay::Reset() {
    game.Reset(); m_space=(GetAsyncKeyState(VK_SPACE)&0x8000)!=0;
    m_moving=false; m_facingLeft=false; m_feedbackTime=0;
    m_lastTime=std::chrono::steady_clock::now();
    if(m_music>=0) { g2_SoundStop(m_music); g2_SoundReset(m_music); g2_SoundPlay(m_music,true); }
}
void SceneGamePlay::Feedback(int event) {
    if(!event) return;
    m_feedback=event<0 ? "조리 실패!" : event==1 ? "잘 뒤집었어요! 반대쪽도 익혀 주세요." : "완성! +10점";
    m_feedbackTime=1.2f;
    const int sound=event<0 ? m_failSound : event==1 ? m_flipSound : m_successSound;
    if(sound>=0) { g2_SoundReset(sound); g2_SoundPlay(sound); }
}
void SceneGamePlay::Update() {
    const auto now=std::chrono::steady_clock::now();
    const float dt=std::chrono::duration<float>(now-m_lastTime).count(); m_lastTime=now;
    const bool space=(GetAsyncKeyState(VK_SPACE)&0x8000)!=0;
    if(GetForegroundWindow()!=g2_GetHwnd()) { m_space=space; return; }
    const int direction=((GetAsyncKeyState(VK_RIGHT)&0x8000)!=0)-((GetAsyncKeyState(VK_LEFT)&0x8000)!=0);
    m_moving=direction!=0; if(m_moving) m_facingLeft=direction<0;
    game.x=(std::max)(0.0f,(std::min)(1216.0f,game.x+direction*300.0f*dt));
    const int oldFailures=game.failures;
    game.Tick(dt); if(game.failures>oldFailures) { Feedback(-1); m_feedback="앗, 타버렸어요! 초록색일 때 조작해 주세요."; }
    if(space && !m_space && !game.finished) { const int event=game.Act(); Feedback(event); if(event<0) m_feedback="아직 덜 익었어요. 초록색이 될 때까지 기다려 주세요."; } m_space=space;
    m_feedbackTime=(std::max)(0.0f,m_feedbackTime-dt);
    if(game.finished) { if(m_music>=0) g2_SoundStop(m_music); g_app.m_screen=Screen::Result; }
}
void SceneGamePlay::Render() {
    Image(m_background,0,0,1280,720);
    Box(0,122,1280,538,0x55211921);
    Box(0,0,1280,122,0xFF29232B);
    Box(0,119,1280,3,0xFFE9B678);
    Text(28,16,"PANCAKE CLUB",0xFFFFD99A);
    Text(28,54,"오늘도 달콤한 도전!",0xFFCBAB94,18);
    char line[240];
    Panel(310,16,238,85); Panel(566,16,360,85); Panel(944,16,306,85);
    Text(328,23,"TIME / 남은 시간",0xFFCBAB94,18);
    std::snprintf(line,sizeof(line),"%02d 초",(int)std::ceil(90-game.elapsed));
    Text(328,50,line,game.elapsed>=75?0xFFFF8B7B:0xFFFFE8C5);
    Box(432,65,96,6,0xFF1E1922);
    Box(432,65,96*(std::max)(0.0f,1-game.elapsed/90),6,0xFFE9B678);
    Text(584,23,"SCORE / 목표 200점",0xFFCBAB94,18);
    std::snprintf(line,sizeof(line),"%03d",game.score); Text(584,49,line,0xFFFFD99A);
    Box(676,64,228,10,0xFF1E1922);
    Box(676,64,228*(std::min)(1.0f,game.score/200.0f),10,0xFF8FDCAC);
    Text(962,23,"CHANCES / 남은 기회",0xFFCBAB94,18);
    for(int i=0;i<5;++i) {
        Box(966+i*53.0f,58,37,22,i<5-game.failures?0xFFEBAD79:0xFF53424A);
        Box(971+i*53.0f,61,27,3,i<5-game.failures?0xFFFFDCAB:0xFF63505A);
    }
    Panel(140,143,1000,87);
    Text(165,151,m_feedbackTime>0?m_feedback:"주문이 들어왔어요! 노릇하게 구워 주세요.",0xFFFFD99A);
    const int selected=game.Selected();
    const char* hint="← → 이동해서 화구를 선택하세요. 초록색일 때 SPACE!";
    if(selected>=0) {
        const Food& f=game.pans[selected];
        hint=!f.active?"다음 주문을 기다리는 중이에요. 다른 팬도 확인해 보세요.":f.flip>0?"공중에서 뒤집는 중!":f.Ready()?(f.second?"[ SPACE ] 완성된 팬케이크 담기!":"[ SPACE ] 지금 뒤집으세요!"):"노릇노릇 익는 중… 초록색 구간까지 기다려 주세요.";
    }
    Text(165,191,hint,0xFFF8ECDD,18);
    Box(140,274,1000,219,0xFF573C36);
    Box(140,484,1000,12,0xFFE0AD77);
    Box(150,496,980,12,0xFF32232B);
    for(int i=0;i<4;++i) {
        const float x=GameState::PanX(i); const Food& f=game.pans[i];
        const bool enabled=game.Enabled(i), active=i==selected;
        Panel(x-100,259,200,216,active?0xFF654A3C:0xFF302A32);
        if(active) { Box(x-100,259,200,5,0xFFFFD99A); Text((int)x-64,483,"선택한 화구",0xFFFFE5AE,18); }
        std::snprintf(line,sizeof(line),"0%d  /  %s",i+1,!enabled?"LOCKED":f.active?(f.thin?"THIN":"CLASSIC"):"WAITING");
        Text((int)x-82,273,line,enabled?0xFFE9C39B:0xFF9D8D96,18);
        if(!enabled) {
            Text((int)x-56,331,"준비 중",0xFFAD9A9C);
            Text((int)x-73,382,"30초 후 오픈",0xFFAD9A9C,18); continue;
        }
        Box(x-66,319,132,80,0xFF1D1C25);
        if(f.active) for(int j=0;j<5;++j) {
            const float h=12+8*std::sin(game.elapsed*12+j*2);
            Box(x-42+j*19,396-h,12,h,0xFFFF9850);
        }
        Image(m_pan,x-64,310,128,100);
        if(f.active) {
            const float lift=f.flip>0?60*std::sin(f.flip/0.3f*3.14159265f):0;
            Image(f.Ready()?m_cooked:m_raw,x-36,330-lift,72,f.thin?42.0f:56.0f);
            Box(x-76,407,152,12,0xFF191921);
            const float readyX=x-76+152*f.ReadyTime()/f.BurnTime();
            Box(readyX,407,152*(1-f.ReadyTime()/f.BurnTime()),12,0xFF35694F);
            Box(x-76,407,152*(std::min)(1.0f,f.age/f.BurnTime()),12,f.Ready()?0xFF9BE5AD:0xFFE9B678);
            Box(readyX,404,2,18,0xFFFFF2DD);
            Text((int)x-74,432,f.flip>0?"뒤집는 중!":f.Ready()?(f.second?"SPACE : 완성!":"SPACE : 뒤집기!"):(f.second?"뒷면 굽는 중":"앞면 굽는 중"),f.Ready()?0xFFA6EDB8:0xFFF8ECDD,18);
        } else Text((int)x-66,431,"다음 주문 대기",0xFFB9A5A6,18);
    }
    const float bob=m_moving?3*std::sin(game.elapsed*22):0;
    Box(game.x+4,613,56,6,0x65000000);
    Image(m_facingLeft?m_playerLeft:m_player,game.x,490+bob,64,128);
    Box(0,650,1280,70,0xFF29232B); Box(0,650,1280,3,0xFFE9B678);
    std::snprintf(line,sizeof(line),"STAGE 0%d  /  완성 %d접시",game.Stage()+1,game.completed);
    Text(30,672,line,0xFFFFD99A);
    Text(460,677,"← → 이동    SPACE 뒤집기 · 완성",0xFFF8ECDD,18);
    Text(1055,677,"ESC  나가기",0xFFCBAB94,18);
}
void SceneGamePlay::Destroy() {
    for(int id : {m_music,m_flipSound,m_successSound,m_failSound}) if(id>=0) { g2_SoundStop(id); g2_SoundRelease(id); }
    for(int id : {m_background,m_player,m_playerLeft,m_white,m_pan,m_raw,m_cooked}) if(id>=0) g2_TextureRelease(id);
}
