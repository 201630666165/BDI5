#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <mmsystem.h>
using std::ios;
using std::ofstream;
using std::ifstream;
#pragma comment (lib,"winmm.lib")
#pragma comment (lib,"Msimg32.lib")
//预先进行宏定义，减少硬编码，方便代码修改
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 660
#define SQUIREUNITWIDTH 80
#define SQUIREUNITHEIGHT 60
#define  WINDOW_TITLE L"魔塔：拯救公主"
#define UP -1
#define DOWN -2
#define  BRONZEWGATE -3
#define  SILVERGATE -4
#define GOADGATE  -5
#define  GAINSWORD -23
#define  GAINBLUEJEWEL -24
#define  GAINREDJEWEL -25
#define  GAINSMALLBOTTLE -26
#define  GAINBIGBOTTLE -27
#define GAINMUTIPLEKEY -28
#define  GAINARMOR -29
#define  GAINRADAR -30
#define GAINSILVERSWORD -31
#define GAINSMALLFLY -32
#define GAINBULLION -33
#define GAINGOADARMOR -34
#define GAINHOLYWATER -35
#define GAINCOMPASS -36
#define  GAINCROSS -37
#define GAINSTEELARMOR -38
#define GAINSTEELSWORD -39
#define GAINBIGFLY -40
#define GAINSTARLIGHTSWORD -41
#define GAINSTARLIGHTARMOR -42
#define GAINHAMMER -43
#define GAINGODSWORD -44
#define GAINGODARMOR -45
#define MONSTERSPECIES 16
#define  NPCSPECIES 2
HDC g_hdc = NULL, g_mdc = NULL, g_bufdc = NULL, g_swichdc =NULL;
HBITMAP		g_hSprite[4] = { NULL }, bmp, g_bg, g_msgbg, g_hWall, g_hLava, g_hyg, g_hStar, g_hRoad, g_hUp, g_hDown, g_hKey, g_hGoadgate, g_hMonster[MONSTERSPECIES], g_hvs, g_hNPC[NPCSPECIES];//这里保存游戏所需的各个位图句柄
HBITMAP     g_hStone, g_hSilverGate,g_hSword,g_hBlueJewel,g_hRedJewel,g_hSmallBottle,g_hBigBottle,g_hMutipleKey,g_hArmor,g_hRadar,g_hSilverSword,g_hSmallFly,g_hBullion,g_hGoadArmor,g_hHolyWater,g_hCompass,g_hCross,g_hBigFly,g_hHammer,g_hGodSword,g_hGodArmor,g_hStart,g_hStartSelect,g_hGut,g_hGutbg,g_hEnd,bbmp;//这里保存游戏所需的各个位图句柄
DWORD g_tPre = 0, g_tNow = 0;//这里判断时间，每间隔一定时间执行一次Game_paint()函数，以实现动画效果
int g_PileNo = 0, g_maxpile = 0;//记载当前所在层数，和到过的最高层数
BOOL g_swiching = FALSE, g_swichup = TRUE, g_battle = FALSE, g_battlegain = FALSE, g_talking = FALSE, g_selecting = FALSE, g_gain = FALSE,g_seleckeydown = FALSE,g_radaring=FALSE,g_compassing=FALSE,g_RedNo=TRUE,g_BlackNo = TRUE,g_start=TRUE,g_gut=FALSE,g_end=FALSE,g_blood=FALSE,g_bgm=TRUE;//保存各个剧情的触发状态，楼层的转换状态，背景音乐的播放状态
int g_swich = 0, g_battletime = 0, g_battleobject = 0, g_battlegaintime = 0, g_talkingNo = 0, g_talkingtime = 0, g_gaintime = 0, g_gainobject = 0, g_selecttime = 0, g_seletobjectsNo = 0, g_selectofnow = 0, g_thieftalkingNo = 0, g_princessNo = 0, g_compass = 1, g_compasstime = 0, g_compassswichtime = 0, sum = 0, g_starttime = 0, g_startNo = 0, g_guttime = 0,g_bgmtime=0;//保存剧情，转换等已经过的时间单位数，以及人物相关剧情的阶段，当前战斗对象和选择对象
int g_maze[30][11][11] = { { { 1,1,1,1,1,1,1,1,2,2,2 },{ 3,3,3,3,3,3,1,1,1,2,2 },{ 3,3,3,3,3,3,3,1,2,2,2 },{ 3,3,3,3,3,3,3,1,1,2,2 },{ 3,3,3,3,3,3,3,1,0,2,2 },{ UP,0,0,0,0,0,0,BRONZEWGATE,1003,0,0 },{ 3,3,3,3,3,3,3,1,0,2,2 } ,{ 3,3,3,3,3,3,3,1,1,2,2 } ,{ 3,3,3,3,3,3,3,1,2,2,2 } ,{ 3,3,3,3,3,3,1,1,1,2,2 },{ 3,3,3,3,3,1,1,1,2,2,2 } },
{{UP,1,GAINSMALLBOTTLE,-20,1,-20,GAINBLUEJEWEL,1,0,GAINSMALLBOTTLE,GAINSMALLBOTTLE},{0,1,0,28,BRONZEWGATE,29,0,BRONZEWGATE,29,GAINBIGBOTTLE,GAINRADAR},{-20,1,28,GAINREDJEWEL,1,0,-21,1,0,-20,-20},{20,1,BRONZEWGATE,1,1,1,1,1,1,1,1},{21,1,0,0,0,0,0,0,1,-22,0},{20,1,1,1,1,BRONZEWGATE,1,0,GOADGATE,0,DOWN},{0,1,GAINSMALLBOTTLE,GAINSMALLBOTTLE,1,32,1,0,1,0,0},{0,1,-20,-20,1,20,1,0,1,1,1},{0,1,GAINSMALLBOTTLE,GAINSMALLBOTTLE,22,24,1,0,1,-20,-20}, {0,1,1,1,1,1,1,0,BRONZEWGATE,36,-20},{0,0,0,0,0,0,0,0,1,-21,-20}},
{{DOWN,0,0,0,0,0,0,0,0,0,UP},{1,1,1,1,1,1,4,1,1,1,1},{0,GAINBLUEJEWEL,-20,-20,0,BRONZEWGATE,0,BRONZEWGATE,-20,-20,GAINREDJEWEL},{61,1,1,1,1,1,0,1,1,1,1},{0,GAINBIGBOTTLE,-20,-20,0,1,0,1,GAINBIGBOTTLE,GAINBIGBOTTLE,GAINBIGBOTTLE},{1,1,1,1,0,BRONZEWGATE,0,SILVERGATE,GAINSMALLBOTTLE,GAINSMALLBOTTLE,GAINSMALLBOTTLE},{GAINREDJEWEL,GAINREDJEWEL,GAINREDJEWEL,1,0,1,1,1,1,1,1},{GAINBLUEJEWEL,GAINBLUEJEWEL,GAINBLUEJEWEL,1,BRONZEWGATE,1,0,0,0,0,1000},{-20,-20,-20,1,0,BRONZEWGATE,60,1,1,1,1},{-22,-21,60,BRONZEWGATE,0,1,0,0,0,0,1001},{1,1,1,1,1,1,1,1,1,1,1}} ,
{ {GAINSWORD,21,-20,1,0,20,20,0,1,1,DOWN},{21,-20,28,BRONZEWGATE,0,1,1,0,1,0,0},{-20,0,0,1,0,0,1,0,1,0,1},{1,1,1,1,1,24,1,0,1,0,1},{0,0,1,0,1,21,1,0,24,0,1},{1006,0,BRONZEWGATE,28,1,24,1,1,1,1,1},{0,0,1,0,0,0,0,1,21,GAINBLUEJEWEL,GAINREDJEWEL},{1,1,1,1,1,1,0,BRONZEWGATE,0,24,GAINBIGBOTTLE},{1,0,0,-20,-20,-20,0,1,21,-20,-20},{1,24,1,1,1,1,1,1,1,1,1},{1,0,0,21,24,21,0,0,0,0,UP}},
{{0,BRONZEWGATE,0,0,24,24,21,0,0,0,UP},{22,1,1,1,1,1,1,1,1,1,1},{0,BRONZEWGATE,0,28,GAINSMALLBOTTLE,GAINSMALLBOTTLE,0,0,0,0,0},{1,1,1,1,1,1,1,1,1,1,22},{0,0,1,25,GAINBLUEJEWEL,1,36,GAINREDJEWEL,1,-20,0},{1002,0,0,26,25,GOADGATE,40,36,SILVERGATE,0,0},{0,0,1,25,GAINBLUEJEWEL,1,36,GAINREDJEWEL,1,-20,0},{1,1,1,1,1,1,1,1,1,1,22},{0,BRONZEWGATE,0,28,GAINSMALLBOTTLE,GAINSMALLBOTTLE ,0,0,0,0,0},{22,1,1,1,1,1,1,1,1,1,1},{0,BRONZEWGATE,0,0,24,24,21,0,0,0,DOWN}},
{{GAINMUTIPLEKEY,0,25,0,25,GAINREDJEWEL,GAINBLUEJEWEL,0,1,0,DOWN},{1,1,1,BRONZEWGATE,1,1,1,1000,1,0,0},{GAINSMALLBOTTLE,GAINREDJEWEL,0,32,0,0,1,1,1,1,24},{1,1,1,1,1,0,22,22,24,0,0},{GAINBIGBOTTLE,32,29,GAINARMOR,1,0,1,1,1,1,0},{32,0,0,29,1,24,1,0,BRONZEWGATE,24,0},{0,0,1,1,1,28,1,0,1,1,1},{0,0,1,0,0,0,1,0,SILVERGATE,GAINBLUEJEWEL,-20},{32,0,BRONZEWGATE,36,0,0,0,36,1,BRONZEWGATE,1},{-20,32,1,29,0,0,0,40,BRONZEWGATE,0,UP},{-21,-20,1,1001,29,0,0,0,1,1,1}},
{{GAINSMALLFLY,30,-20,0,1,0,0,0,0,0,0},{30,-20,26,0,1,0,1,1,1,1,0},{1,1,SILVERGATE,1,1,33,1,25,BRONZEWGATE,25,0},{GAINBLUEJEWEL,GAINREDJEWEL,0,40,GOADGATE,0,1,BRONZEWGATE,1,1,1},{1,1,SILVERGATE,1,1,-20,1,25,BRONZEWGATE,0,UP},{-20,0,26,0,1,-20,1,0,1,0,0},{23,-20,0,0,1,-20,1,0,1,1,BRONZEWGATE},{GAINBULLION,23,-20,0,1,0,1,0,1,1,BRONZEWGATE},{1,1,1,1,1,33,1,0,1,0,0},{GAINBIGBOTTLE,44,0,35,BRONZEWGATE,0,1,0,SILVERGATE,0,DOWN},{GAINBIGBOTTLE,GAINBIGBOTTLE,44,0,1,0,1,1,1,1,1}},
{{UP,1,1,0,0,0,0,0,0,0,0},{0,1,0,0,0,1,1,1,1,0,0},{0,0,26,1,SILVERGATE,1,GAINSMALLBOTTLE,-20,1,1,0},{0,26,GAINBLUEJEWEL,1,48,1,GAINREDJEWEL,GAINSMALLBOTTLE,-21,1,BRONZEWGATE},{0,1,1,1,0,1,1,1,-21,1,DOWN},{0,SILVERGATE,48,0,GAINCROSS,0,48,SILVERGATE,GAINBIGBOTTLE,GOADGATE,0},{0,1,1,1,0,1,1,1,-21,1,0},{0,30,GAINREDJEWEL,1,48,1,GAINBLUEJEWEL,GAINSMALLBOTTLE,-21,1,BRONZEWGATE},{1,0,30,1,SILVERGATE,1,GAINSMALLBOTTLE,-20,1,1,0},{1,1,0,0,0,1,1,1,1,0,0},{1,1,1,0,0,0,0,0,0,0,1}},
{{DOWN,0,0,0,25,26,25,0,0,0,0},{1,1,1,1,1,1,1,1,1,1,0},{0,0,0,0,0,GAINBLUEJEWEL,22,22,0,1,34},{0,1,1,1,1,1,1,1,30,1,0},{0,1,0,34,GAINSMALLBOTTLE,GAINSMALLBOTTLE,0,1,0,BRONZEWGATE,0},{0,BRONZEWGATE,0,1,1,1,0,1,1,1,1},{1,1,SILVERGATE,1,UP,1,0,40,30,0,0},{0,BRONZEWGATE,0,1,0,1,1,1,1,1,23},{-20,1,0,25,0,1,0,BRONZEWGATE,0,1,48},{30,1,1,1,1,1,26,1,0,1,23},{0,0,GAINREDJEWEL,22,22,0,0,1,0,0,0}},
{{GAINCOMPASS,-20,1,0,0,1,GAINBLUEJEWEL,1,30,-21,GAINSILVERSWORD},{-20,0,BRONZEWGATE,0,0,SILVERGATE,35,BRONZEWGATE,GAINSMALLBOTTLE,30,-21},{0,37,1,0,0,1,GAINREDJEWEL,1,30,GAINSMALLBOTTLE,30},{1,BRONZEWGATE,1,1,GOADGATE,1,1,1,1,1,BRONZEWGATE},{1,0,0,0,0,0,34,0,1,44,GAINBIGBOTTLE},{1,0,1,1,1,1,1,0,SILVERGATE,34,44},{0,0,1,0,DOWN,1,UP,0,1,44,GAINBIGBOTTLE},{0,1,1,0,1,1,1,BRONZEWGATE,1,1,1},{0},{1,BRONZEWGATE,1,1,1,1,1,1,1,BRONZEWGATE,1},{0,30,-20,-20,GAINSMALLBOTTLE,1,GAINSMALLBOTTLE,-20,-20,30,0}},
{{0,0,0,0,25,26,25,0,0,0,UP},{1,0,0,1,1,1,1,1,1,1,1},{1,1,0,0,0,0,0,1,GAINSMALLBOTTLE,GAINSMALLBOTTLE,GAINSMALLBOTTLE},{GAINBLUEJEWEL,1,0,1,0,1,0,1,GAINBLUEJEWEL,GAINBLUEJEWEL,GAINBLUEJEWEL},{37,BRONZEWGATE,0,1,-20,1,0,1,GAINREDJEWEL,GAINREDJEWEL,GAINREDJEWEL},{1,1,1,1,-20,1,DOWN,1,1,GOADGATE,1},{37,BRONZEWGATE,0,1,-20,1,1,1,0,34,-21},{GAINREDJEWEL,1,0,1,0,BRONZEWGATE,0,BRONZEWGATE,26,1,1},{1,1,0,0,0,1,BRONZEWGATE,1,0,34,-21},{1,0,35,1,1,1,26,1,1,1,1},{0,35,GAINBIGBOTTLE,1,-20,-20,0,0,-20,-20,GAINSMALLBOTTLE}},
{{GAINSMALLBOTTLE,GAINSMALLBOTTLE,GAINSMALLBOTTLE,BRONZEWGATE,0,BRONZEWGATE,GAINBLUEJEWEL,GAINBLUEJEWEL,GAINBLUEJEWEL,1,DOWN},{1,1,1,1,0,1,1,1,1,1,0},{-20,-20,-20,BRONZEWGATE,0,1,0,37,37,GOADGATE,0},{1,1,1,1,0,SILVERGATE,41,1,1,1,0},{-21,-21,-21,BRONZEWGATE,0,1,GAINBIGBOTTLE,1,0,GAINSMALLBOTTLE,0},{1,1,1,1,1,1,52,1,1006,0,0},{-22,-22,-22,BRONZEWGATE,0,1,GAINBIGBOTTLE,1,0,GAINSMALLBOTTLE,0},{1,1,1,1,0,SILVERGATE,41,1,1,1,0},{GAINBIGBOTTLE,41,0,1,0,1,0,37,37,GOADGATE,0},{GAINGOADARMOR,42,41,SILVERGATE,0,1,1,1,1,1,0},{GAINBIGBOTTLE,41,0,1,0,BRONZEWGATE,GAINREDJEWEL,GAINREDJEWEL,GAINREDJEWEL,1,UP}},
{{1001,GAINREDJEWEL,0,0,52,1,0,1,GAINBLUEJEWEL,1,UP},{GAINBLUEJEWEL,0,0,52,56,SILVERGATE,0,1,52,1,0},{1,1,1,1,1,1,0,1,BRONZEWGATE,1,0},{0,0,0,0,0,0,0,0,41,1,0},{60,1,1,1,1,1,1,1,41,1,0},{61,BRONZEWGATE,61,-20,-20,GAINSMALLBOTTLE,GAINSMALLBOTTLE,1,42,SILVERGATE,0},{60,1,1,1,1,1,1,1,41,1,0},{0,0,0,0,0,0,0,0,41,1,0},{1,1,1,1,1,1,0,1,BRONZEWGATE,1,0},{GAINBIGBOTTLE,0,0,64,68,SILVERGATE,0,1,52,1,0},{GAINHAMMER,GAINBIGBOTTLE,0,0,64,1,0,1,GAINREDJEWEL,1,DOWN}} ,
{{0,0,0,GAINBIGBOTTLE,60,61,60,0,0,1,DOWN},{52,1,1,1,1,1,1,1,60,1,0},{0,1,0,GOADGATE,0,0,56,1,0,0,0},{0,1,0,1,0,31,0,1,1,1,SILVERGATE},{0,1,41,1,56,0,1000,1,0,GAINBIGBOTTLE,0},{0,1,0,1,1,1,1,1,0,1,UP},{0,BRONZEWGATE,0,0,41,42,41,0,0,1,1},{1,1,1,1,1,1,1,1,56,1,GAINBIGFLY},{0,0,0,0,GAINREDJEWEL,GAINREDJEWEL,GAINREDJEWEL,0,0,1,31},{56,1,1,1,1,1,1,1,1,1,BRONZEWGATE},{0,0,0,0,0,0,GAINBLUEJEWEL,GAINBLUEJEWEL,GAINBLUEJEWEL,0,0} } ,
{{1,1,1,1,1,1,1,1,1,1,1},{0,0,0,0,0,0,0,0,0,41,1},{64,GAINBIGBOTTLE,1,1,1,GAINSMALLBOTTLE,3,3,3,42,1},{GAINMUTIPLEKEY,1,1,1,1,1,3,3,3,41,1},{UP,1,1,1,1,1,1,1,1,SILVERGATE,1},{0,1,1,GAINHOLYWATER,0,56,31,56,SILVERGATE,0,DOWN},{0,1,1,1,1,1,1,1,1,SILVERGATE,1},{0,1,1,1,1,1,3,3,3,41,1},{0,GAINBIGBOTTLE,1,1,1,GAINSMALLBOTTLE,3,3,3,42,1},{0,0,0,0,0,0,0,0,0,41,1},{1,1,1,1,1,1,1,1,1,1,1} },
{{0},{0,3,3,3,3,3,3,3,3,3,0},{0,3,3,1,1,1,3,3,3,3,0},{0,3,1,1,1,1,1,1,3,3,0},{DOWN,3,1,1000,GAINBLUEJEWEL,GAINREDJEWEL,0,BRONZEWGATE,0,3,0},{3,3,1,1,1,1,1,1,0,GOADGATE,0},{UP,3,1,1001,GAINBLUEJEWEL,GAINREDJEWEL,0,BRONZEWGATE,0,3,0},{0,3,1,1,1,1,1,1,3,3,0},{0,3,3,1,1,1,3,3,3,3,0},{0,3,3,3,3,3,3,3,3,3,0},{0}},
{{3,3,3,3,3,3,3,3,3,3,3},{3,3,3,3,3,3,3,3,3,3,3},{3,3,3,3,3,3,3,3,3,3,3},{3,3,3,3,1,1,1,1,3,3,3},{3,3,3,1,1,1,1,1,1,3,3},{0,0,0,GOADGATE,0,72,0,UP,1,3,3},{DOWN,3,3,1,1,1,1,1,1,3,3},{3,3,3,3,1,1,1,1,3,3,3},{3,3,3,3,3,3,3,3,3,3,3},{3,3,3,3,3,3,3,3,3,3,3},{3,3,3,3,3,3,3,3,3,3,3}},
{{3,3,3,3,3,3,3,3,3,3,UP},{64,31,0,0,0,0,0,31,64,3,0},{31,3,3,3,3,3,3,3,31,3,76},{0,3,64,0,0,64,3,3,0,3,0},{0,3,0,3,3,0,3,3,76,3,0},{0,3,0,3,64,0,3,DOWN,0,3,0},{0,3,0,3,0,3,3,3,3,3,0},{0,3,0,3,0,3,64,0,64,3,0},{0,3,0,3,0,3,0,3,0,3,0},{0,3,0,3,64,0,64,3,0,3,0},{64,0,64,3,3,3,3,3,64,0,64}},
{{3,3,3,3,3,3,3,3,3,3,DOWN},{3,3,3,3,3,3,3,3,3,3,0},{3,3,3,3,3,3,3,3,3,3,0},{3,3,3,3,1,1,1,3,3,3,0},{3,3,3,1,1,1,1,1,3,3,0},{3,3,3,1,1007,0,GOADGATE,GOADGATE,3,3,0},{3,3,3,1,1,1,1,1,3,3,0},{3,3,3,3,1,1,1,3,3,3,0},{3,3,3,3,3,3,3,3,3,3,0},{3,3,3,3,3,3,3,3,3,3,0},{3,3,3,3,3,3,3,3,3,3,0}},
{{0},{0,3,3,3,3,3,3,3,3,3,0},{0,0,0,0,0,72,0,GAINGODSWORD,3,3,0},{0,3,3,3,3,3,3,3,3,3,0},{0,3,3,3,3,3,3,3,3,3,0},{0,3,3,UP,0,0,27,0,0,0,0},{0,3,3,3,3,3,3,3,3,3,0},{0,3,3,3,3,3,3,3,3,3,0},{0,0,0,0,0,72,0,GAINGODARMOR,3,3,0},{0,3,3,3,3,3,3,3,3,3,0},{0,0,0,0,0,0,0,0,0,0,DOWN}},
{{76,GAINBIGBOTTLE,3,GAINSMALLBOTTLE,31,-22,31,GAINSMALLBOTTLE,3,GAINBIGBOTTLE,76},{GAINREDJEWEL,3,GAINBLUEJEWEL,3,-21,3,-21,3,GAINBLUEJEWEL,3,GAINREDJEWEL},{48,-20,48,-20,64,0,64,-20,48,-20,48},{GAINSMALLBOTTLE,3,0,3,0,3,0,3,0,3,GAINSMALLBOTTLE},{31,-21,64,0,0,0,0,0,64,-21,31},{-22,3,0,DOWN,0,3,0,UP,0,3,-22},{31,-21,64,0,0,0,0,0,64,-21,31},{GAINSMALLBOTTLE,3,0,3,0,3,0,3,0,3,GAINSMALLBOTTLE},{48,-20,48,-20,64,0,64,-20,48,-20,48},{GAINREDJEWEL,3,GAINBLUEJEWEL,3,-21,3,-21,3,GAINBLUEJEWEL,3,GAINREDJEWEL},{76,GAINBIGBOTTLE,3,GAINSMALLBOTTLE,31,-22,31,GAINSMALLBOTTLE,3,GAINBIGBOTTLE,76}},
{{3,3,3,3,3,3,3,3,3,3,3},{3,3,0,0,3,3,3,3,3,3,3},{3,0,0,0,0,0,3,3,3,3,3},{3,0,3,0,0,0,0,3,3,3,3},{3,3,3,3,0,0,0,3,3,3,3},{3,27,68,68,0,0,3,DOWN,3,3,3},{3,3,3,3,0,0,0,3,3,3,3},{3,0,3,0,0,0,0,3,3,3,3},{3,0,0,0,0,0,3,3,3,3,3},{3,3,0,0,3,3,3,3,3,3,3},{3,3,3,3,3,3,3,3,3,3,3}} };//保存各层的迷宫信息
POINT g_UpInitPos[30] = { {9*SQUIREUNITWIDTH,9*SQUIREUNITHEIGHT},{9 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT}, {4 * SQUIREUNITWIDTH,SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{14 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT}, {4 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{13 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT} ,{9 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{4 * SQUIREUNITWIDTH,SQUIREUNITHEIGHT},{10 * SQUIREUNITWIDTH,3 * SQUIREUNITHEIGHT},{8 * SQUIREUNITWIDTH,6 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{13 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT} ,
{9 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{7 * SQUIREUNITWIDTH,0},{9 * SQUIREUNITWIDTH,0},{9 * SQUIREUNITWIDTH,8 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{13 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{9 * SQUIREUNITWIDTH,4 * SQUIREUNITHEIGHT},{9*SQUIREUNITWIDTH,5*SQUIREUNITHEIGHT} };//记录上楼以及楼层跳转之后的初始位置
POINT g_DownInitPos[29] = { {9 * SQUIREUNITWIDTH,SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,0}, {4 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{14 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{4 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{13 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{9 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,0},{11 * SQUIREUNITWIDTH,4 * SQUIREUNITHEIGHT},{10 * SQUIREUNITWIDTH,7 * SQUIREUNITHEIGHT},{4 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{13 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{8*SQUIREUNITWIDTH,10*SQUIREUNITHEIGHT} ,
{9 * SQUIREUNITWIDTH,0},{11 * SQUIREUNITWIDTH,0},{9 * SQUIREUNITWIDTH,6 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT} ,{13 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{9 * SQUIREUNITWIDTH,4 * SQUIREUNITHEIGHT},{0,0}/*(NULL)*/,{0,0}/*(NULL)*/ };//记录下楼之后人物的初始位置
struct HERO
{
	POINT Position;
	int Direction;
	int iNum;
	int HP;
	int damage;
	int defence;
	int exp;
	int goad;
	int bronzekey;
	int silverkey;
	int goadkey;
	int level;
	int weapon;
	int armor;
	BOOL radar;
	BOOL compass;
	BOOL cross;
	BOOL hammer;
};//包括英雄的各个数据
struct MONSTER
{
	int damage;
	int defence;
	int HP;
	int exp;
	int goad;
};//记录怪物各个属性
MONSTER g_monster[MONSTERSPECIES][4] = { {{20,1,50,1,1},{15,2,70,2,2},{35,10,200,5,5},{250,125,700,30,32}},//怪系
{{20,5,100,3,3},{65,30,150,8,10},{160,90,550,20,25},{1700,1500,30000,220,250}} ,//蝙蝠系
{{25,5,110,4,5},{40,20,150,6,8},{90,50,400,12,15},{680,590,2000,65,70}},//骷髅系
{{50,25,125,7,10},{200,110,100,25,30},{120,70,220,17,20},{400,260,500,45,47}},//巫师系
{{75,45,300,10,13},{450,330,900,50,50},{},{}},//兽面系
{{150,90,450,19,22},{500,400,1250,45,55},{560,460,1500,60,60},{}} ,//卫兵系
{{115,65,500,15,15},{},{},{}},//石头系
{{300,150,1300,35,40},{},{},{}},//衣武士系
{{620,520,1200,75,65},{},{},{}},//双手剑士系
{{680,590,2000,65,70},{},{},{} },//冥战士系
{{350,200,850,40,45},{750,650,900,70,77},{},{} },//金乘五系
{{980,900,1200,75,88},{},{},{} },//灵武士系
{{830,730,1500,70,80},{},{},{}},//灵法师系
{{1000,1000,15000,100,100},{},{},{} },//红衣魔王
{{1050,950,3100,80,92},{},{},{}},//影子战士系
{{4000,2000,40000,0,0}}/*血影*/ };//记录怪物属性
MONSTER g_CopyMonster = g_monster[g_battleobject / 4][g_battleobject % 4];//当前战斗对象，用以判断是否触发剧情，以及战斗需要
HERO Hero;
HBRUSH g_hCleaner,g_battlebrush;//清屏画刷，战斗框画刷
HPEN g_battlepen,g_cleanpen;//战斗边框，空的清屏边框
HWND g_hwnd;//窗口句柄
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL Game_Init(HWND hwnd);
VOID Game_Paint(HWND hwnd);
BOOL Game_CleanUp(HWND hwnd);
VOID Game_Output();
BOOL Game_Battle(int x);
VOID Game_Judge(int &next);
VOID Game_Swich();
VOID Game_gain(int &next);
VOID Game_BattlePaint();
VOID Game_BattleGainPaint();
VOID Game_TalkingPaint();
VOID Game_GainPaint(int next);
VOID Game_Select();
VOID Game_RadarPaint();
VOID Game_CompassPaint();
VOID Game_StartSelectPaint();
VOID Game_GutPaint();
VOID Game_EndPaint();
VOID Game_BGMJudge();
POINT Game_NextArrayPoint();
//程序的入口函数，负责窗口的设计，创建，显示移动更新，注销
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//窗口类的设计
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"favicon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"ForTheDream";
	//窗口类的注册
	if (!RegisterClassEx(&wndClass))
		return -1;
	//窗口的正式创建
	HWND hWnd = CreateWindow(L"ForTheDream", WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	g_hwnd = hWnd;
	//窗口的移动，显示，更新
	MoveWindow(hWnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT+40, true);
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);
	//运行封装好的游戏初始化函数，用以加载所需位图，初始化失败，则弹出消息对话框
	if (!Game_Init(hWnd))
	{
		MessageBox(hWnd, L"资源初始化失败", L"消息窗口", 0);
		return FALSE;
	}
	//游戏信息循环体系，更加高效的绘图
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_tNow = GetTickCount();
			if (g_tNow - g_tPre >= 10)
			{
				Game_Paint(hWnd);
			}
		}
	}
	UnregisterClass(L"FoeTheDream", wndClass.hInstance);
	return 0;
}
//窗口过程函数，响应键盘消息
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		
	case WM_KEYDOWN:
		if (!g_swiching && !g_battle && !g_battlegain && !g_talking && !g_selecting && !g_gain && !g_radaring && !g_compassing && !g_start&&!g_gut&&!g_end)
		{
			ofstream fout("data.dat", ios::binary);
			switch (wParam)
			{
			case VK_ESCAPE://写入二进制文件，存盘
				fout.write((char*)(&Hero), sizeof(Hero));
				fout.write((char*)(&g_PileNo), sizeof(g_PileNo));
				fout.write((char*)(&g_maze), sizeof(g_maze));
				fout.write((char*)(&g_maxpile), sizeof(g_maxpile));
				fout.write((char*)(&g_RedNo), sizeof(g_RedNo));
				fout.write((char*)(&g_BlackNo), sizeof(g_BlackNo));
				fout.write((char*)(&g_thieftalkingNo), sizeof(g_thieftalkingNo));
				fout.write((char*)(&g_princessNo), sizeof(g_princessNo));
				fout.write((char*)(&g_monster), sizeof(g_monster));
				sum = MessageBox(NULL, L"已自动存档。确定要返回标题界面么？", L"魔塔：拯救公主", MB_YESNO);
				switch (sum)
				{
				case IDYES:
					g_start = TRUE;
					PlaySound(L"Ag3.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
					mciSendString(L"close back", NULL, 0, NULL);
					break;
				default:
					break;
				}
				
				break;
			case VK_UP:
				Hero.Direction = 0;
				if ((Hero.Position.y - SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT >= 0)//到达边界之后，再向前走则不能触发判断！！
				{

					if (g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y - SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT] != 0 && g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y - SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT] > (-20))//此时人物无法走动，触发判断函数，以判断下一步是否触发剧情以及引起变化
					{
						Game_Judge(g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y - SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT]);
						break;
					}
					Game_gain(g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y - SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT]);//运行获得道具函数
					Hero.Position.y -= SQUIREUNITHEIGHT;
				}
				if (Hero.Position.y < 0)
				{
					Hero.Position.y = 0;
				}
				break;
			case VK_DOWN:
				Hero.Direction = 2;
				if ((Hero.Position.y + SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT <= 10)
				{

					if (g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y + SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT] != 0 && g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y + SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT] > (-20))
					{
						Game_Judge(g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y + SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT]);
						break;
					}
					Game_gain(g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y + SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT]);
					Hero.Position.y += SQUIREUNITHEIGHT;
				}
				if (Hero.Position.y > 10 * SQUIREUNITHEIGHT)
				{
					Hero.Position.y = 10 * SQUIREUNITHEIGHT;
				}
				break;
			case VK_LEFT:
				Hero.Direction = 3;
				if ((Hero.Position.x - 4 * SQUIREUNITWIDTH - SQUIREUNITWIDTH) / SQUIREUNITWIDTH >= 0)
				{

					if (g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH - SQUIREUNITWIDTH) / SQUIREUNITWIDTH][Hero.Position.y / SQUIREUNITHEIGHT] != 0 && g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH - SQUIREUNITWIDTH) / SQUIREUNITWIDTH][Hero.Position.y / SQUIREUNITHEIGHT] > (-20))
					{
						Game_Judge(g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH - SQUIREUNITWIDTH) / SQUIREUNITWIDTH][Hero.Position.y / SQUIREUNITHEIGHT]);
						break;
					}
					Game_gain(g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH - SQUIREUNITWIDTH) / SQUIREUNITWIDTH][Hero.Position.y / SQUIREUNITHEIGHT]);
					Hero.Position.x -= SQUIREUNITWIDTH;
				}
				if (Hero.Position.x < 4 * SQUIREUNITWIDTH)
				{
					Hero.Position.x = 4 * SQUIREUNITWIDTH;
				}
				break;
			case VK_RIGHT:
				Hero.Direction = 1;
				if ((Hero.Position.x - 4 * SQUIREUNITWIDTH + SQUIREUNITWIDTH) / SQUIREUNITWIDTH<=10)
				{
					if (g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH + SQUIREUNITWIDTH) / SQUIREUNITWIDTH][Hero.Position.y / SQUIREUNITHEIGHT] != 0 && g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH + SQUIREUNITWIDTH) / SQUIREUNITWIDTH][Hero.Position.y / SQUIREUNITHEIGHT] > (-20))
					{
						Game_Judge(g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH + SQUIREUNITWIDTH) / SQUIREUNITWIDTH][Hero.Position.y / SQUIREUNITHEIGHT]);
						break;
					}
					Game_gain(g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH + SQUIREUNITWIDTH) / SQUIREUNITWIDTH][Hero.Position.y / SQUIREUNITHEIGHT]);
					Hero.Position.x += SQUIREUNITWIDTH;
				}
				if (Hero.Position.x > (14 * SQUIREUNITWIDTH))
				{
					Hero.Position.x = 14 * SQUIREUNITWIDTH;
				}
				break;
				if (g_PileNo != 21)
				{
			case VK_F1:
				if (Hero.radar)
				{
					g_radaring = TRUE;
				}
				break;
			case VK_F2:
				if (Hero.compass)
				{
					g_compassing = TRUE;
				}
				break;
				}
			default:
				break;
			}
			fout.close();
		}
		if (g_talking)//剧情控制
		{
			switch (wParam)
			{
			case VK_SPACE:
				g_talkingNo++;
				PlaySound(L"skip.wav", NULL, SND_FILENAME | SND_ASYNC );
				break;
			default:
				break;
			}
		}
		else if (g_selecting)//选择控制
		{
			switch (wParam)
			{
			case VK_DOWN:
				g_selectofnow++;
				if (g_selectofnow>=g_seletobjectsNo)
				{
					g_selectofnow = 0;
				}
				PlaySound(L"select.wav", NULL, SND_FILENAME | SND_ASYNC);
				break;
			case VK_UP:
				g_selectofnow--;
				if (g_selectofnow<0)
				{
					g_selectofnow = g_seletobjectsNo - 1;
				}
				PlaySound(L"select.wav", NULL, SND_FILENAME | SND_ASYNC);
				break;
			case VK_SPACE:
				g_seleckeydown = TRUE;
				break;
			default:
				break;
			}
		}
		else if (g_radaring)//怪物雷达退出控制
		{
			switch (wParam)
			{
			case VK_ESCAPE:
					g_radaring = FALSE;
				break;
			default:
				break;
			}
		}
		else if (g_compassing)//罗盘楼层跳转选择控制
		{
			if (g_compassswichtime == 0)
			{

				switch (wParam)
				{
				case VK_ESCAPE:
					g_compassing = FALSE;
					break;
				case VK_DOWN:
					g_compass++;
					if (g_compass > g_maxpile)
					{
						g_compass = 1;
					}
					break;
				case VK_UP:
					g_compass--;
					if (g_compass < 1)
					{
						g_compass = g_maxpile;
					}
					break;
				case VK_SPACE:
					g_seleckeydown = TRUE;
					break;
				default:
					break;
				}
			}
		}
		else if (g_start)//标题界面选择控制
		{
			switch (wParam)
			{
			case VK_DOWN:
				g_startNo++;
				if (g_startNo > 2)
				{
					g_startNo = 0;
				}
				break;
			case VK_UP:
				g_startNo--;
				if (g_startNo < 0)
				{
					g_startNo = 2;
				}
				break;
			case VK_SPACE:
				g_seleckeydown = TRUE;
				break;
			default:
				break;
			}
		}
		else if (g_gut)//开始剧情控制
		{
			switch (wParam)
			{
			case VK_SPACE:
				g_gut = FALSE;
				g_guttime = 0;
				mciSendString(L"close gut", NULL, 0, NULL);
				break;
			default:
				break;
			}
		}
		break;
	case WM_DESTROY:
		Game_CleanUp(hwnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
//主要加载各个位图，以及初始化画笔，画刷
BOOL Game_Init(HWND hwnd)
{
	g_PileNo = 0;
	Hero = { g_UpInitPos[0],0,0,1000,10,10,0,0,0,0,0,0,0,0,FALSE,FALSE,FALSE,FALSE };
	g_hdc = GetDC(hwnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufdc = CreateCompatibleDC(g_hdc);
	g_swichdc = CreateCompatibleDC(g_hdc);
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_mdc, bmp);
	g_battlebrush = CreateSolidBrush(RGB(0, 0, 0));
	g_battlepen = CreatePen(PS_SOLID, 5, RGB(199, 97, 20));
	g_cleanpen = CreatePen(PS_NULL, 5, RGB(0, 0, 0));
	g_hSprite[0] = (HBITMAP)LoadImage(NULL, L"go1.bmp", IMAGE_BITMAP, 8 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hSprite[1] = (HBITMAP)LoadImage(NULL, L"go4.bmp", IMAGE_BITMAP, 8 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hSprite[2] = (HBITMAP)LoadImage(NULL, L"go2.bmp", IMAGE_BITMAP, 8 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hSprite[3] = (HBITMAP)LoadImage(NULL, L"go3.bmp", IMAGE_BITMAP, 8 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_bg = (HBITMAP)LoadImage(NULL, L"clean.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_msgbg = (HBITMAP)LoadImage(NULL, L"bgofmsg.bmp", IMAGE_BITMAP, 4 * SQUIREUNITWIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hWall = (HBITMAP)LoadImage(NULL, L"wall.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hLava = (HBITMAP)LoadImage(NULL, L"lava.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hyg = (HBITMAP)LoadImage(NULL, L"yellowgate.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hStar = (HBITMAP)LoadImage(NULL, L"star.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hRoad = (HBITMAP)LoadImage(NULL, L"road.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hUp = (HBITMAP)LoadImage(NULL, L"up.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hDown = (HBITMAP)LoadImage(NULL, L"down.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hKey = (HBITMAP)LoadImage(NULL, L"key.bmp", IMAGE_BITMAP, 3 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hGoadgate = (HBITMAP)LoadImage(NULL, L"goadgate.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hvs = (HBITMAP)LoadImage(NULL, L"vs.bmp", IMAGE_BITMAP, 3.5*SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hStone = (HBITMAP)LoadImage(NULL, L"stonegate.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hSilverGate = (HBITMAP)LoadImage(NULL, L"silvergate.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hSword = (HBITMAP)LoadImage(NULL, L"gainsword.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hBlueJewel = (HBITMAP)LoadImage(NULL, L"bluejewel.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hRedJewel = (HBITMAP)LoadImage(NULL, L"redjewel.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hSmallBottle = (HBITMAP)LoadImage(NULL, L"smallbottle.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hBigBottle = (HBITMAP)LoadImage(NULL, L"bigbottle.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hMutipleKey = (HBITMAP)LoadImage(NULL, L"mutiplekey.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hArmor = (HBITMAP)LoadImage(NULL, L"armor.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hRadar = (HBITMAP)LoadImage(NULL, L"radar.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hSilverSword = (HBITMAP)LoadImage(NULL, L"silversword.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hSmallFly = (HBITMAP)LoadImage(NULL, L"smallfly.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hBullion = (HBITMAP)LoadImage(NULL, L"bullion.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hGoadArmor = (HBITMAP)LoadImage(NULL, L"goadarmor.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hHolyWater = (HBITMAP)LoadImage(NULL, L"holywater.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hCompass = (HBITMAP)LoadImage(NULL, L"compass.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hCross = (HBITMAP)LoadImage(NULL, L"cross.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hBigFly = (HBITMAP)LoadImage(NULL, L"bigfly.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hHammer = (HBITMAP)LoadImage(NULL, L"hammer.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hGodSword = (HBITMAP)LoadImage(NULL, L"godsword.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hGodArmor = (HBITMAP)LoadImage(NULL, L"godarmor.bmp", IMAGE_BITMAP, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hStart = (HBITMAP)LoadImage(NULL, L"start.bmp", IMAGE_BITMAP, 6 * SQUIREUNITWIDTH, 8 * SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hStartSelect = (HBITMAP)LoadImage(NULL, L"startselect.bmp", IMAGE_BITMAP, 2 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	g_hGut = (HBITMAP)LoadImage(NULL, L"gut.bmp", IMAGE_BITMAP, 10 * SQUIREUNITWIDTH, 2 * WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hGutbg = (HBITMAP)LoadImage(NULL, L"gutbg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hEnd = (HBITMAP)LoadImage(NULL, L"end.bmp", IMAGE_BITMAP, 10 * SQUIREUNITWIDTH, 2 * WINDOW_HEIGHT, LR_LOADFROMFILE);
	wchar_t mon[20];
	for (int i = 0; i < MONSTERSPECIES; i++)
	{
		if (i == 15)
		{
			memset(mon, 0, sizeof(mon));
			swprintf_s(mon, L"Monster%d.bmp", i);
			g_hMonster[i] = (HBITMAP)LoadImage(NULL, mon, IMAGE_BITMAP, 4 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, LR_LOADFROMFILE);
			break;
		}
		memset(mon, 0, sizeof(mon));
		swprintf_s(mon, L"Monster%d.bmp", i);
		g_hMonster[i] = (HBITMAP)LoadImage(NULL, mon, IMAGE_BITMAP, 4 * SQUIREUNITWIDTH, 4 * SQUIREUNITHEIGHT, LR_LOADFROMFILE);

	}
	wchar_t NPCs[10];
	for (int i = 0; i < NPCSPECIES; i++)
	{
		memset(NPCs, 0, sizeof(NPCs));
		swprintf_s(NPCs, L"NPC%d.bmp", i);
		g_hNPC[i] = (HBITMAP)LoadImage(NULL, NPCs, IMAGE_BITMAP, 4 * SQUIREUNITWIDTH, 4 * SQUIREUNITHEIGHT, LR_LOADFROMFILE);
	}
	Game_Paint(hwnd);
	PlaySound(L"Ag3.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	return TRUE;
}
//进行游戏画面的绘制
VOID Game_Paint(HWND hwnd)
{
	SelectObject(g_bufdc, g_bg);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);
	SelectObject(g_bufdc, g_msgbg);
	BitBlt(g_mdc, 0, 0, 4 * SQUIREUNITWIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);
	int m = 0;
	for (int i=0;i<11;i++)
	{
		m = i;
		for (int i = 0;i<11;i++)
		{
			switch (g_maze[g_PileNo][m][i])
			{
			case 1:
				SelectObject(g_bufdc, g_hWall);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				break;
			case 2:
				SelectObject(g_bufdc, g_hLava);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				break;
			case BRONZEWGATE:
				SelectObject(g_bufdc, g_hyg);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				break;
			case SILVERGATE:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hSilverGate);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH,SQUIREUNITHEIGHT,RGB(255,255,255));
				break;
			case GOADGATE:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hGoadgate);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				break;
			case 3:
				SelectObject(g_bufdc,g_hStar);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				break;
			case 4:
				SelectObject(g_bufdc, g_hStone);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				break;
			case 0:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				break;
			case UP:
				SelectObject(g_bufdc, g_hUp);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				break;
			case DOWN:
				SelectObject(g_bufdc, g_hDown);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				break;
			case -22:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hKey);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH+8, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 2 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH+8, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 2 * SQUIREUNITWIDTH, 0, SRCPAINT);
				break;
			case -21:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hKey);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, SQUIREUNITWIDTH, 0, SRCPAINT);
			    break;
			case -20:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hKey);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH-8, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH-8, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
			    break;
			case GAINSWORD:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hSword);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINBLUEJEWEL:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hBlueJewel);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINREDJEWEL:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hRedJewel);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINSMALLBOTTLE:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hSmallBottle);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINBIGBOTTLE:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hBigBottle);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINMUTIPLEKEY:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hMutipleKey);
			    TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
			    break;
			case GAINARMOR:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hArmor);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINRADAR:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hRadar);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINSILVERSWORD:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hSilverSword);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINSMALLFLY:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hSmallFly);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINBULLION:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hBullion);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINGOADARMOR:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hGoadArmor);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINHOLYWATER:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hHolyWater);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINCOMPASS:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hCompass);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINCROSS:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hCross);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINBIGFLY:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hBigFly);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINHAMMER:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hHammer);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINGODSWORD:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hGodSword);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			case GAINGODARMOR:
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hGodArmor);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				break;
			default:
				break;
			}
			if (g_maze[g_PileNo][m][i]>=20&& g_maze[g_PileNo][m][i]<80)
			{
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hMonster[(g_maze[g_PileNo][m][i]-20)/4]);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, (g_maze[g_PileNo][m][i]-20)%4*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
			}
			else if (g_maze[g_PileNo][m][i] == 80)
			{
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
			}
			else if (g_maze[g_PileNo][m][i]>=1000)
			{
				SelectObject(g_bufdc, g_hRoad);
				BitBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCCOPY);
				SelectObject(g_bufdc, g_hNPC[(g_maze[g_PileNo][m][i]-1000)/4]);
				TransparentBlt(g_mdc, (m + 4)*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, (g_maze[g_PileNo][m][i] - 1000) % 4 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
			}
		}
	}
	if (g_PileNo==21&&g_maze[21][0][0]==2)
	{
		SelectObject(g_bufdc, g_hMonster[15]);
		TransparentBlt(g_mdc, 8*SQUIREUNITWIDTH, SQUIREUNITHEIGHT, 3 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
		//TransparentBlt(g_mdc, (m + 5)*SQUIREUNITWIDTH, (i - 2)*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH+SQUIREUNITWIDTH*2/3,0, SQUIREUNITWIDTH/3, SQUIREUNITHEIGHT, RGB(255, 255, 255));

	}
	SelectObject(g_bufdc, g_hSprite[Hero.Direction]);
	BitBlt(g_mdc, Hero.Position.x, Hero.Position.y, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 5 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SRCAND);
	BitBlt(g_mdc, Hero.Position.x, Hero.Position.y, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 5 * SQUIREUNITWIDTH, 0, SRCPAINT);
	Game_Output();
	if (g_swiching)
	{
		g_swich++;
		Game_Swich();
	}
	if (g_battle)
	{
		g_battletime++;
		Game_BattlePaint();
	}
	if (g_battlegain)
	{
		g_battlegaintime++;
		Game_BattleGainPaint();
	}
	if (g_talking)
	{
		g_talkingtime++;
		Game_TalkingPaint();
	}
	if (g_selecting)
	{
		g_selecttime++;
		Game_Select();
	}
	if (g_gain)
	{
		g_gaintime++;
		Game_GainPaint(g_gainobject);
	}
	if (g_radaring)
	{
		Game_RadarPaint();
	}
	if (g_compassing)
	{
		g_compasstime++;
		Game_CompassPaint();
	}
	if (g_start)
	{
		g_starttime++;
		Game_StartSelectPaint();
	}
	if (g_gut)
	{
		g_guttime++;
		Game_GutPaint();
	}
	if (g_end)
	{
		g_guttime++;
		Game_EndPaint();
	}
	if (g_bgm)
	{
		g_bgmtime++;
		Game_BGMJudge();
	}
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
	g_tPre = GetTickCount();
	Hero.iNum++;
	if (Hero.iNum==40)
	{
		Hero.iNum = 0;
	}
}
//输出位于画面左侧的各个信息
VOID Game_Output()
{
	HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	SelectObject(g_mdc, hFont);
	SetBkMode(g_mdc, TRANSPARENT);
	SetTextColor(g_mdc, RGB(255, 255, 255));
	wchar_t msg[20];
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"第%d层", g_PileNo);
	TextOut(g_mdc, 50, 30, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"Lv%d", Hero.level);
	TextOut(g_mdc, 190, 30, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"生命值 %d", Hero.HP);
	TextOut(g_mdc, 50, 100, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"攻击力 %d", Hero.damage);
	TextOut(g_mdc, 50, 140, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"防御力 %d", Hero.defence);
	TextOut(g_mdc, 50, 180, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"经验值 %d", Hero.exp);
	TextOut(g_mdc, 50, 220, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"金币 %d", Hero.goad);
	TextOut(g_mdc, 50, 260, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"         %d把", Hero.bronzekey);
	TextOut(g_mdc, 50, 323, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"         %d把", Hero.silverkey);
	TextOut(g_mdc, 50, 383, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"         %d把", Hero.goadkey);
	TextOut(g_mdc, 50, 443, msg, wcslen(msg));
	DeleteObject(hFont);
	SelectObject(g_bufdc, g_hKey);
	BitBlt(g_mdc, 27, 300, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
	BitBlt(g_mdc, 27, 300, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
	SelectObject(g_bufdc, g_hKey);
	BitBlt(g_mdc, 35, 360, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SRCAND);
	BitBlt(g_mdc, 35, 360, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, SQUIREUNITWIDTH, 0, SRCPAINT);
	SelectObject(g_bufdc, g_hKey);
	BitBlt(g_mdc, 43, 420, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 2*SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SRCAND);
	BitBlt(g_mdc, 43, 420, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 2*SQUIREUNITWIDTH, 0, SRCPAINT);
	SelectObject(g_bufdc, g_hSprite[2]);
	BitBlt(g_mdc, 30, 520, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, (Hero.iNum / 5+1)%8 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SRCAND);
	BitBlt(g_mdc, 30, 520, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 5 * SQUIREUNITWIDTH, 0, SRCPAINT);
	switch (Hero.weapon)
	{
	case GAINGODSWORD:
		SelectObject(g_bufdc, g_hGodSword);
		TransparentBlt(g_mdc, 110, 520, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
		break;
	case GAINSILVERSWORD:
		SelectObject(g_bufdc, g_hSilverSword);
		TransparentBlt(g_mdc, 110, 520, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
		break;
	case GAINSWORD:
		SelectObject(g_bufdc, g_hSword);
		TransparentBlt(g_mdc, 110, 520, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
		break;
	default:
		break;
	}
	switch (Hero.armor)
	{
	case GAINGODARMOR:
		SelectObject(g_bufdc, g_hGodArmor);
		TransparentBlt(g_mdc, 190, 520, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
		break;
	case GAINGOADARMOR:
		SelectObject(g_bufdc, g_hGoadArmor);
		TransparentBlt(g_mdc, 190, 520, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
		break;
	case GAINARMOR:
		SelectObject(g_bufdc, g_hArmor);
		TransparentBlt(g_mdc, 190, 520, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
		break;
	default:
		break;
	}
	if (Hero.radar)
	{
		SelectObject(g_bufdc, g_hRadar);
		TransparentBlt(g_mdc, 30, 580, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
	}
	if (Hero.compass)
	{
		SelectObject(g_bufdc, g_hCompass);
		TransparentBlt(g_mdc, 110, 580, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
	}
	DeleteObject(hFont);
}
//预测战斗后英雄是否死亡
BOOL Game_Battle(int x)
{
	HERO savehero=Hero;
	MONSTER copymonster= g_monster[x / 4][x % 4];
	if (savehero.damage <= copymonster.defence&&copymonster.damage <= savehero.defence)
	{
		return FALSE;
	}
	while (savehero.HP > 0 && copymonster.HP > 0)
	{
		switch (x)
		{
		case 14:
			savehero.HP -= 100;
			break;
		case 15:
			savehero.HP -= 300;
			break;
		case 28:
			savehero.HP *= 0.75;
			break;
		case 48:
			savehero.HP = savehero.HP * 2 / 3;
			break;
		default:
			break;
		}
		if (savehero.HP<=0)
		{
			break;
		}
		copymonster.HP -= (savehero.damage - copymonster.defence);
		if (copymonster.HP<=0)
		{
			break;
		}
		if (copymonster.damage - savehero.defence>0)
		{
			savehero.HP -= (copymonster.damage - savehero.defence);
		}
		
	}
	if (copymonster.HP<=0)
	{
		g_battleobject = x;
		g_CopyMonster = g_monster[g_battleobject / 4][g_battleobject % 4];
		return TRUE;
	}
	return FALSE;
}
//进行战斗界面绘制
VOID Game_BattlePaint()
{
	SelectObject(g_mdc,g_battlepen);
	SelectObject(g_mdc, g_battlebrush);
	Rectangle(g_mdc, 4.5*SQUIREUNITWIDTH, 0.5*SQUIREUNITHEIGHT, WINDOW_WIDTH - 0.5*SQUIREUNITWIDTH, 7.5*SQUIREUNITHEIGHT);
	SelectObject(g_bufdc, g_msgbg);//贴战斗背景
	StretchBlt(g_mdc, 4.5*SQUIREUNITWIDTH, 0.5*SQUIREUNITHEIGHT, 10 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, g_bufdc, 0, 0, 4*SQUIREUNITWIDTH, WINDOW_HEIGHT, SRCCOPY);
	SelectObject(g_bufdc, g_hSprite[1]);//贴英雄
	StretchBlt(g_mdc, 4.75*SQUIREUNITWIDTH, 1.25*SQUIREUNITHEIGHT, 2 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 5 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SRCAND);
	StretchBlt(g_mdc, 4.75*SQUIREUNITWIDTH, 1.25*SQUIREUNITHEIGHT, 2 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 5 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SRCPAINT);
	SelectObject(g_bufdc, g_hMonster[g_battleobject / 4]);
	if (g_battleobject == 60)
	{
		TransparentBlt(g_mdc, WINDOW_WIDTH - 2.75*SQUIREUNITWIDTH, 1.25*SQUIREUNITHEIGHT, 2*SQUIREUNITWIDTH, 2*SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH+SQUIREUNITWIDTH/3, SQUIREUNITHEIGHT*2/3, SQUIREUNITWIDTH/3, SQUIREUNITHEIGHT/3, RGB(255, 255, 255));
	}
	else
	{
		TransparentBlt(g_mdc, WINDOW_WIDTH - 2.75*SQUIREUNITWIDTH, 1.25*SQUIREUNITHEIGHT, 2 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, g_battleobject % 4 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
	}
	SelectObject(g_bufdc, g_hvs);
	TransparentBlt(g_mdc, 7.75*SQUIREUNITWIDTH, 1.25*SQUIREUNITHEIGHT, 3.5*SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, g_bufdc, 0, 0, 3.5*SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, RGB(255, 255, 255));
	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	SelectObject(g_mdc, hFont);
	SetBkMode(g_mdc, TRANSPARENT);
	SetTextColor(g_mdc, RGB(255, 255, 255));
	wchar_t msg[20];
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"生命值 %d", Hero.HP);
	TextOut(g_mdc, 4.75*SQUIREUNITWIDTH, 3.25*SQUIREUNITHEIGHT+60, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"攻击力 %d", Hero.damage);
	TextOut(g_mdc, 4.75*SQUIREUNITWIDTH, 3.25*SQUIREUNITHEIGHT+120, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"防御力 %d", Hero.defence);
	TextOut(g_mdc, 4.75*SQUIREUNITWIDTH, 3.25*SQUIREUNITHEIGHT+180, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"生命值 %d", g_CopyMonster.HP);
	TextOut(g_mdc, WINDOW_WIDTH - 2.75*SQUIREUNITWIDTH, 3.25*SQUIREUNITHEIGHT + 60, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"攻击力 %d", g_CopyMonster.damage);
	TextOut(g_mdc, WINDOW_WIDTH - 2.75*SQUIREUNITWIDTH, 3.25*SQUIREUNITHEIGHT + 120, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"防御力 %d", g_CopyMonster.defence);
	TextOut(g_mdc, WINDOW_WIDTH - 2.75*SQUIREUNITWIDTH, 3.25*SQUIREUNITHEIGHT + 180, msg, wcslen(msg));
	DeleteObject(hFont);
	if ( g_battletime==1)
	{
		switch (g_battleobject)
		{
		case 14:
			Hero.HP -= 100;
			PlaySound(L"crit.wav", NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 15:
			Hero.HP -= 300;
			PlaySound(L"special2.wav", NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 28:
			Hero.HP *= 0.75;
			PlaySound(L"special3.wav", NULL, SND_FILENAME | SND_ASYNC);
			break;
		case 48:
			Hero.HP = Hero.HP * 2 / 3;
			PlaySound(L"special4.wav", NULL, SND_FILENAME | SND_ASYNC);
			break;
		default:
			break;
		}
		
	}
	if ((g_battletime+20 )% 40 == 0)
		{
		    srand((unsigned)time(NULL));
			if (rand() % 19 == 1)
			{ 
				g_CopyMonster.HP -= (Hero.damage - g_CopyMonster.defence) * 2;
				PlaySound(L"crit.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			}
			else
			{
				g_CopyMonster.HP -= (Hero.damage - g_CopyMonster.defence);
				PlaySound(L"knife.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			}
			
			if (g_CopyMonster.HP<=0)
			{
				Hero.exp += g_CopyMonster.exp;
				Hero.goad += g_CopyMonster.goad;
				g_battletime = 0;
				mciSendString(L"close battle", NULL, 0, NULL);
				g_battle = FALSE;
				g_battlegain = TRUE;
				PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
				g_maze[g_PileNo][Game_NextArrayPoint().x][Game_NextArrayPoint().y] = 0;
			}
		}
		else if (g_battletime % 40 == 0)
		{
			if (g_CopyMonster.damage - Hero.defence>0)
			{
				Hero.HP -= (g_CopyMonster.damage - Hero.defence);
				PlaySound(L"knife.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			}
			else {
				if (Hero.weapon == 0 && Hero.armor == 0)
				{
					PlaySound(L"knife.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
				}
				else if (Hero.weapon == 0 && Hero.armor != 0)
				{
					PlaySound(L"konckdefence.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
				}
				else if (Hero.weapon != 0 && Hero.armor == 0)
				{
					PlaySound(L"sword.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
				}
				else if (Hero.weapon != 0 && Hero.armor != 0)
				{
					srand((unsigned)time(NULL));
					rand() % 2 == 0 ? PlaySound(L"konckdefence.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP) : PlaySound(L"sword.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
				}
			}
		}
}
//判断下一步的动作
VOID Game_Judge(int &next)
{
	switch (next)
	{
	case BRONZEWGATE:
		if (Hero.bronzekey>0)
		{
			Hero.bronzekey -= 1;
			next = 0;
			PlaySound(L"unlock.wav", NULL, SND_FILENAME | SND_ASYNC );
		}
		break;
	case SILVERGATE:
		if (Hero.silverkey>0)
		{
			Hero.silverkey -= 1;
			next = 0;
			PlaySound(L"unlock.wav", NULL, SND_FILENAME | SND_ASYNC );
		}
		break;
	case GOADGATE:
		if (Hero.goadkey>0)
		{
			Hero.goadkey -= 1;
			next = 0;
			PlaySound(L"unlock.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
		break;
	case UP:
		g_swichup = TRUE;
		PlaySound(L"up.wav", NULL, SND_FILENAME | SND_ASYNC );
		g_swiching = TRUE;
		if ((g_PileNo + 1) <= 20 && (g_PileNo+1) > g_maxpile)
		{
			g_maxpile = (g_PileNo + 1);
		    if (g_maxpile==1)
		    {
 				mciSendString(L"open ../Debug/Danger.wav alias back", NULL, 0, NULL);
				mciSendString(L"play back", NULL, 0, NULL);
				g_bgm = TRUE;
				g_bgmtime = 0;
			}
			if (g_maxpile==8)
			{
				mciSendString(L"close back", NULL, 0, NULL);
				mciSendString(L"open ../Debug/Battle_Elder01.wav alias back", NULL, 0, NULL);
				mciSendString(L"play back", NULL, 0, NULL);
				g_bgm = TRUE;
				g_bgmtime = 0;
			}
			if (g_maxpile==15)
			{
				mciSendString(L"close back", NULL, 0, NULL);
				mciSendString(L"open ../Debug/Battle_Elder02.wav alias back", NULL, 0, NULL);
				mciSendString(L"play back", NULL, 0, NULL);
				g_bgm = TRUE;
				g_bgmtime = 0;
			}
			if (g_maxpile==19)
			{
				mciSendString(L"close back", NULL, 0, NULL);
				mciSendString(L"open ../Debug/Last_war.wav alias back", NULL, 0, NULL);
				mciSendString(L"play back", NULL, 0, NULL);
				g_bgm = TRUE;
				g_bgmtime = 0;
			}
		}
		if (g_maxpile>=17)
		{
			g_monster[14][0] = { 1150,1050,3100,80,92 };
			g_monster[2][3] = {1200,1133,3333,112,126};
			g_monster[11][0] = {1306,1200,1600,112,132};
			g_monster[12][0] = {1106,973,105,120};
		}
		if (g_maxpile>=19)
		{
			g_monster[13][0] = {1333,1333,20000,133,133};
		}
		if (g_maxpile == 21)
		{
			g_monster[1][3] = { 2000,1600,33333,330,375 };
		}
		break;
	case DOWN:
		g_swichup = FALSE;
		PlaySound(L"up.wav", NULL, SND_FILENAME | SND_ASYNC );
		g_swiching = TRUE;
		break;
	default:
		break;
	}
	if (next >= 20 && next < 1000)
	{
		if (Game_Battle(next - 20))
		{
			g_battle = TRUE;
			if (g_blood)
			{
				mciSendString(L"open ../Debug/Last_war.wav alias battle", NULL, 0, NULL);
				mciSendString(L"play battle", NULL, 0, NULL);
			}
			else
			{
				mciSendString(L"open ../Debug/BabyGoGoGo.wav alias battle", NULL, 0, NULL);
				mciSendString(L"play battle", NULL, 0, NULL);
			}
		}
	}
	if (next >= 1000)
	{
		g_talking = TRUE;
	}
}
//绘制与NPC对话框以及文字
VOID Game_TalkingPaint()
{
	SelectObject(g_mdc, g_battlepen);
	SelectObject(g_mdc, g_battlebrush);
	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	HFONT hFont2 = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	HFONT hFont3 = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
	wchar_t msg[20];
	
	switch (g_maze[g_PileNo][Game_NextArrayPoint().x][Game_NextArrayPoint().y])
	{
	case 1003:
		if (g_maze[0][4][8] != 1003)
		{
			switch (g_talkingNo)
			{
			case 0:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"……");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 1:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"你醒了！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 2:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"……");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"你是谁？我在哪里？");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 3:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"我是这里的仙子，刚才你被这");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"里的小怪打昏了。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 4:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"……");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"剑，剑，我的剑呢？");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 5:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"你的剑被他们抢走了，我只来");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"得及将你救出来。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 6:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"那，公主呢？我是来救公主的");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"。");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 7:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主还在里面，你这样进去是");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"打不过里面的小怪的。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 8:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"那我怎么办，我答应了国王一");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"定要把公主救出来的，那我现");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 9:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"在应该怎么办呢？");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 10:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"放心吧，我把我的力量借给你");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"，你就可以打赢那些小怪了。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 11:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"不过，你得先帮我去找一样东");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"西，找到了再来这里找我。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 12:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"找东西？找什么东西？");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 13:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"是一个十字架，中间有一颗红");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"色的宝石。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 14:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"那个东西有什么用么？");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 15:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"我本是这座塔的守护者，可不");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"久前，从北方来了一批恶魔，");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 16:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"他们占领了这座塔，并将我的");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"魔力封在了这个十字架里面，");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 17:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"如果你能将它带出塔来，那我");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"的魔力便会慢慢地恢复，到那");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 18:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"时我便可以把力量借给你去救");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"出公主了。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 19:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"……");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"好吧，我试试看。");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 20:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"刚才我去看过了，你的剑被放");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"在三楼，你的盾在五楼上，而");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 21:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"那个十字架被放在七楼。要到");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"七楼，你得先取回你的剑和盾。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 22:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"另外，在塔里的其他楼层上，");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"还有一些存放了好几百年的宝");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 23:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"剑和宝物，如果得到它们，对");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"于你对付这里面的怪物将有很");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 24:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"大帮助。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 25:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"……");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"可是，我怎么进去呢？");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 26:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"我这里有三把钥匙，你先拿去");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"，在塔里面还有很多这样的钥");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				Hero.bronzekey = 1;
				Hero.silverkey = 1;
				Hero.goadkey = 1;
				break;
			case 27:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"匙，你一定要珍稀使用。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 28:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"仙子：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇敢的去吧，勇士！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 29:
				PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC );
				g_talkingNo = 0;
				g_talking = FALSE;
				g_maze[g_PileNo][Game_NextArrayPoint().x][Game_NextArrayPoint().y] = 0;
				g_maze[g_PileNo][Game_NextArrayPoint().x - 1][Game_NextArrayPoint().y] = 1003;
				break;
			default:
				g_talkingNo = 0;
				g_talking = FALSE;
				break;
			}
		}
		else
		{
			if (Hero.cross)
			{
				switch (g_talkingNo)
				{
				case 0:
					Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hSprite[2]);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"勇士：");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"仙子，我已经将那个十字架找");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"到了。");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 1:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hNPC[0]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"仙子：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"你做的很好。");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"那么，现在我就开始授予你更");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 2:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hNPC[0]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"仙子：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"强的力量！");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"咪啦哆咪哗……");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 3:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hNPC[0]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"仙子：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"好了，我已经将你现在的能力");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"提升了！");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 4:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hNPC[0]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"仙子：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"记住：如果你没有足够的实力");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"的话，不要去第二十一层！在");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 5:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hNPC[0]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"仙子：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"那一层里，你所有宝物的法力");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"都会失去作用！");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				default:
					g_talkingNo = 0;
					g_talking = FALSE;
					Hero.defence *= 1.33;
					Hero.damage *= 1.33;
					Hero.HP *= 1.33;
					Hero.cross = FALSE;
					g_maze[g_PileNo][Game_NextArrayPoint().x][Game_NextArrayPoint().y] = 0;
					PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
					break;
				}
			}
			else
			{
				g_talking = FALSE;
			}
		
		}
		break;
	case 1006:
		if (g_PileNo == 3)
		{
			switch (g_talkingNo)
			{
			case 0:
				Rectangle(g_mdc, 6.5 * SQUIREUNITWIDTH, 2.5*SQUIREUNITHEIGHT, 13.1 * SQUIREUNITWIDTH, 9 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 6.5 * SQUIREUNITWIDTH + 10, 2.5 * SQUIREUNITHEIGHT + 20, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont3);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商 店 老 板");
				TextOut(g_mdc, 8 * SQUIREUNITWIDTH, 2.9*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont);
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"嗨，你好，英雄的勇士，这里是怪物");
				TextOut(g_mdc, 7.0 * SQUIREUNITWIDTH - 1, 4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商店，这里告诉你一些操作方法：");
				TextOut(g_mdc, 6.6 * SQUIREUNITWIDTH, 4.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"使用方向键↑和↓可以在菜单中进行");
				TextOut(g_mdc, 7.0 * SQUIREUNITWIDTH - 1, 5.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"选择，使用空格键可以用来确认你的选");
				TextOut(g_mdc, 6.6 * SQUIREUNITWIDTH, 5.8 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"择！");
				TextOut(g_mdc, 6.6 * SQUIREUNITWIDTH, 6.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"同时，在商人或神秘老人处进行交易");
				TextOut(g_mdc, 7.0 * SQUIREUNITWIDTH - 1, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"也是一样的操作方法！");
				TextOut(g_mdc, 6.6 * SQUIREUNITWIDTH, 7.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"知道了吗？勇士！");
				TextOut(g_mdc, 7.0 * SQUIREUNITWIDTH - 1, 8.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 12 * SQUIREUNITWIDTH, 8.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			default:
				g_seletobjectsNo = 4;
				g_selectofnow = 0;
				g_talkingNo = 0;
				g_talking = FALSE;
				g_selecting = TRUE;
				g_talkingtime = 0;
				break;
			}
		}
		else
		{
			g_seletobjectsNo = 4;
			g_selectofnow = 0;
			g_talkingNo = 0;
			g_talking = FALSE;
			g_selecting = TRUE;
			g_talkingtime = 0;
		}
		break;
	case 1000:
		if (g_PileNo == 2)
		{
			switch (g_talkingNo)
			{
			case 0:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"您已经得救了！");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 1:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"神秘老人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"哦，我的孩子，真是太感谢你");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"了！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 2:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"神秘老人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"这个地方又脏又坏，我真的是");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"快待不下去了。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 3:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"快走吧，我还得去救被关在这");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"里的公主。");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 4:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"神秘老人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"哦，你是来救公主的，为了表");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"示对你的感谢，这个东西就送");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 5:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"神秘老人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"给你吧，这还是我年轻的时候");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"用过的。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 6:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"神秘老人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"拿着它去解救公主吧！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			default:
				g_talkingNo = 0;
				g_talkingtime = 0;
				g_talking = FALSE;
				Hero.damage += 30;
				g_maze[g_PileNo][Game_NextArrayPoint().x][Game_NextArrayPoint().y] = 0;
				PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
				g_gainobject = GAINSTEELSWORD;
				g_gain = TRUE;
				
				break;
			}
			break;//对话结束break跳出防止进入选择页面
		}
		else if (g_PileNo==15)
		{
			switch (g_talkingNo)
			{
			case 0:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"神秘老人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"你好，勇敢的孩子，你终于来");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"到这里了。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 1:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"神秘老人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"我将给你一个非常好的宝物，");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"它可以使你的攻击力提升120");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 2:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"神秘老人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"点，但这必须得用你的500点经");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"验来进行交换，考虑一下子吧！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 3:
				if (Hero.exp < 500)
				{
					g_talkingNo = 0;
					g_talkingtime = 0;
					g_talking = FALSE;
					break;
				}
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"好吧，那就将那把剑给我吧！");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				//memset(msg, 0, sizeof(msg));
				//swprintf_s(msg, L"里的公主。");
				//TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 4:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"神秘老人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"那好吧，这把剑就给你了！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			default:
				Hero.exp -= 500;
				Hero.damage += 120;
				g_gainobject = GAINSTARLIGHTSWORD;
				g_gain = TRUE;
				g_maze[g_PileNo][Game_NextArrayPoint().x][Game_NextArrayPoint().y] = 0;
				PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
				g_talkingNo = 0;
				g_talkingtime = 0;
				g_talking = FALSE;

				break;
			}
			break;
		}
		g_seletobjectsNo = 4;
		g_selectofnow = 0;
		g_talkingNo = 0;
		g_talking = FALSE;
		g_selecting = TRUE;
		break;
	case 1001:
		if (g_PileNo == 2)
		{
			switch (g_talkingNo)
			{
			case 0:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"您已经得救了！");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 1:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH,  SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"哦，是嘛！真是太感谢你了！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 2:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"我是个商人，不知为什么被抓");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"到这里来了。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 3:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"快走吧，现在你已经自由了。");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 4:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"哦，对对对，我已经自由了。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 5:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"那这个东西就给你吧，本来我");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"是准备卖钱的。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 6:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"相信它对你一定有很大帮助！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;

			default:
				g_talkingNo = 0;
				g_talkingtime = 0;
				g_talking = FALSE;
				Hero.defence += 30;
				PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
				g_gainobject = GAINSTEELARMOR;
				g_gain = TRUE;
				g_maze[g_PileNo][Game_NextArrayPoint().x][Game_NextArrayPoint().y] = 0;
				break;
			}
			break;//对话结束break跳出，防止进入选择页面
		}
		else if (g_PileNo == 15)
		{
			switch (g_talkingNo)
			{
			case 0:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"啊哈，欢迎你的到来！我这里");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"有一件对你来说非常好的宝物，");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 1:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"只要你出得起钱，我就卖给你");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				//memset(msg, 0, sizeof(msg));
				//swprintf_s(msg, L"");
				//TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 2:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"什么宝物？要多少钱？");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 3:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"是这个游戏最好的盾牌，防御");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"值可以增加120点，而你只要");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 4:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"出500个金币就可以买下。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 5:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"怎么样？你有500个金币吗？");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 6:
				if (Hero.goad<500)
				{
					g_talkingNo = 0;
					g_talkingtime = 0;
					g_talking = FALSE;
					break;
				}
				else
				{
					Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hSprite[2]);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"勇士：");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"我有500个金币。");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				}
			case 7:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"商人：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"好，成交！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			default:
				Hero.goad -= 500;
				Hero.defence += 120;
				g_gainobject = GAINSTARLIGHTARMOR;
				g_gain = TRUE;
				g_maze[g_PileNo][Game_NextArrayPoint().x][Game_NextArrayPoint().y] = 0;
				PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
				g_talkingNo = 0;
				g_talkingtime = 0;
				g_talking = FALSE;
				break;
			}
			break;
			
		}
		g_seletobjectsNo = 4;
		g_selectofnow = 0;
		g_talkingNo = 0;
		g_talking = FALSE;
		g_selecting = TRUE;
		break;
	case 1002:
		if (g_thieftalkingNo == 0)
		{
			switch (g_talkingNo)
			{
			case 0:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"你已经得救了！");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 1:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"啊，那真是太好了，我又可以");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"在这里面寻宝了！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 2:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"哦，还没有自我介绍，我叫杰");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"克，是这附近有名的小偷，什");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 3:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"么金银财宝我样样都偷过。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 4:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"不过这次运气可不是太好，刚");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"进来就被抓了。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 5:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"现在你帮我打开了门，那我就");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"帮你做一件事吧。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 6:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"快走吧，外面还有很多怪物，");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"我可能顾不上你。");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 7:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"不，不，不会有事的。快说吧");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"，叫我做什么？");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 8:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"……");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"你会开门么？");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 9:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"那当然。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 10:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"那就请你帮我打开第二层的");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"门吧！");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 11:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"那个简单，不过，如果你能帮");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"我找到一把嵌了红宝石的铁榔");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 12:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"头的话，我还帮你打通第十八");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"层的路。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 13:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"嵌了红宝石的铁榔头？好吧，");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"我帮你找找。");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 14:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"非常的感谢。一会我便会将第");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"二层的门打开。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 15:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"如果你找到那个铁榔头的话，");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"还是来这里找我！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			default:
				g_talkingNo = 0;
				g_talkingtime = 0;
				g_talking = FALSE;
				g_thieftalkingNo = 1;
				g_maze[2][1][6] = 0;
				break;
			}
		}
		else if (Hero.hammer)
		{
			switch (g_talkingNo)
			{
			case 0:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"蛤，快看，我找到了什么！");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 1:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"太好了，这个东西果然是在这");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"里。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 2:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[0]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"小偷：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"好吧，我这就去帮你修好第十");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"八层的路面。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			default:
				g_talkingNo = 0;
				g_talkingtime = 0;
				g_talking = FALSE;
				g_maze[18][5][8] = 0;
				g_maze[18][5][9] = 0;
				g_maze[g_PileNo][Game_NextArrayPoint().x][Game_NextArrayPoint().y] = 0;
				break;
			}
		}
		else
		{
			g_talkingNo = 0;
			g_talkingtime = 0;
			g_talking = FALSE;
		}
		break;
    case 1007:
		if (g_princessNo == 0)
		{
			switch (g_talkingNo)
			{
			case  0:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主！你得救了！");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 1:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"啊，你是来救我的吗？");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 2:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"是的，我是奉国王的命令来救");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"你的。");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 3:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"请你快随我出去吧！");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 4:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"不，我还不想走。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 5:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"为什么？这里面到处都是恶魔");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"。");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 6:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"正是因为这里面到处都是恶魔");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"，所以才不可以就这样出去，");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 7:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"我要看着那个恶魔被杀死！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 8:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"英雄的勇士，如果你能够将那");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"个大恶魔杀死，我就和你一起");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 9:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"出去！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 10:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"大恶魔？我已经杀死了一个魔");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"王！");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 11:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"大恶魔在这座塔的最顶端，你");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"杀死的可能是一个小队长之类");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 12:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"的恶魔。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 13:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"好，那你等着，等我杀了那个");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"恶魔再来这里找你！");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 14:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"大恶魔比你刚才杀死的那个厉");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"害多了。");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 15:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"而且他还会变身，变身后的魔");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"王他的攻击力和防御力都会提");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 16:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"升至少一半以上，你要小心！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 17:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hNPC[1]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"请一定要杀死大魔王！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			default:
				g_talkingNo = 0;
				g_talkingtime = 0;
				g_talking = FALSE;
				g_princessNo = 1;
				g_maze[18][10][10] = UP;
				break;
			}
		}
		break;
	case 0:
		switch (g_PileNo)
		{
		case 16:
			switch (g_talkingNo)
			{
			case 0:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"……");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 1:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hMonster[13]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"红衣魔王：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"停止吧！愚蠢的人类！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 2:
				Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hSprite[2]);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
				BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"勇士：");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"该停止的是你！魔王。快说，");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"公主关在哪里？");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			case 3:
				Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
				SelectObject(g_bufdc, g_hMonster[13]);
				TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
				SelectObject(g_mdc, hFont);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255, 255, 255));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"红衣魔王：");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"等你打赢我再说吧！");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont2);
				SetBkMode(g_mdc, TRANSPARENT);
				SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"--space--");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				break;
			default:
				g_talkingNo = 0;
				g_talkingtime = 0;
				g_talking = FALSE;
				g_RedNo = FALSE;
				break;
			}
			break;
		case 19:
			if (Game_NextArrayPoint().x == 5 && Game_NextArrayPoint().y == 7)
			{
				switch (g_talkingNo)
				{
				case 0:
					Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hSprite[2]);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"勇士：");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"大魔头，你的死期到了！");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 1:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[1]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"冥灵魔王：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"哈哈哈……");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"你也真是有意思，别以为蝶仙");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 2:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[1]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"冥灵魔王：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"那家伙给了你力量你就可以打");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"败我，想打败我你还早着呢！");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 3:
					Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hSprite[2]);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"勇士：");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"废话少说，去死吧！");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				default:
					g_talkingNo = 0;
					g_talkingtime = 0;
					g_talking = FALSE;
					g_BlackNo = FALSE;
					break;
				}
			}
			else
			{
				switch (g_talkingNo)
				{
				case 0:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[1]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"冥灵魔王：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"看不出你还有两下子，有本事");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"的话来21楼。");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 1:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[1]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"冥灵魔王：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"在那里，你就可以见识到我真");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"正的实力了！");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				default:
					g_talkingNo = 0;
					g_talkingtime = 0;
					g_talking = FALSE;
					break;
				}
			}
			break;
		case 21:
			if (!g_blood)
			{
				int maze21[11][11] = { { 2,2,2,2,2,2,2,2,2,2,2 },{ 2,2,2,2,2,0,2,2,2,2,2 },{ 2,2,2,2,2,0,2,2,2,2,2 },{ 2,2,2,2,2,0,2,0,2,2,2 },{ 2,0,0,0,2,0,2,0,2,0,2 },{ 2,0,0,80,0,0,0,0,0,0,0 },{ 2,0,0,0,2,0,2,0,2,0,2 },{ 2,2,2,2,2,0,2,0,2,2,2 },{ 2,2,2,2,2,0,2,2,2,2,2 },{ 2,2,2,2,2,0,2,2,2,2,2 },{ 2,2,2,2,2,2,2,2,2,2,2 } };
				int m = 0;
				switch (g_talkingNo)
				{
				case 0:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[1]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"冥灵魔王：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"啊……");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"怎么可能，我怎么可能会被你");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 1:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[1]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"冥灵魔王：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"打败！");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"不，我是最强的魔王！！！");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 2:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[1]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"冥灵魔王：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"昂——！！！！！");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 3:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[1]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 3 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"愤怒的魔王展开了究极变身。");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 4:
					SelectObject(g_mdc, CreateSolidBrush(RGB(255, 255, 255)));
					Rectangle(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
					SelectObject(g_mdc, g_battlepen);
					SelectObject(g_mdc, g_battlebrush);
					Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hSprite[2]);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"勇士：");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"!!!!!!!!");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 5:
					for (int i = 0; i < 11; i++)
					{
						m = i;
						for (int i = 0; i < 11; i++)
						{
							g_maze[21][m][i] = maze21[m][i];
						}
					}
					Hero.Position = { 9 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT };
					SelectObject(g_mdc, g_battlepen);
					SelectObject(g_mdc, g_battlebrush);
					Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hSprite[2]);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"勇士：");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"原来你就是那个为祸四方的血");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"影！");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 6:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[15]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH+SQUIREUNITWIDTH/3, SQUIREUNITHEIGHT*2/3, SQUIREUNITWIDTH/3, SQUIREUNITHEIGHT/3, RGB(255, 255, 255));
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"血影：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"算你眼力好，不错，我就是血");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"影，能把我逼到显现原形的份");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 7:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[15]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH + SQUIREUNITWIDTH / 3, SQUIREUNITHEIGHT * 2 / 3, SQUIREUNITWIDTH / 3, SQUIREUNITHEIGHT / 3, RGB(255, 255, 255));					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"血影：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"上，你还是头一个，不过马上");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"就是最后一个了。");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 8:
					SelectObject(g_mdc, g_battlepen);
					SelectObject(g_mdc, g_battlebrush);
					Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hSprite[2]);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"勇士：");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"这也是你最后一次显现原形了");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"今天我就替天行道，斩了你这");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 8.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 9:
					SelectObject(g_mdc, g_battlepen);
					SelectObject(g_mdc, g_battlebrush);
					Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hSprite[2]);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"勇士：");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"个魔头，以绝后患！");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				default:
					g_talkingNo = 0;
					g_talkingtime = 0;
					g_talking = FALSE;
					g_blood = TRUE;
					g_bgm = TRUE;
					g_bgmtime = 0;
					mciSendString(L"close back", NULL, 0, NULL);
					mciSendString(L"open ../Debug/Final_Power.wav alias back", NULL, 0, NULL);
					mciSendString(L"play back", NULL, 0, NULL);
					break;
				}
			}
			else
			{
				switch (g_talkingNo)
				{
				case 0:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[15]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH + SQUIREUNITWIDTH / 3, SQUIREUNITHEIGHT * 2 / 3, SQUIREUNITWIDTH / 3, SQUIREUNITHEIGHT / 3, RGB(255, 255, 255));					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"血影：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"咳咳……好一个星神剑，本");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"座纵横一世，却败在了无名");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 1:
					Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 3.5*SQUIREUNITHEIGHT, 11.25*SQUIREUNITWIDTH, 6 * SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hMonster[15]);
					TransparentBlt(g_mdc, 4.25 * SQUIREUNITWIDTH, 3.75 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH + SQUIREUNITWIDTH / 3, SQUIREUNITHEIGHT * 2 / 3, SQUIREUNITWIDTH / 3, SQUIREUNITHEIGHT / 3, RGB(255, 255, 255));					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"血影：");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"小卒手中,等我再次醒来一定");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"会拿回我拥有的一切！！");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 5.15*SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 5.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				case 2:
					SelectObject(g_mdc, g_battlepen);
					SelectObject(g_mdc, g_battlebrush);
					Rectangle(g_mdc, 7.5*SQUIREUNITWIDTH, 6.75*SQUIREUNITHEIGHT, 14.75*SQUIREUNITWIDTH, 9.25*SQUIREUNITHEIGHT);
					SelectObject(g_bufdc, g_hSprite[2]);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, SQUIREUNITHEIGHT, SRCAND);
					BitBlt(g_mdc, 7.75 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, 0, 0, SRCPAINT);
					SelectObject(g_mdc, hFont);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255, 255, 255));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"勇士：");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"你等不到那一天了，受死吧！");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					SelectObject(g_mdc, hFont2);
					SetBkMode(g_mdc, TRANSPARENT);
					SetTextColor(g_mdc, RGB(255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20, 255 - abs(g_talkingtime % 40 - 20) * 255 / 20));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"--space--");
					TextOut(g_mdc, 13 * SQUIREUNITWIDTH, 8.85*SQUIREUNITHEIGHT, msg, wcslen(msg));
					break;
				default:
					g_talkingNo = 0;
					g_talkingtime = 0;
					g_talking = FALSE;
					g_end = TRUE;
					mciSendString(L"close back", NULL, 0, NULL);
					mciSendString(L"open ../Debug/Win_road.wav alias back", NULL, 0, NULL);
					mciSendString(L"play back", NULL, 0, NULL);
					g_bgmtime = 0;
					break;
				}
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	DeleteObject(hFont3);
	DeleteObject(hFont2);
	DeleteObject(hFont);
}
//进行选择相关的操作
VOID Game_Select()
{
	SelectObject(g_mdc, g_battlepen);
	SelectObject(g_mdc, g_battlebrush);
	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	HFONT hFont2 = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	HFONT hFont3 = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	HPEN selectpen = CreatePen(PS_SOLID, 5, RGB(255 - abs(g_selecttime % 40 - 20) * 255 / 20, 255 - abs(g_selecttime % 40 - 20) * 255 / 20, 255 - abs(g_selecttime % 40 - 20) * 255 / 20));
	wchar_t msg[20];
	switch (g_maze[g_PileNo][Game_NextArrayPoint().x][Game_NextArrayPoint().y])
	{
	case 1006:
		Rectangle(g_mdc, 6.5 * SQUIREUNITWIDTH, 2.5*SQUIREUNITHEIGHT, 13.1 * SQUIREUNITWIDTH, 9 * SQUIREUNITHEIGHT);
		SelectObject(g_bufdc, g_hNPC[1]);
		TransparentBlt(g_mdc, 6.5 * SQUIREUNITWIDTH + 10, 2.5 * SQUIREUNITHEIGHT + 20, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 2 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
		SelectObject(g_mdc, hFont3);
		memset(msg, 0, sizeof(msg));
		swprintf_s(msg, L"想要增加你的能力吗？");
		TextOut(g_mdc, 8.1 * SQUIREUNITWIDTH, 2.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		if (g_PileNo == 3)
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"如果你有25个金币，你可以选择");
			TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		}
		else
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"如果你有100个金币，你可以选择");
			TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		}
		memset(msg, 0, sizeof(msg));
		swprintf_s(msg, L"任一项：");
		TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		SelectObject(g_mdc, selectpen);
		Rectangle(g_mdc, 6.75*SQUIREUNITWIDTH, 4.8*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT, 12.85*SQUIREUNITWIDTH, 5.55*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT);
		if (g_PileNo == 3)
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"增加 800 点生命");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"增加 4 点攻击");
			TextOut(g_mdc, 8.5 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"增加 4 点防御");
			TextOut(g_mdc, 8.5 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"离开商店");
			TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7.25*SQUIREUNITHEIGHT, msg, wcslen(msg));
		}
		else
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"增加 4000 点生命");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"增加 20 点攻击");
			TextOut(g_mdc, 8.5 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"增加 20 点防御");
			TextOut(g_mdc, 8.5 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"离开商店");
			TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7.25*SQUIREUNITHEIGHT, msg, wcslen(msg));

		}
		if (g_seleckeydown == TRUE)
		{
			if (g_PileNo == 3)
			{
				switch (g_selectofnow)
				{
				case 0:
					if (Hero.goad >= 25)
					{
						Hero.goad -= 25;
						Hero.HP += 800;
						PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC );
					}
					break;
				case 1:
					if (Hero.goad >= 25)
					{
						Hero.goad -= 25;
						Hero.damage += 4;
						PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC );
					}
					break;
				case 2:
					if (Hero.goad >= 25)
					{
						Hero.goad -= 25;
						Hero.defence += 4;
						PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC );
					}
					break;
				case 3:
					g_selecting = FALSE;
					g_selecttime = 0;
					g_selectofnow = 0;
					g_seletobjectsNo = 0;
					PlaySound(L"leave.wav", NULL, SND_FILENAME | SND_ASYNC );
					break;
				default:
					break;
				}
			}
			else
			{
				switch (g_selectofnow)
				{
				case 0:
					if (Hero.goad >= 100)
					{
						Hero.goad -= 100;
						Hero.HP += 4000;
						PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
					}
					break;
				case 1:
					if (Hero.goad >= 100)
					{
						Hero.goad -= 100;
						Hero.damage += 20;
						PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
					}
					break;
				case 2:
					if (Hero.goad >= 100)
					{
						Hero.goad -= 100;
						Hero.defence += 20;
						PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
					}
					break;
				case 3:
					g_selecting = FALSE;
					g_selecttime = 0;
					g_selectofnow = 0;
					g_seletobjectsNo = 0;
					PlaySound(L"leave.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
					break;
				default:
					break;
				}
			}
			g_seleckeydown = FALSE;
		}
		break;
	case 1000:
		Rectangle(g_mdc, 6.5 * SQUIREUNITWIDTH, 2.5*SQUIREUNITHEIGHT, 13.1 * SQUIREUNITWIDTH, 9 * SQUIREUNITHEIGHT);
		SelectObject(g_bufdc, g_hNPC[0]);
		TransparentBlt(g_mdc, 6.5 * SQUIREUNITWIDTH + 10, 2.5 * SQUIREUNITHEIGHT + 20, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
		SelectObject(g_mdc, hFont3);
		memset(msg, 0, sizeof(msg));
		swprintf_s(msg, L"你好，英雄的人类，只要你有");
		TextOut(g_mdc, 8.1 * SQUIREUNITWIDTH, 2.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		memset(msg, 0, sizeof(msg));
		swprintf_s(msg, L"足够的经验，我就可以让你变得");
		TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		memset(msg, 0, sizeof(msg));
		swprintf_s(msg, L"更强大：");
		TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		SelectObject(g_mdc, selectpen);
		Rectangle(g_mdc, 6.75*SQUIREUNITWIDTH, 4.8*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT, 12.85*SQUIREUNITWIDTH, 5.55*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT);
		if (g_PileNo == 5)
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"提升一级（需要100点）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"增加攻击5（需要30点）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"增加防御5（需要30点）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"离开商店");
			TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7.25*SQUIREUNITHEIGHT, msg, wcslen(msg));
			if (g_seleckeydown == TRUE)
			{
				switch (g_selectofnow)
				{
				case 0:
					if (Hero.exp >= 100)
					{
						Hero.exp -= 100;
						Hero.damage += 7;
						Hero.defence += 7;
						PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 1:
					if (Hero.exp >= 30)
					{
						Hero.exp -= 30;
						Hero.damage += 5;
						PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 2:
					if (Hero.exp >= 30)
					{
						Hero.exp -= 30;
						Hero.defence += 5;
						PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 3:
					g_selecting = FALSE;
					g_selecttime = 0;
					g_selectofnow = 0;
					g_seletobjectsNo = 0;
					PlaySound(L"leave.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
					break;
				default:
					break;
				}
				g_seleckeydown = FALSE;
			}

		}
		else
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"提升三级（需要270点）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"增加攻击17（需要95点）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"增加防御17（需要95点）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"离开商店");
			TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7.25*SQUIREUNITHEIGHT, msg, wcslen(msg));
			if (g_seleckeydown == TRUE)
			{
				switch (g_selectofnow)
				{
				case 0:
					if (Hero.exp >= 270)
					{
						Hero.exp -= 270;
						Hero.damage += 21;
						Hero.defence += 21;
						PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 1:
					if (Hero.exp >= 95)
					{
						Hero.exp -= 95;
						Hero.damage += 17;
						PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 2:
					if (Hero.exp >= 95)
					{
						Hero.exp -= 95;
						Hero.defence += 17;
						PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 3:
					g_selecting = FALSE;
					g_selecttime = 0;
					g_selectofnow = 0;
					g_seletobjectsNo = 0;
					PlaySound(L"leave.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
					break;
				default:
					break;
				}
				g_seleckeydown = FALSE;
			}
		}
			break;
	case 1001:
		Rectangle(g_mdc, 6.5 * SQUIREUNITWIDTH, 2.5*SQUIREUNITHEIGHT, 13.1 * SQUIREUNITWIDTH, 9 * SQUIREUNITHEIGHT);
		SelectObject(g_bufdc, g_hNPC[0]);
		TransparentBlt(g_mdc, 6.5 * SQUIREUNITWIDTH + 10, 2.5 * SQUIREUNITHEIGHT + 20, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
		SelectObject(g_mdc, hFont3);
		if (g_PileNo == 5)
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"相信你一定有特殊的需要，只");
			TextOut(g_mdc, 8.1 * SQUIREUNITWIDTH, 2.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"你有金币，我就可以帮你：");
			TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			SelectObject(g_mdc, selectpen);
			Rectangle(g_mdc, 6.75*SQUIREUNITWIDTH, 4.8*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT, 12.85*SQUIREUNITWIDTH, 5.55*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT);
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"购买一把铜钥匙（$ 10）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"购买一把银钥匙（$ 50）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"购买一把金钥匙（$100）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"离开商店");
			TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7.25*SQUIREUNITHEIGHT, msg, wcslen(msg));
			if (g_seleckeydown == TRUE)
			{
				switch (g_selectofnow)
				{
				case 0:
					if (Hero.goad >= 10)
					{
						Hero.goad -= 10;
						Hero.bronzekey++;
						PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 1:
					if (Hero.goad >= 50)
					{
						Hero.goad -= 50;
						Hero.silverkey++;
						PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 2:
					if (Hero.goad >= 100)
					{
						Hero.goad -= 100;
						Hero.goadkey++;
						PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 3:
					g_selecting = FALSE;
					g_selecttime = 0;
					g_selectofnow = 0;
					g_seletobjectsNo = 0;
					PlaySound(L"leave.wav", NULL, SND_FILENAME | SND_ASYNC);
					break;
				default:
					break;
				}
				g_seleckeydown = FALSE;
			}
		}
		else
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"哦，欢迎你的到来，如果你手");
			TextOut(g_mdc, 8.1 * SQUIREUNITWIDTH, 2.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"里缺少金币，我可以帮你：");
			TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			SelectObject(g_mdc, selectpen);
			Rectangle(g_mdc, 6.75*SQUIREUNITWIDTH, 4.8*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT, 12.85*SQUIREUNITWIDTH, 5.55*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT);
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"卖出一把铜钥匙（$ 7）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"卖出一把银钥匙（$ 35）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"卖出一把金钥匙（$ 70）");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"离开商店");
			TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7.25*SQUIREUNITHEIGHT, msg, wcslen(msg));
			if (g_seleckeydown == TRUE)
			{
				switch (g_selectofnow)
				{
				case 0:
					if (Hero.bronzekey > 0)
					{
						Hero.bronzekey--;
						Hero.goad += 7;
						PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 1:
					if (Hero.silverkey > 0)
					{
						Hero.silverkey--;
						Hero.goad += 35;
						PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 2:
					if (Hero.goadkey > 0)
					{
						Hero.goadkey -= 1;
						Hero.goad += 70;
						PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					break;
				case 3:
					g_selecting = FALSE;
					g_selecttime = 0;
					g_selectofnow = 0;
					g_seletobjectsNo = 0;
					PlaySound(L"leave.wav", NULL, SND_FILENAME | SND_ASYNC);
					break;
				default:
					break;
				}
				g_seleckeydown = FALSE;
			}
		}
		break;
	default:
		break;
	}
	DeleteObject(selectpen);
	DeleteObject(hFont3);
	DeleteObject(hFont2);
	DeleteObject(hFont);
}
//获取道具之后修改人物属性
VOID Game_gain(int &next)
{
	if (next <= -20)
	{
		g_gainobject = next;
		g_gain = TRUE;
	}
	switch (next)
	{
	case -20:
		Hero.bronzekey++;
		next = 0;
		PlaySound(L"gainkey.wav", NULL, SND_FILENAME | SND_ASYNC );
		break;
	case -21:
		Hero.silverkey++;
		next = 0;
		PlaySound(L"gainkey.wav", NULL, SND_FILENAME | SND_ASYNC );
		break;
	case -22:
		Hero.goadkey++;
		next = 0;
		PlaySound(L"gainkey.wav", NULL, SND_FILENAME | SND_ASYNC );
		break;
	case GAINBLUEJEWEL:
		Hero.defence += 3;
		next = 0;
		PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC );
		break;
	case GAINREDJEWEL:
		Hero.damage += 3;
		next = 0;
		PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC );
		break;
	case GAINSMALLBOTTLE:
		Hero.HP += 200;
		next = 0;
		PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC );
		break;
	case GAINBIGBOTTLE:
		Hero.HP += 500;
		next = 0;
		PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC );
		break;
	case GAINSWORD:
		Hero.damage += 10;
		Hero.weapon = GAINSWORD;
		next = 0;
		PlaySound(L"picksword.wav", NULL, SND_FILENAME | SND_ASYNC );
		break;
	case GAINMUTIPLEKEY:
		Hero.bronzekey++;
		Hero.silverkey++;
		Hero.goadkey++;
		next = 0;
		PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC );
		break;
	case GAINARMOR:
		Hero.defence += 10;
		Hero.armor = GAINARMOR;
		next = 0;
		PlaySound(L"picksword.wav", NULL, SND_FILENAME | SND_ASYNC );
		break;
	case GAINRADAR:
		Hero.radar = TRUE;
		next = 0;
		PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC );
		break;
	case GAINSILVERSWORD:
		Hero.damage += 70;
		Hero.weapon = GAINSILVERSWORD;
		next = 0;
		PlaySound(L"picksword.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case GAINSMALLFLY:
		Hero.level++;
		Hero.damage += 10;
		Hero.defence += 10;
		Hero.HP += 1000;
		next = 0;
		PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case GAINBULLION:
		Hero.goad += 300;
		next = 0;
		PlaySound(L"coin.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case GAINGOADARMOR:
		Hero.defence += 85;
		Hero.armor = GAINGOADARMOR;
		next = 0;
		PlaySound(L"picksword.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case GAINHOLYWATER:
		Hero.HP *= 2;
		next = 0;
		PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case GAINCOMPASS:
		Hero.compass = TRUE;
		next = 0;
		PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case GAINCROSS:
		Hero.cross = TRUE;
		next = 0;
		PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case GAINBIGFLY:
		Hero.level += 3;
		Hero.HP += 3000;
		Hero.damage += 30;
		Hero.defence += 30;
		next = 0;
		PlaySound(L"pregain.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case GAINHAMMER:
		Hero.hammer = TRUE;
		next = 0;
		PlaySound(L"picksword.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case GAINGODSWORD:
		Hero.damage += 150;
		Hero.weapon = GAINGODSWORD;
		next = 0;
		PlaySound(L"picksword.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case GAINGODARMOR:
		Hero.defence += 190;
		Hero.armor = GAINGODARMOR;
		next = 0;
		PlaySound(L"picksword.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	default:
		break;
	}
	if (Game_NextArrayPoint().x == 5 && Game_NextArrayPoint().y == 3 && g_PileNo == 16)
	{
		if (g_RedNo)
		{
			g_RedNo = FALSE;
			g_talking = TRUE;
		}
	}
	else if (Game_NextArrayPoint().x == 5 && Game_NextArrayPoint().y == 8 && g_PileNo == 19)
	{
		if (g_BlackNo)
		{
			g_BlackNo = FALSE;
			g_talking = TRUE;
		}
	}
}
//获得道具的提示框绘制
VOID Game_GainPaint(int next)
{

	if (g_gaintime <= 40)
	{
		SelectObject(g_mdc, g_battlepen);
		SelectObject(g_mdc, g_battlebrush);
		Rectangle(g_mdc, 9.5*SQUIREUNITWIDTH - 0.13*g_gaintime*SQUIREUNITWIDTH, 4.5 * SQUIREUNITHEIGHT, 9.5*SQUIREUNITWIDTH + 0.13*g_gaintime*SQUIREUNITWIDTH, 5.5 * SQUIREUNITHEIGHT);
	}
	else if (g_gaintime > 40 && g_gaintime < 80)
	{
		Rectangle(g_mdc, 9.5*SQUIREUNITWIDTH - 5.2*SQUIREUNITWIDTH, 4.5 * SQUIREUNITHEIGHT, 9.5*SQUIREUNITWIDTH + 5.2*SQUIREUNITWIDTH, 5.5 * SQUIREUNITHEIGHT);
		HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
		SelectObject(g_mdc, hFont);
		SetBkMode(g_mdc, TRANSPARENT);
		SetTextColor(g_mdc, RGB(255, 255, 255));
		wchar_t msg[30];
		memset(msg, 0, sizeof(msg));
		switch (next)
		{
		case -20:
			swprintf_s(msg, L"您获得了一把铜钥匙");
			break;
		case -21:
			swprintf_s(msg, L"您获得了一把银钥匙");
			break;
		case -22:
			swprintf_s(msg, L"您获得了一把金钥匙");
			break;
		case GAINBLUEJEWEL:
			swprintf_s(msg, L"您获得了一个蓝宝石，防御力增加3");
			break;
		case GAINREDJEWEL:
			swprintf_s(msg, L"您获得了一个红宝石，攻击力增加3");
			break;
		case GAINSMALLBOTTLE:
			swprintf_s(msg, L"您获得了一个小血瓶，生命值增加200");
			break;
		case  GAINBIGBOTTLE:
			swprintf_s(msg, L"您获得了一个大血瓶，生命值增加500");
			break;
		case GAINSWORD:
			swprintf_s(msg, L"得到铁剑，攻击力加10");
			break;
		case GAINMUTIPLEKEY:
			swprintf_s(msg, L"您获得了金钥匙，银钥匙，铜钥匙各一把!");
			break;
		case GAINARMOR:
			swprintf_s(msg, L"得到铁盾，防御力加10");
			break;
		case GAINRADAR:
			swprintf_s(msg, L"获得怪物雷达！按F1键使用查看本层怪物信息,Esc键退出");
			TextOut(g_mdc, 5 * SQUIREUNITWIDTH, 4.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			goto lop;
			break;
		case GAINSILVERSWORD:
			swprintf_s(msg, L"得到银剑！攻击力加70");
			break;
		case GAINSMALLFLY:
			swprintf_s(msg, L"您获得了小飞羽，等级提升一级！");
			break;
		case GAINBULLION:
			swprintf_s(msg, L"得到金块，金币数增加300");
			break;
		case GAINGOADARMOR:
			swprintf_s(msg, L"得到黄金盾，防御力增加85！");
			break;
		case GAINHOLYWATER:
			swprintf_s(msg, L"获得圣水！生命值加倍！！");
			break;
		case GAINCOMPASS:
			swprintf_s(msg, L"得到罗盘！按F2键可选择跳转至任意去过的楼层，Esc键退出");
			TextOut(g_mdc, 5 * SQUIREUNITWIDTH, 4.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			goto lop;
			break;
		case GAINCROSS:
			swprintf_s(msg, L"获得幸运十字架，交到0层的仙子处可以增加攻防能力和生命值");
			TextOut(g_mdc, 5 * SQUIREUNITWIDTH, 4.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			goto lop;
			break;
		case GAINSTEELARMOR:
			swprintf_s(msg, L"得到钢盾，防御力增加30！");
			break;
		case GAINSTEELSWORD:
			swprintf_s(msg, L"得到钢剑，攻击力增加30！");
			break;
		case  GAINBIGFLY:
			swprintf_s(msg, L"得到大飞羽！等级提升三级！");
			break;
		case GAINSTARLIGHTSWORD:
			swprintf_s(msg, L"得到星光剑！攻击力提升120点！");
			break;
		case GAINSTARLIGHTARMOR:
			swprintf_s(msg, L"得到星光盾！防御力提升120点！");
			break;
		case GAINHAMMER:
			swprintf_s(msg, L"得到星光神榔！交给小偷可打通18层隐藏地面");
			break;
		case GAINGODSWORD :
			swprintf_s(msg, L"得到星光神剑！攻击力增加150！");
			break;
		case GAINGODARMOR:
			swprintf_s(msg, L"得到光芒神盾！防御力增加190！");
			break;
		default:
			break;
		}
		TextOut(g_mdc, 7 * SQUIREUNITWIDTH, 4.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
		lop:
		DeleteObject(hFont);
	}
	else
	{
		g_gaintime = 0;
		g_gainobject = 0;
		g_gain = FALSE;
	}
}
//战斗结束获得金币经验框绘制
VOID Game_BattleGainPaint()
{
	if (g_battlegaintime<=40)
	{
		SelectObject(g_mdc, g_battlepen);
		SelectObject(g_mdc, g_battlebrush);
		Rectangle(g_mdc, 9.5*SQUIREUNITWIDTH - 0.13*g_battlegaintime*SQUIREUNITWIDTH, 4.5 * SQUIREUNITHEIGHT, 9.5*SQUIREUNITWIDTH + 0.13*g_battlegaintime*SQUIREUNITWIDTH, 5.5 * SQUIREUNITHEIGHT);
	}
	else if (g_battlegaintime > 40 && g_battlegaintime < 80)
	{
		Rectangle(g_mdc, 9.5*SQUIREUNITWIDTH - 5.2*SQUIREUNITWIDTH, 4.5 * SQUIREUNITHEIGHT, 9.5*SQUIREUNITWIDTH + 5.2*SQUIREUNITWIDTH, 5.5 * SQUIREUNITHEIGHT);
		HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
		SelectObject(g_mdc, hFont);
		SetBkMode(g_mdc, TRANSPARENT);
		SetTextColor(g_mdc, RGB(255, 255, 255));
		wchar_t msg[30];
		memset(msg, 0, sizeof(msg));
		swprintf_s(msg, L"您获得了 %d 点经验值，%d 个金币", g_CopyMonster.exp,g_CopyMonster.goad);
		TextOut(g_mdc, 6*SQUIREUNITWIDTH, 4.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
		DeleteObject(hFont);
	}
	else
	{
		g_battlegaintime = 0;
		mciSendString(L"resume back", NULL, 0, NULL);
		g_battlegain = FALSE;
		if (Game_NextArrayPoint().x==5&&Game_NextArrayPoint().y==6&&g_PileNo==19)
		{
			g_talking = TRUE;
		}
		if (Game_NextArrayPoint().x == 5 && Game_NextArrayPoint().y == 1 && g_PileNo == 21)
		{
			g_talking = TRUE;
		}
		if (Game_NextArrayPoint().x == 5 && Game_NextArrayPoint().y == 3 && g_PileNo == 21 && g_blood)
		{
			g_talking = TRUE;
		}
	}
}
//怪物雷达视图绘制
VOID Game_RadarPaint()
{
	SelectObject(g_mdc, g_battlepen);
	SelectObject(g_mdc, g_battlebrush);
	Rectangle(g_mdc, 4 * SQUIREUNITWIDTH+2, 2, WINDOW_WIDTH-18 , WINDOW_HEIGHT );
	int  m = 0,n=0,a=0, list[11] = { 0 };
	for (int i=0;i<11;i++)
	{
		m = i;
		for (int i = 0; i < 11; i++)
		{
			a = i;
			if (g_maze[g_PileNo][m][i] >= 20 && g_maze[g_PileNo][m][i] < 1000)
			{
				for (int i = 0; i < n; i++)
				{
					if (g_maze[g_PileNo][m][a]==list[i])
					{
						goto lp;
					}
				}
				list[n] = g_maze[g_PileNo][m][i];
				n++;
			lp:
				;
			}
		}
	}
	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	SelectObject(g_mdc, hFont);
	SetBkMode(g_mdc, TRANSPARENT);
	SetTextColor(g_mdc, RGB(255, 255, 255));
	wchar_t msg[30];
	for (int i = 0; i < n; i++)
	{
		SelectObject(g_bufdc, g_hMonster[(list[i] - 20) / 4]);
		TransparentBlt(g_mdc,  4*SQUIREUNITWIDTH, i*SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, g_bufdc, Hero.iNum / 10 * SQUIREUNITWIDTH, (list[i] - 20) % 4 * SQUIREUNITHEIGHT, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, RGB(255, 255, 255));
		swprintf_s(msg, L"攻击   %d  防御   %d  金币   %d",g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].damage, g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].defence, g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].goad);
		TextOut(g_mdc, 5.5 * SQUIREUNITWIDTH + 1, i*SQUIREUNITHEIGHT, msg, wcslen(msg));
		int loss = 0;
		int MHP = g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].HP;
		switch (list[i])
		{
		case 34:
			loss += 100;
			break;
		case 35:
			loss += 300;
			break;
		case 48:
			loss += Hero.HP / 4;
			break;
		case 49:
			loss += Hero.HP / 3;
			break;
		default:
			break;
		}
		if (Hero.damage<= g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].defence)
		{
			swprintf_s(msg, L"经验   %d  生命   %d  损失   ???", g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].exp, g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].HP );
		}
		else if (Hero.defence >= g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].damage&&Hero.damage > g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].defence)
		{
			swprintf_s(msg, L"经验   %d  生命   %d  损失   %d", g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].exp, g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].HP,loss);
		}
		else
		{
			while (true)
			{
				MHP -= (Hero.damage - g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].defence);
				if (MHP<=0)
				{
					break;
				}
				loss += (g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].damage - Hero.defence);
			}
			swprintf_s(msg, L"经验   %d  生命   %d  损失   %d", g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].exp, g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].HP,loss);
		}
		TextOut(g_mdc,5.5 * SQUIREUNITWIDTH + 1, (i+0.5)*SQUIREUNITHEIGHT, msg, wcslen(msg));
	}
	DeleteObject(hFont);
}
//罗盘视图绘制
VOID Game_CompassPaint()
{
	HPEN selectpen = CreatePen(PS_SOLID, 5, RGB(255 - abs(g_compasstime % 40 - 20) * 255 / 20, 255 - abs(g_compasstime % 40 - 20) * 255 / 20, 255 - abs(g_compasstime % 40 - 20) * 255 / 20));
	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	if (g_compassswichtime >= 150)
	{
		goto loop;
	}
	SelectObject(g_mdc, g_battlepen);
	SelectObject(g_mdc, g_battlebrush);
	Rectangle(g_mdc, 4 * SQUIREUNITWIDTH + 2, 2, WINDOW_WIDTH - 18, WINDOW_HEIGHT);
	SelectObject(g_mdc, selectpen);
	if (g_compass<=10)
	{
		Rectangle(g_mdc, 4.5*SQUIREUNITWIDTH, (g_compass - 0.5)*SQUIREUNITHEIGHT, 9.5*SQUIREUNITWIDTH, (g_compass + 0.5)*SQUIREUNITHEIGHT);
	}
	else if (g_compass>10)
	{
		Rectangle(g_mdc, 9.5*SQUIREUNITWIDTH, (g_compass - 10.5)*SQUIREUNITHEIGHT, 14.5*SQUIREUNITWIDTH, (g_compass - 9.5)*SQUIREUNITHEIGHT);
	}
	SelectObject(g_mdc, hFont);
	SetBkMode(g_mdc, TRANSPARENT);
	SetTextColor(g_mdc, RGB(255, 255, 255));
	wchar_t msg[30];
	for (int i = 1; i <= g_maxpile; i++)
	{
		memset(msg, 0, sizeof(msg));
		swprintf_s(msg, L"第%d层", i);
		TextOut(g_mdc, (6.5+(i-1)/10*5) * SQUIREUNITWIDTH, 45+(i-1)%10*SQUIREUNITHEIGHT, msg, wcslen(msg));
	}
	loop:
	if (g_seleckeydown==TRUE)
	{
		g_compassswichtime++;
		SelectObject(g_mdc, g_cleanpen);//此处开始引用Game_swich()部分代码（到达层数要变一下）。。。
		if (g_compassswichtime < 150)
		{
			SelectObject(g_swichdc, CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT));
			g_hCleaner = CreateSolidBrush(RGB((150 - g_compassswichtime) * 255 / 150, (150 - g_compassswichtime) * 255 / 150, (150 - g_compassswichtime) * 255 / 150));
			SelectObject(g_swichdc, g_hCleaner);
			Rectangle(g_swichdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			BitBlt(g_mdc, 4 * SQUIREUNITWIDTH, 0, 11 * SQUIREUNITWIDTH, WINDOW_HEIGHT, g_swichdc, 0, 0, SRCAND);
		}
		else if (g_compassswichtime == 150)
		{
			g_hCleaner = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(g_mdc, g_hCleaner);
			Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			g_PileNo = g_compass;
			Hero.Position = g_UpInitPos[g_PileNo];
		}
		else if (g_compassswichtime > 150 && g_compassswichtime < 350)
		{
			g_hCleaner = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(g_mdc, g_hCleaner);
			Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
			SelectObject(g_mdc, hFont);
			SetBkMode(g_mdc, TRANSPARENT);
			SetTextColor(g_mdc, RGB(255, 255, 255));
			wchar_t msg[20];
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"第%d层", g_PileNo);
			TextOut(g_mdc, 9 * SQUIREUNITWIDTH + 10, 5 * SQUIREUNITHEIGHT + 20, msg, wcslen(msg));
			DeleteObject(hFont);
		}
		else if (g_compassswichtime >= 350 && g_compassswichtime < 500)
		{
			g_hCleaner = CreateSolidBrush(RGB((g_compassswichtime - 350) * 255 / 150, (g_compassswichtime - 350) * 255 / 150, (g_compassswichtime - 350) * 255 / 150));
			SelectObject(g_swichdc, g_hCleaner);
			Rectangle(g_swichdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			BitBlt(g_mdc, 4 * SQUIREUNITWIDTH, 0, 11 * SQUIREUNITWIDTH, WINDOW_HEIGHT, g_swichdc, 0, 0, SRCAND);
		}
		else
		{
			g_seleckeydown = FALSE;
			g_compass = 1;
			g_compasstime = 0;
			g_compassswichtime = 0;
			g_compassing = FALSE;
		}
	}
	DeleteObject(hFont);
	DeleteObject(selectpen);
}
//楼层跳转（包括上下楼）过程绘制函数
VOID Game_Swich()
{
	SelectObject(g_mdc, g_cleanpen);
	if (g_swich<150)
	{
		bbmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
		SelectObject(g_swichdc, bbmp);
		g_hCleaner = CreateSolidBrush(RGB((150-g_swich)*255/150, (150 - g_swich) * 255 / 150, (150 - g_swich) * 255 / 150));
		SelectObject(g_swichdc, g_hCleaner);
		Rectangle(g_swichdc, 0, 0,WINDOW_WIDTH ,WINDOW_HEIGHT );
		BitBlt(g_mdc, 4*SQUIREUNITWIDTH, 0, 11*SQUIREUNITWIDTH, WINDOW_HEIGHT, g_swichdc, 0, 0, SRCAND);
		DeleteObject(g_hCleaner);
		DeleteObject(bbmp);
	}
	else if (g_swich==150)
	{
		g_hCleaner = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(g_mdc, g_hCleaner);
		Rectangle(g_mdc, 4 * SQUIREUNITWIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		g_swichup ? g_PileNo++ : g_PileNo-=1;
		g_swichup? Hero.Position = g_UpInitPos[g_PileNo]: Hero.Position = g_DownInitPos[g_PileNo];
		DeleteObject(g_hCleaner);
	}
	else if (g_swich>150 && g_swich<300)
	{
		g_hCleaner = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(g_mdc, g_hCleaner);
		Rectangle(g_mdc, 4*SQUIREUNITWIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
		SelectObject(g_mdc, hFont);
		SetBkMode(g_mdc, TRANSPARENT);
		SetTextColor(g_mdc, RGB(255, 255, 255));
		wchar_t msg[20];
		memset(msg, 0, sizeof(msg));
		swprintf_s(msg, L"第%d层", g_PileNo);
		TextOut(g_mdc, 9*SQUIREUNITWIDTH+10, 5*SQUIREUNITHEIGHT+20, msg, wcslen(msg));
		DeleteObject(hFont);
		DeleteObject(g_hCleaner);
	}
	else if (g_swich>=300 && g_swich<450)
	{
		g_hCleaner = CreateSolidBrush(RGB( (g_swich-300) * 255 / 150, (g_swich - 300) * 255 / 150, (g_swich - 300) * 255 / 150));
		SelectObject(g_swichdc, g_hCleaner);
		Rectangle(g_swichdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		BitBlt(g_mdc, 4 * SQUIREUNITWIDTH, 0, 11 * SQUIREUNITWIDTH, WINDOW_HEIGHT, g_swichdc, 0, 0, SRCAND);
		DeleteObject(g_hCleaner);
	}
	else
	{
		g_swich = 0;
		g_swiching = FALSE;
	}
}
//标题界面的选择绘制函数
VOID Game_StartSelectPaint()
{
	SelectObject(g_mdc, g_battlebrush);
	Rectangle(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_bufdc, g_hStart);
	TransparentBlt(g_mdc, 4.3*SQUIREUNITWIDTH, 1.5*SQUIREUNITHEIGHT, 6 * SQUIREUNITWIDTH, 8 * SQUIREUNITHEIGHT, g_bufdc, 0, 0, 6 * SQUIREUNITWIDTH, 8 * SQUIREUNITHEIGHT, RGB(255, 255, 255));
	SelectObject(g_bufdc, g_hStartSelect);
	StretchBlt(g_mdc, 4.3*SQUIREUNITWIDTH, (5.3+g_startNo*1.35) * SQUIREUNITHEIGHT+(SQUIREUNITHEIGHT-(abs(g_starttime % 40 - 20)*SQUIREUNITHEIGHT / 20))/2, SQUIREUNITWIDTH, abs(g_starttime%40-20)*SQUIREUNITHEIGHT/20, g_bufdc, 0, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT,SRCCOPY);
	StretchBlt(g_mdc, 9.4*SQUIREUNITWIDTH, (5.3+g_startNo*1.35) * SQUIREUNITHEIGHT + (SQUIREUNITHEIGHT - (abs(g_starttime % 40 - 20)*SQUIREUNITHEIGHT / 20)) / 2,SQUIREUNITWIDTH, abs(g_starttime % 40 - 20)*SQUIREUNITHEIGHT / 20, g_bufdc, SQUIREUNITWIDTH, 0, SQUIREUNITWIDTH, SQUIREUNITHEIGHT, SRCCOPY);
	if (g_seleckeydown)
	{
		if (g_startNo==0)
		{
			g_start = FALSE;
			g_starttime = 0;
			g_PileNo = 0;
			Hero = { g_UpInitPos[0],0,0,1000,10,10,0,0,0,0,0,0,0,0,FALSE,FALSE,FALSE,FALSE };
			g_BlackNo = TRUE;
			g_RedNo = TRUE;
			g_maxpile = 0;
			g_thieftalkingNo = 0;
			g_princessNo = 0;
			g_blood = FALSE;
			int maze[30][11][11] = { { { 1,1,1,1,1,1,1,1,2,2,2 },{ 3,3,3,3,3,3,1,1,1,2,2 },{ 3,3,3,3,3,3,3,1,2,2,2 },{ 3,3,3,3,3,3,3,1,1,2,2 },{ 3,3,3,3,3,3,3,1,0,2,2 },{ UP,0,0,0,0,0,0,BRONZEWGATE,1003,0,0 },{ 3,3,3,3,3,3,3,1,0,2,2 } ,{ 3,3,3,3,3,3,3,1,1,2,2 } ,{ 3,3,3,3,3,3,3,1,2,2,2 } ,{ 3,3,3,3,3,3,1,1,1,2,2 },{ 3,3,3,3,3,1,1,1,2,2,2 } },
           {{UP,1,GAINSMALLBOTTLE,-20,1,-20,GAINBLUEJEWEL,1,0,GAINSMALLBOTTLE,GAINSMALLBOTTLE},{0,1,0,28,BRONZEWGATE,29,0,BRONZEWGATE,29,GAINBIGBOTTLE,GAINRADAR},{-20,1,28,GAINREDJEWEL,1,0,-21,1,0,-20,-20},{20,1,BRONZEWGATE,1,1,1,1,1,1,1,1},{21,1,0,0,0,0,0,0,1,-22,0},{20,1,1,1,1,BRONZEWGATE,1,0,GOADGATE,0,DOWN},{0,1,GAINSMALLBOTTLE,GAINSMALLBOTTLE,1,32,1,0,1,0,0},{0,1,-20,-20,1,20,1,0,1,1,1},{0,1,GAINSMALLBOTTLE,GAINSMALLBOTTLE,22,24,1,0,1,-20,-20}, {0,1,1,1,1,1,1,0,BRONZEWGATE,36,-20},{0,0,0,0,0,0,0,0,1,-21,-20}},
           {{DOWN,0,0,0,0,0,0,0,0,0,UP},{1,1,1,1,1,1,4,1,1,1,1},{0,GAINBLUEJEWEL,-20,-20,0,BRONZEWGATE,0,BRONZEWGATE,-20,-20,GAINREDJEWEL},{61,1,1,1,1,1,0,1,1,1,1},{0,GAINBIGBOTTLE,-20,-20,0,1,0,1,GAINBIGBOTTLE,GAINBIGBOTTLE,GAINBIGBOTTLE},{1,1,1,1,0,BRONZEWGATE,0,SILVERGATE,GAINSMALLBOTTLE,GAINSMALLBOTTLE,GAINSMALLBOTTLE},{GAINREDJEWEL,GAINREDJEWEL,GAINREDJEWEL,1,0,1,1,1,1,1,1},{GAINBLUEJEWEL,GAINBLUEJEWEL,GAINBLUEJEWEL,1,BRONZEWGATE,1,0,0,0,0,1000},{-20,-20,-20,1,0,BRONZEWGATE,60,1,1,1,1},{-22,-21,60,BRONZEWGATE,0,1,0,0,0,0,1001},{1,1,1,1,1,1,1,1,1,1,1}} ,
           { {GAINSWORD,21,-20,1,0,20,20,0,1,1,DOWN},{21,-20,28,BRONZEWGATE,0,1,1,0,1,0,0},{-20,0,0,1,0,0,1,0,1,0,1},{1,1,1,1,1,24,1,0,1,0,1},{0,0,1,0,1,21,1,0,24,0,1},{1006,0,BRONZEWGATE,28,1,24,1,1,1,1,1},{0,0,1,0,0,0,0,1,21,GAINBLUEJEWEL,GAINREDJEWEL},{1,1,1,1,1,1,0,BRONZEWGATE,0,24,GAINBIGBOTTLE},{1,0,0,-20,-20,-20,0,1,21,-20,-20},{1,24,1,1,1,1,1,1,1,1,1},{1,0,0,21,24,21,0,0,0,0,UP}},
           {{0,BRONZEWGATE,0,0,24,24,21,0,0,0,UP},{22,1,1,1,1,1,1,1,1,1,1},{0,BRONZEWGATE,0,28,GAINSMALLBOTTLE,GAINSMALLBOTTLE,0,0,0,0,0},{1,1,1,1,1,1,1,1,1,1,22},{0,0,1,25,GAINBLUEJEWEL,1,36,GAINREDJEWEL,1,-20,0},{1002,0,0,26,25,GOADGATE,40,36,SILVERGATE,0,0},{0,0,1,25,GAINBLUEJEWEL,1,36,GAINREDJEWEL,1,-20,0},{1,1,1,1,1,1,1,1,1,1,22},{0,BRONZEWGATE,0,28,GAINSMALLBOTTLE,GAINSMALLBOTTLE ,0,0,0,0,0},{22,1,1,1,1,1,1,1,1,1,1},{0,BRONZEWGATE,0,0,24,24,21,0,0,0,DOWN}},
           {{GAINMUTIPLEKEY,0,25,0,25,GAINREDJEWEL,GAINBLUEJEWEL,0,1,0,DOWN},{1,1,1,BRONZEWGATE,1,1,1,1000,1,0,0},{GAINSMALLBOTTLE,GAINREDJEWEL,0,32,0,0,1,1,1,1,24},{1,1,1,1,1,0,22,22,24,0,0},{GAINBIGBOTTLE,32,29,GAINARMOR,1,0,1,1,1,1,0},{32,0,0,29,1,24,1,0,BRONZEWGATE,24,0},{0,0,1,1,1,28,1,0,1,1,1},{0,0,1,0,0,0,1,0,SILVERGATE,GAINBLUEJEWEL,-20},{32,0,BRONZEWGATE,36,0,0,0,36,1,BRONZEWGATE,1},{-20,32,1,29,0,0,0,40,BRONZEWGATE,0,UP},{-21,-20,1,1001,29,0,0,0,1,1,1}},
           {{GAINSMALLFLY,30,-20,0,1,0,0,0,0,0,0},{30,-20,26,0,1,0,1,1,1,1,0},{1,1,SILVERGATE,1,1,33,1,25,BRONZEWGATE,25,0},{GAINBLUEJEWEL,GAINREDJEWEL,0,40,GOADGATE,0,1,BRONZEWGATE,1,1,1},{1,1,SILVERGATE,1,1,-20,1,25,BRONZEWGATE,0,UP},{-20,0,26,0,1,-20,1,0,1,0,0},{23,-20,0,0,1,-20,1,0,1,1,BRONZEWGATE},{GAINBULLION,23,-20,0,1,0,1,0,1,1,BRONZEWGATE},{1,1,1,1,1,33,1,0,1,0,0},{GAINBIGBOTTLE,44,0,35,BRONZEWGATE,0,1,0,SILVERGATE,0,DOWN},{GAINBIGBOTTLE,GAINBIGBOTTLE,44,0,1,0,1,1,1,1,1}},
           {{UP,1,1,0,0,0,0,0,0,0,0},{0,1,0,0,0,1,1,1,1,0,0},{0,0,26,1,SILVERGATE,1,GAINSMALLBOTTLE,-20,1,1,0},{0,26,GAINBLUEJEWEL,1,48,1,GAINREDJEWEL,GAINSMALLBOTTLE,-21,1,BRONZEWGATE},{0,1,1,1,0,1,1,1,-21,1,DOWN},{0,SILVERGATE,48,0,GAINCROSS,0,48,SILVERGATE,GAINBIGBOTTLE,GOADGATE,0},{0,1,1,1,0,1,1,1,-21,1,0},{0,30,GAINREDJEWEL,1,48,1,GAINBLUEJEWEL,GAINSMALLBOTTLE,-21,1,BRONZEWGATE},{1,0,30,1,SILVERGATE,1,GAINSMALLBOTTLE,-20,1,1,0},{1,1,0,0,0,1,1,1,1,0,0},{1,1,1,0,0,0,0,0,0,0,1}},
           {{DOWN,0,0,0,25,26,25,0,0,0,0},{1,1,1,1,1,1,1,1,1,1,0},{0,0,0,0,0,GAINBLUEJEWEL,22,22,0,1,34},{0,1,1,1,1,1,1,1,30,1,0},{0,1,0,34,GAINSMALLBOTTLE,GAINSMALLBOTTLE,0,1,0,BRONZEWGATE,0},{0,BRONZEWGATE,0,1,1,1,0,1,1,1,1},{1,1,SILVERGATE,1,UP,1,0,40,30,0,0},{0,BRONZEWGATE,0,1,0,1,1,1,1,1,23},{-20,1,0,25,0,1,0,BRONZEWGATE,0,1,48},{30,1,1,1,1,1,26,1,0,1,23},{0,0,GAINREDJEWEL,22,22,0,0,1,0,0,0}},
           {{GAINCOMPASS,-20,1,0,0,1,GAINBLUEJEWEL,1,30,-21,GAINSILVERSWORD},{-20,0,BRONZEWGATE,0,0,SILVERGATE,35,BRONZEWGATE,GAINSMALLBOTTLE,30,-21},{0,37,1,0,0,1,GAINREDJEWEL,1,30,GAINSMALLBOTTLE,30},{1,BRONZEWGATE,1,1,GOADGATE,1,1,1,1,1,BRONZEWGATE},{1,0,0,0,0,0,34,0,1,44,GAINBIGBOTTLE},{1,0,1,1,1,1,1,0,SILVERGATE,34,44},{0,0,1,0,DOWN,1,UP,0,1,44,GAINBIGBOTTLE},{0,1,1,0,1,1,1,BRONZEWGATE,1,1,1},{0},{1,BRONZEWGATE,1,1,1,1,1,1,1,BRONZEWGATE,1},{0,30,-20,-20,GAINSMALLBOTTLE,1,GAINSMALLBOTTLE,-20,-20,30,0}},
           {{0,0,0,0,25,26,25,0,0,0,UP},{1,0,0,1,1,1,1,1,1,1,1},{1,1,0,0,0,0,0,1,GAINSMALLBOTTLE,GAINSMALLBOTTLE,GAINSMALLBOTTLE},{GAINBLUEJEWEL,1,0,1,0,1,0,1,GAINBLUEJEWEL,GAINBLUEJEWEL,GAINBLUEJEWEL},{37,BRONZEWGATE,0,1,-20,1,0,1,GAINREDJEWEL,GAINREDJEWEL,GAINREDJEWEL},{1,1,1,1,-20,1,DOWN,1,1,GOADGATE,1},{37,BRONZEWGATE,0,1,-20,1,1,1,0,34,-21},{GAINREDJEWEL,1,0,1,0,BRONZEWGATE,0,BRONZEWGATE,26,1,1},{1,1,0,0,0,1,BRONZEWGATE,1,0,34,-21},{1,0,35,1,1,1,26,1,1,1,1},{0,35,GAINBIGBOTTLE,1,-20,-20,0,0,-20,-20,GAINSMALLBOTTLE}},
           {{GAINSMALLBOTTLE,GAINSMALLBOTTLE,GAINSMALLBOTTLE,BRONZEWGATE,0,BRONZEWGATE,GAINBLUEJEWEL,GAINBLUEJEWEL,GAINBLUEJEWEL,1,DOWN},{1,1,1,1,0,1,1,1,1,1,0},{-20,-20,-20,BRONZEWGATE,0,1,0,37,37,GOADGATE,0},{1,1,1,1,0,SILVERGATE,41,1,1,1,0},{-21,-21,-21,BRONZEWGATE,0,1,GAINBIGBOTTLE,1,0,GAINSMALLBOTTLE,0},{1,1,1,1,1,1,52,1,1006,0,0},{-22,-22,-22,BRONZEWGATE,0,1,GAINBIGBOTTLE,1,0,GAINSMALLBOTTLE,0},{1,1,1,1,0,SILVERGATE,41,1,1,1,0},{GAINBIGBOTTLE,41,0,1,0,1,0,37,37,GOADGATE,0},{GAINGOADARMOR,42,41,SILVERGATE,0,1,1,1,1,1,0},{GAINBIGBOTTLE,41,0,1,0,BRONZEWGATE,GAINREDJEWEL,GAINREDJEWEL,GAINREDJEWEL,1,UP}},
           {{1001,GAINREDJEWEL,0,0,52,1,0,1,GAINBLUEJEWEL,1,UP},{GAINBLUEJEWEL,0,0,52,56,SILVERGATE,0,1,52,1,0},{1,1,1,1,1,1,0,1,BRONZEWGATE,1,0},{0,0,0,0,0,0,0,0,41,1,0},{60,1,1,1,1,1,1,1,41,1,0},{61,BRONZEWGATE,61,-20,-20,GAINSMALLBOTTLE,GAINSMALLBOTTLE,1,42,SILVERGATE,0},{60,1,1,1,1,1,1,1,41,1,0},{0,0,0,0,0,0,0,0,41,1,0},{1,1,1,1,1,1,0,1,BRONZEWGATE,1,0},{GAINBIGBOTTLE,0,0,64,68,SILVERGATE,0,1,52,1,0},{GAINHAMMER,GAINBIGBOTTLE,0,0,64,1,0,1,GAINREDJEWEL,1,DOWN}} ,
           {{0,0,0,GAINBIGBOTTLE,60,61,60,0,0,1,DOWN},{52,1,1,1,1,1,1,1,60,1,0},{0,1,0,GOADGATE,0,0,56,1,0,0,0},{0,1,0,1,0,31,0,1,1,1,SILVERGATE},{0,1,41,1,56,0,1000,1,0,GAINBIGBOTTLE,0},{0,1,0,1,1,1,1,1,0,1,UP},{0,BRONZEWGATE,0,0,41,42,41,0,0,1,1},{1,1,1,1,1,1,1,1,56,1,GAINBIGFLY},{0,0,0,0,GAINREDJEWEL,GAINREDJEWEL,GAINREDJEWEL,0,0,1,31},{56,1,1,1,1,1,1,1,1,1,BRONZEWGATE},{0,0,0,0,0,0,GAINBLUEJEWEL,GAINBLUEJEWEL,GAINBLUEJEWEL,0,0} } ,
           {{1,1,1,1,1,1,1,1,1,1,1},{0,0,0,0,0,0,0,0,0,41,1},{64,GAINBIGBOTTLE,1,1,1,GAINSMALLBOTTLE,3,3,3,42,1},{GAINMUTIPLEKEY,1,1,1,1,1,3,3,3,41,1},{UP,1,1,1,1,1,1,1,1,SILVERGATE,1},{0,1,1,GAINHOLYWATER,0,56,31,56,SILVERGATE,0,DOWN},{0,1,1,1,1,1,1,1,1,SILVERGATE,1},{0,1,1,1,1,1,3,3,3,41,1},{0,GAINBIGBOTTLE,1,1,1,GAINSMALLBOTTLE,3,3,3,42,1},{0,0,0,0,0,0,0,0,0,41,1},{1,1,1,1,1,1,1,1,1,1,1} },
           {{0},{0,3,3,3,3,3,3,3,3,3,0},{0,3,3,1,1,1,3,3,3,3,0},{0,3,1,1,1,1,1,1,3,3,0},{DOWN,3,1,1000,GAINBLUEJEWEL,GAINREDJEWEL,0,BRONZEWGATE,0,3,0},{3,3,1,1,1,1,1,1,0,GOADGATE,0},{UP,3,1,1001,GAINBLUEJEWEL,GAINREDJEWEL,0,BRONZEWGATE,0,3,0},{0,3,1,1,1,1,1,1,3,3,0},{0,3,3,1,1,1,3,3,3,3,0},{0,3,3,3,3,3,3,3,3,3,0},{0}},
           {{3,3,3,3,3,3,3,3,3,3,3},{3,3,3,3,3,3,3,3,3,3,3},{3,3,3,3,3,3,3,3,3,3,3},{3,3,3,3,1,1,1,1,3,3,3},{3,3,3,1,1,1,1,1,1,3,3},{0,0,0,GOADGATE,0,72,0,UP,1,3,3},{DOWN,3,3,1,1,1,1,1,1,3,3},{3,3,3,3,1,1,1,1,3,3,3},{3,3,3,3,3,3,3,3,3,3,3},{3,3,3,3,3,3,3,3,3,3,3},{3,3,3,3,3,3,3,3,3,3,3}},
           {{3,3,3,3,3,3,3,3,3,3,UP},{64,31,0,0,0,0,0,31,64,3,0},{31,3,3,3,3,3,3,3,31,3,76},{0,3,64,0,0,64,3,3,0,3,0},{0,3,0,3,3,0,3,3,76,3,0},{0,3,0,3,64,0,3,DOWN,0,3,0},{0,3,0,3,0,3,3,3,3,3,0},{0,3,0,3,0,3,64,0,64,3,0},{0,3,0,3,0,3,0,3,0,3,0},{0,3,0,3,64,0,64,3,0,3,0},{64,0,64,3,3,3,3,3,64,0,64}},
           {{3,3,3,3,3,3,3,3,3,3,DOWN},{3,3,3,3,3,3,3,3,3,3,0},{3,3,3,3,3,3,3,3,3,3,0},{3,3,3,3,1,1,1,3,3,3,0},{3,3,3,1,1,1,1,1,3,3,0},{3,3,3,1,1007,0,GOADGATE,GOADGATE,3,3,0},{3,3,3,1,1,1,1,1,3,3,0},{3,3,3,3,1,1,1,3,3,3,0},{3,3,3,3,3,3,3,3,3,3,0},{3,3,3,3,3,3,3,3,3,3,0},{3,3,3,3,3,3,3,3,3,3,0}},
           {{0},{0,3,3,3,3,3,3,3,3,3,0},{0,0,0,0,0,72,0,GAINGODSWORD,3,3,0},{0,3,3,3,3,3,3,3,3,3,0},{0,3,3,3,3,3,3,3,3,3,0},{0,3,3,UP,0,0,27,0,0,0,0},{0,3,3,3,3,3,3,3,3,3,0},{0,3,3,3,3,3,3,3,3,3,0},{0,0,0,0,0,72,0,GAINGODARMOR,3,3,0},{0,3,3,3,3,3,3,3,3,3,0},{0,0,0,0,0,0,0,0,0,0,DOWN}},
           {{76,GAINBIGBOTTLE,3,GAINSMALLBOTTLE,31,-22,31,GAINSMALLBOTTLE,3,GAINBIGBOTTLE,76},{GAINREDJEWEL,3,GAINBLUEJEWEL,3,-21,3,-21,3,GAINBLUEJEWEL,3,GAINREDJEWEL},{48,-20,48,-20,64,0,64,-20,48,-20,48},{GAINSMALLBOTTLE,3,0,3,0,3,0,3,0,3,GAINSMALLBOTTLE},{31,-21,64,0,0,0,0,0,64,-21,31},{-22,3,0,DOWN,0,3,0,UP,0,3,-22},{31,-21,64,0,0,0,0,0,64,-21,31},{GAINSMALLBOTTLE,3,0,3,0,3,0,3,0,3,GAINSMALLBOTTLE},{48,-20,48,-20,64,0,64,-20,48,-20,48},{GAINREDJEWEL,3,GAINBLUEJEWEL,3,-21,3,-21,3,GAINBLUEJEWEL,3,GAINREDJEWEL},{76,GAINBIGBOTTLE,3,GAINSMALLBOTTLE,31,-22,31,GAINSMALLBOTTLE,3,GAINBIGBOTTLE,76}},
           {{3,3,3,3,3,3,3,3,3,3,3},{3,3,0,0,3,3,3,3,3,3,3},{3,0,0,0,0,0,3,3,3,3,3},{3,0,3,0,0,0,0,3,3,3,3},{3,3,3,3,0,0,0,3,3,3,3},{3,27,68,68,0,0,3,DOWN,3,3,3},{3,3,3,3,0,0,0,3,3,3,3},{3,0,3,0,0,0,0,3,3,3,3},{3,0,0,0,0,0,3,3,3,3,3},{3,3,0,0,3,3,3,3,3,3,3},{3,3,3,3,3,3,3,3,3,3,3}} };
			MONSTER monster[MONSTERSPECIES][4] = { { { 20,1,50,1,1 },{ 15,2,70,2,2 },{ 35,10,200,5,5 },{ 250,125,700,30,32 } },
			{ { 20,5,100,3,3 },{ 65,30,150,8,10 },{ 160,90,550,20,25 },{ 1700,1500,30000,220,250 } } ,
			{ { 25,5,110,4,5 },{ 40,20,150,6,8 },{ 90,50,400,12,15 },{ 680,590,2000,65,70 } },
			{ { 50,25,125,7,10 },{ 200,110,100,25,30 },{ 120,70,220,17,20 },{ 400,260,500,45,47 } },
			{ { 75,45,300,10,13 },{ 450,330,900,50,50 },{},{} },
			{ { 150,90,450,19,22 },{ 500,400,1250,55,55 },{ 560,460,1500,60,60 }, {} } ,
			{ { 115,65,500,15,15 },{},{},{} },
			{ { 300,150,1300,35,40 },{},{},{} },
			{ { 620,520,1200,75,65 },{},{},{} },
			{ { 680,590,2000,65,70 },{},{},{} },
			{ { 350,200,850,40,45 },{ 750,650,900,70,77 },{},{} },
			{ { 980,900,1200,75,88 },{},{},{} },
			{ { 830,730,1500,70,80 },{},{},{} },
			{ { 1000,1000,15000,100,100 },{},{},{} },
			{ { 1050,950,3100,80,92 },{},{},{} },
			{{4000,2000,30000,0,0}} };
			int m = 0, n = 0;
			for (int i = 0; i < 30; i++)
			{
				m = i;
				for (int i = 0; i < 11;  i++)
				{
					n = i;
					for (int i = 0; i < 11; i++)
					{
						g_maze[m][n][i] = maze[m][n][i];
					}
				}
			}
			for (int i = 0; i < MONSTERSPECIES; i++)
			{
				m = i;
				for (int i = 0; i < 4; i++)
				{
					g_monster[m][i] = monster[m][i];
				}
			}
			PlaySound(NULL, NULL, SND_PURGE);
			g_gut = TRUE;
			mciSendString(L"open ../Debug/Get_win.wav alias gut", NULL, 0, NULL);
			mciSendString(L"play gut", NULL, 0, NULL);
		}
		else if (g_startNo==1)
		{
			ifstream fin("data.dat", ios::binary);
			if (!fin)
			{
				MessageBox(g_hwnd, L"还木有存档哦！", L"魔塔：拯救公主", 0);
				goto looop;
			}
			fin.read((char*)(&Hero), sizeof(Hero));
			fin.read((char*)(&g_PileNo), sizeof(g_PileNo));
			fin.read((char*)(&g_maze), sizeof(g_maze));
			fin.read((char*)(&g_maxpile), sizeof(g_maxpile));
			fin.read((char*)(&g_RedNo), sizeof(g_RedNo));
			fin.read((char*)(&g_BlackNo), sizeof(g_BlackNo));
			fin.read((char*)(&g_thieftalkingNo), sizeof(g_thieftalkingNo));
			fin.read((char*)(&g_princessNo), sizeof(g_princessNo));
			fin.read((char*)(&g_monster), sizeof(g_monster));
			fin.close();
			if (g_maxpile >=1&&g_maxpile< 8)
			{
				mciSendString(L"open ../Debug/Danger.wav alias back", NULL, 0, NULL);
				mciSendString(L"play back", NULL, 0, NULL);
				g_bgm = TRUE;
				g_bgmtime = 0;
			}
			if (g_maxpile >= 8&&g_maxpile<15)
			{
				mciSendString(L"close back", NULL, 0, NULL);
				mciSendString(L"open ../Debug/Battle_Elder01.wav alias back", NULL, 0, NULL);
				mciSendString(L"play back", NULL, 0, NULL);
				g_bgm = TRUE;
				g_bgmtime = 0;
			}
			if (g_maxpile >= 15 && g_maxpile<19)
			{
				mciSendString(L"close back", NULL, 0, NULL);
				mciSendString(L"open ../Debug/Battle_Elder02.wav alias back", NULL, 0, NULL);
				mciSendString(L"play back", NULL, 0, NULL);
				g_bgm = TRUE;
				g_bgmtime = 0;
			}
			if (g_maxpile >= 19&&!g_blood)
			{
				mciSendString(L"close back", NULL, 0, NULL);
				mciSendString(L"open ../Debug/Last_war.wav alias back", NULL, 0, NULL);
				mciSendString(L"play back", NULL, 0, NULL);
				g_bgm = TRUE;
				g_bgmtime = 0;
			}
			if (g_blood)
			{
				mciSendString(L"close back", NULL, 0, NULL);
				mciSendString(L"open ../Debug/Final_Power.wav alias back", NULL, 0, NULL);
				mciSendString(L"play back", NULL, 0, NULL);
				g_bgm = TRUE;
				g_bgmtime = 0;
			}
			g_start = FALSE;
			PlaySound(NULL, NULL, SND_PURGE);

		looop:
			g_starttime = 0;	
		}
		else if(g_startNo==2)
		{
			sum = MessageBox(NULL, L"真的要退出吗？", L"魔塔：拯救公主", MB_YESNO);
			switch (sum)
			{
			case IDYES:
				DestroyWindow(g_hwnd);
				PostQuitMessage(0);
				break;
			default:
				break;
			}
		}
		g_seleckeydown = FALSE;
	}
}
//剧情绘制函数
VOID Game_GutPaint()
{
	SelectObject(g_mdc, g_battlebrush);
	Rectangle(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_bufdc, g_hGut);
	BitBlt(g_mdc, 2.4*SQUIREUNITWIDTH, WINDOW_HEIGHT - g_guttime, 10 * SQUIREUNITWIDTH, 2 * WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);
	SelectObject(g_bufdc, g_hGutbg);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCAND);
	HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	SelectObject(g_mdc, hFont);
	SetBkMode(g_mdc, TRANSPARENT);
	SetTextColor(g_mdc, RGB(255 - abs(g_guttime % 40 - 20) * 255 / 20, 255 - abs(g_guttime % 40 - 20) * 255 / 20, 255 - abs(g_guttime % 40 - 20) * 255 / 20));
	wchar_t msg[20];
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"…space…");
	TextOut(g_mdc, 7 * SQUIREUNITWIDTH, WINDOW_HEIGHT-30, msg, wcslen(msg));
	DeleteObject(hFont);
	if (g_guttime>3*WINDOW_HEIGHT)
	{
		mciSendString(L"close gut", NULL, 0, NULL);
		g_gut = FALSE;
		g_guttime = 0;
	}
}
//结局绘制函数
VOID Game_EndPaint()
{
	SelectObject(g_mdc, g_battlebrush);
	Rectangle(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_bufdc, g_hEnd);
	BitBlt(g_mdc, 2.4*SQUIREUNITWIDTH, WINDOW_HEIGHT - g_guttime, 10 * SQUIREUNITWIDTH, 2 * WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);
	SelectObject(g_bufdc, g_hGutbg);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCAND);
	HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	SelectObject(g_mdc, hFont);
	SetBkMode(g_mdc, TRANSPARENT);
	SetTextColor(g_mdc, RGB(255 - abs(g_guttime % 40 - 20) * 255 / 20, 255 - abs(g_guttime % 40 - 20) * 255 / 20, 255 - abs(g_guttime % 40 - 20) * 255 / 20));
	wchar_t msg[20];
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"…space…");
	TextOut(g_mdc, 7 * SQUIREUNITWIDTH, WINDOW_HEIGHT - 30, msg, wcslen(msg));
	DeleteObject(hFont);
	if (g_guttime > 3 * WINDOW_HEIGHT)
	{
		g_end = FALSE;
		g_guttime = 0;
		g_start = TRUE;
	}
}
//背景音乐控制函数
VOID Game_BGMJudge()
{
	if (g_battle)
	{
		mciSendString(L"pause back", NULL, 0, NULL);
		g_bgmtime--;
	}
	if (g_maxpile >= 1)
	{
		if (!g_gut)
		{
			mciSendString(L"close gut", NULL, 0, NULL);
		}
		if (g_bgmtime > 5087&&g_maxpile<=7)
		{
			mciSendString(L"close back", NULL, 0, NULL);
			mciSendString(L"open ../Debug/Danger.wav alias back", NULL, 0, NULL);
			mciSendString(L"play back", NULL, 0, NULL);
			g_bgmtime = 0;
		}
 		if (g_bgmtime > 2500 && g_maxpile>7&&g_maxpile<15)
		{
 			mciSendString(L"close back", NULL, 0, NULL);
 			mciSendString(L"open ../Debug/Battle_Elder01.wav alias back", NULL, 0, NULL);
 			mciSendString(L"play back", NULL, 0, NULL);
 			g_bgmtime = 0;
 		}
		if (g_bgmtime > 6300 && g_maxpile >=15 && g_maxpile < 19)
		{
			mciSendString(L"close back", NULL, 0, NULL);
			mciSendString(L"open ../Debug/Battle_Elder02.wav alias back", NULL, 0, NULL);
			mciSendString(L"play back", NULL, 0, NULL);
			g_bgmtime = 0;
		}
		if (g_bgmtime > 9000 && g_maxpile >= 19 && g_maxpile <= 21&&!g_blood)
		{
			mciSendString(L"close back", NULL, 0, NULL);
			mciSendString(L"open ../Debug/Last_war.wav alias back", NULL, 0, NULL);
			mciSendString(L"play back", NULL, 0, NULL);
			g_bgmtime = 0;
		}
	}
	if (g_blood && g_bgmtime > 1490&&!g_end&&!g_start)
	{
		mciSendString(L"close back", NULL, 0, NULL);
		mciSendString(L"open ../Debug/Final_Power.wav alias back", NULL, 0, NULL);
		mciSendString(L"play back", NULL, 0, NULL);
		g_bgmtime = 0;
	}
	if (g_end&&g_bgmtime > 3400)
	{
		mciSendString(L"close back", NULL, 0, NULL);
		mciSendString(L"open ../Debug/Win_road.wav alias back", NULL, 0, NULL);
		mciSendString(L"play back", NULL, 0, NULL);
		g_bgmtime = 0;
	}
}
//运算下一步所在的迷宫数组二维位置
POINT  Game_NextArrayPoint()
{
	POINT n = {0,0};
	switch (Hero.Direction)
	{
	case 0:
		n.x = (Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH;
		n.y = (Hero.Position.y - SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT;
		break;
	case 1:
		n.x = (Hero.Position.x - 4 * SQUIREUNITWIDTH + SQUIREUNITWIDTH) / SQUIREUNITWIDTH;
		n.y = Hero.Position.y / SQUIREUNITHEIGHT;
		break;
	case 2:
		n.x = (Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH;
		n.y = (Hero.Position.y + SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT;
	    break;
	case 3:
		n.x = (Hero.Position.x - 4 * SQUIREUNITWIDTH - SQUIREUNITWIDTH) / SQUIREUNITWIDTH;
		n.y = Hero.Position.y / SQUIREUNITHEIGHT;
	    break;
	default:
		break;
	}
	return n;
}
//程序结束后释放内存
BOOL Game_CleanUp(HWND hwnd)
{
	DeleteObject(g_hCleaner);
	DeleteObject(g_bg);
	DeleteObject(g_DownInitPos);
	DeleteObject(g_hDown);
	DeleteObject(g_hGoadgate);
	DeleteObject(g_hKey);
	DeleteObject(g_hLava);
	DeleteObject(g_hRoad);
	DeleteObject(bmp);
	DeleteObject(g_hStar);
	DeleteObject(g_hUp);
	DeleteObject(g_hWall);
	DeleteObject(g_hyg);
	DeleteObject(g_battlebrush);
	DeleteObject(g_hvs);
	DeleteObject(g_battlepen);
	DeleteObject(g_cleanpen);
	DeleteObject(g_msgbg);
	DeleteObject(g_hStone);
	DeleteObject(g_hSilverGate);
	DeleteObject(g_hSword);
	DeleteObject(g_hBlueJewel);
	DeleteObject(g_hRedJewel);
	DeleteObject(g_hSmallBottle);
	DeleteObject(g_hBigBottle);
	DeleteObject(g_hMutipleKey);
	DeleteObject(g_hArmor);
	DeleteObject(g_hRadar);
	DeleteObject(g_hSilverSword);
	DeleteObject(g_hSmallFly);
	DeleteObject(g_hBullion);
	DeleteObject(g_hGoadArmor);
	DeleteObject(g_hHolyWater);
	DeleteObject(g_hCross);
	DeleteObject(g_hBigFly);
	DeleteObject(g_hHammer);
	DeleteObject(g_hGodSword);
	DeleteObject(g_hGodArmor);
	DeleteObject(g_hStart);
	DeleteObject(g_hGut);
	DeleteObject(g_hGutbg);
	DeleteDC(g_swichdc);
	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	for (int i = 0; i < 4; i++)
	{
		DeleteObject(g_hSprite[i]);
	}
	for (int i = 0; i < MONSTERSPECIES; i++)
	{
		DeleteObject(g_hMonster[i]);
	}
	for (int i = 0; i < NPCSPECIES; i++)
	{
		DeleteObject(g_hNPC[i]);
	}
	return TRUE;
}
