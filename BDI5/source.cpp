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
//Ԥ�Ƚ��к궨�壬����Ӳ���룬��������޸�
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 660
#define SQUIREUNITWIDTH 80
#define SQUIREUNITHEIGHT 60
#define  WINDOW_TITLE L"ħ�������ȹ���"
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
HBITMAP		g_hSprite[4] = { NULL }, bmp, g_bg, g_msgbg, g_hWall, g_hLava, g_hyg, g_hStar, g_hRoad, g_hUp, g_hDown, g_hKey, g_hGoadgate, g_hMonster[MONSTERSPECIES], g_hvs, g_hNPC[NPCSPECIES];//���ﱣ����Ϸ����ĸ���λͼ���
HBITMAP     g_hStone, g_hSilverGate,g_hSword,g_hBlueJewel,g_hRedJewel,g_hSmallBottle,g_hBigBottle,g_hMutipleKey,g_hArmor,g_hRadar,g_hSilverSword,g_hSmallFly,g_hBullion,g_hGoadArmor,g_hHolyWater,g_hCompass,g_hCross,g_hBigFly,g_hHammer,g_hGodSword,g_hGodArmor,g_hStart,g_hStartSelect,g_hGut,g_hGutbg,g_hEnd,bbmp;//���ﱣ����Ϸ����ĸ���λͼ���
DWORD g_tPre = 0, g_tNow = 0;//�����ж�ʱ�䣬ÿ���һ��ʱ��ִ��һ��Game_paint()��������ʵ�ֶ���Ч��
int g_PileNo = 0, g_maxpile = 0;//���ص�ǰ���ڲ������͵�������߲���
BOOL g_swiching = FALSE, g_swichup = TRUE, g_battle = FALSE, g_battlegain = FALSE, g_talking = FALSE, g_selecting = FALSE, g_gain = FALSE,g_seleckeydown = FALSE,g_radaring=FALSE,g_compassing=FALSE,g_RedNo=TRUE,g_BlackNo = TRUE,g_start=TRUE,g_gut=FALSE,g_end=FALSE,g_blood=FALSE,g_bgm=TRUE;//�����������Ĵ���״̬��¥���ת��״̬���������ֵĲ���״̬
int g_swich = 0, g_battletime = 0, g_battleobject = 0, g_battlegaintime = 0, g_talkingNo = 0, g_talkingtime = 0, g_gaintime = 0, g_gainobject = 0, g_selecttime = 0, g_seletobjectsNo = 0, g_selectofnow = 0, g_thieftalkingNo = 0, g_princessNo = 0, g_compass = 1, g_compasstime = 0, g_compassswichtime = 0, sum = 0, g_starttime = 0, g_startNo = 0, g_guttime = 0,g_bgmtime=0;//������飬ת�����Ѿ�����ʱ�䵥λ�����Լ�������ؾ���Ľ׶Σ���ǰս�������ѡ�����
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
{{3,3,3,3,3,3,3,3,3,3,3},{3,3,0,0,3,3,3,3,3,3,3},{3,0,0,0,0,0,3,3,3,3,3},{3,0,3,0,0,0,0,3,3,3,3},{3,3,3,3,0,0,0,3,3,3,3},{3,27,68,68,0,0,3,DOWN,3,3,3},{3,3,3,3,0,0,0,3,3,3,3},{3,0,3,0,0,0,0,3,3,3,3},{3,0,0,0,0,0,3,3,3,3,3},{3,3,0,0,3,3,3,3,3,3,3},{3,3,3,3,3,3,3,3,3,3,3}} };//���������Թ���Ϣ
POINT g_UpInitPos[30] = { {9*SQUIREUNITWIDTH,9*SQUIREUNITHEIGHT},{9 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT}, {4 * SQUIREUNITWIDTH,SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{14 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT}, {4 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{13 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT} ,{9 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{4 * SQUIREUNITWIDTH,SQUIREUNITHEIGHT},{10 * SQUIREUNITWIDTH,3 * SQUIREUNITHEIGHT},{8 * SQUIREUNITWIDTH,6 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{13 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT} ,
{9 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{7 * SQUIREUNITWIDTH,0},{9 * SQUIREUNITWIDTH,0},{9 * SQUIREUNITWIDTH,8 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{13 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{9 * SQUIREUNITWIDTH,4 * SQUIREUNITHEIGHT},{9*SQUIREUNITWIDTH,5*SQUIREUNITHEIGHT} };//��¼��¥�Լ�¥����ת֮��ĳ�ʼλ��
POINT g_DownInitPos[29] = { {9 * SQUIREUNITWIDTH,SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,0}, {4 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{14 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{4 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{13 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{9 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,0},{11 * SQUIREUNITWIDTH,4 * SQUIREUNITHEIGHT},{10 * SQUIREUNITWIDTH,7 * SQUIREUNITHEIGHT},{4 * SQUIREUNITWIDTH,9 * SQUIREUNITHEIGHT},{13 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{8*SQUIREUNITWIDTH,10*SQUIREUNITHEIGHT} ,
{9 * SQUIREUNITWIDTH,0},{11 * SQUIREUNITWIDTH,0},{9 * SQUIREUNITWIDTH,6 * SQUIREUNITHEIGHT},{5 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT} ,{13 * SQUIREUNITWIDTH,10 * SQUIREUNITHEIGHT},{9 * SQUIREUNITWIDTH,4 * SQUIREUNITHEIGHT},{0,0}/*(NULL)*/,{0,0}/*(NULL)*/ };//��¼��¥֮������ĳ�ʼλ��
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
};//����Ӣ�۵ĸ�������
struct MONSTER
{
	int damage;
	int defence;
	int HP;
	int exp;
	int goad;
};//��¼�����������
MONSTER g_monster[MONSTERSPECIES][4] = { {{20,1,50,1,1},{15,2,70,2,2},{35,10,200,5,5},{250,125,700,30,32}},//��ϵ
{{20,5,100,3,3},{65,30,150,8,10},{160,90,550,20,25},{1700,1500,30000,220,250}} ,//����ϵ
{{25,5,110,4,5},{40,20,150,6,8},{90,50,400,12,15},{680,590,2000,65,70}},//����ϵ
{{50,25,125,7,10},{200,110,100,25,30},{120,70,220,17,20},{400,260,500,45,47}},//��ʦϵ
{{75,45,300,10,13},{450,330,900,50,50},{},{}},//����ϵ
{{150,90,450,19,22},{500,400,1250,45,55},{560,460,1500,60,60},{}} ,//����ϵ
{{115,65,500,15,15},{},{},{}},//ʯͷϵ
{{300,150,1300,35,40},{},{},{}},//����ʿϵ
{{620,520,1200,75,65},{},{},{}},//˫�ֽ�ʿϵ
{{680,590,2000,65,70},{},{},{} },//ڤսʿϵ
{{350,200,850,40,45},{750,650,900,70,77},{},{} },//�����ϵ
{{980,900,1200,75,88},{},{},{} },//����ʿϵ
{{830,730,1500,70,80},{},{},{}},//�鷨ʦϵ
{{1000,1000,15000,100,100},{},{},{} },//����ħ��
{{1050,950,3100,80,92},{},{},{}},//Ӱ��սʿϵ
{{4000,2000,40000,0,0}}/*ѪӰ*/ };//��¼��������
MONSTER g_CopyMonster = g_monster[g_battleobject / 4][g_battleobject % 4];//��ǰս�����������ж��Ƿ񴥷����飬�Լ�ս����Ҫ
HERO Hero;
HBRUSH g_hCleaner,g_battlebrush;//������ˢ��ս����ˢ
HPEN g_battlepen,g_cleanpen;//ս���߿򣬿յ������߿�
HWND g_hwnd;//���ھ��
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
//�������ں��������𴰿ڵ���ƣ���������ʾ�ƶ����£�ע��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//����������
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
	//�������ע��
	if (!RegisterClassEx(&wndClass))
		return -1;
	//���ڵ���ʽ����
	HWND hWnd = CreateWindow(L"ForTheDream", WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	g_hwnd = hWnd;
	//���ڵ��ƶ�����ʾ������
	MoveWindow(hWnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT+40, true);
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);
	//���з�װ�õ���Ϸ��ʼ�����������Լ�������λͼ����ʼ��ʧ�ܣ��򵯳���Ϣ�Ի���
	if (!Game_Init(hWnd))
	{
		MessageBox(hWnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0);
		return FALSE;
	}
	//��Ϸ��Ϣѭ����ϵ�����Ӹ�Ч�Ļ�ͼ
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
//���ڹ��̺�������Ӧ������Ϣ
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
			case VK_ESCAPE://д��������ļ�������
				fout.write((char*)(&Hero), sizeof(Hero));
				fout.write((char*)(&g_PileNo), sizeof(g_PileNo));
				fout.write((char*)(&g_maze), sizeof(g_maze));
				fout.write((char*)(&g_maxpile), sizeof(g_maxpile));
				fout.write((char*)(&g_RedNo), sizeof(g_RedNo));
				fout.write((char*)(&g_BlackNo), sizeof(g_BlackNo));
				fout.write((char*)(&g_thieftalkingNo), sizeof(g_thieftalkingNo));
				fout.write((char*)(&g_princessNo), sizeof(g_princessNo));
				fout.write((char*)(&g_monster), sizeof(g_monster));
				sum = MessageBox(NULL, L"���Զ��浵��ȷ��Ҫ���ر������ô��", L"ħ�������ȹ���", MB_YESNO);
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
				if ((Hero.Position.y - SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT >= 0)//����߽�֮������ǰ�����ܴ����жϣ���
				{

					if (g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y - SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT] != 0 && g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y - SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT] > (-20))//��ʱ�����޷��߶��������жϺ��������ж���һ���Ƿ񴥷������Լ�����仯
					{
						Game_Judge(g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y - SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT]);
						break;
					}
					Game_gain(g_maze[g_PileNo][(Hero.Position.x - 4 * SQUIREUNITWIDTH) / SQUIREUNITWIDTH][(Hero.Position.y - SQUIREUNITHEIGHT) / SQUIREUNITHEIGHT]);//���л�õ��ߺ���
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
		if (g_talking)//�������
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
		else if (g_selecting)//ѡ�����
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
		else if (g_radaring)//�����״��˳�����
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
		else if (g_compassing)//����¥����תѡ�����
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
		else if (g_start)//�������ѡ�����
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
		else if (g_gut)//��ʼ�������
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
//��Ҫ���ظ���λͼ���Լ���ʼ�����ʣ���ˢ
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
//������Ϸ����Ļ���
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
//���λ�ڻ������ĸ�����Ϣ
VOID Game_Output()
{
	HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	SelectObject(g_mdc, hFont);
	SetBkMode(g_mdc, TRANSPARENT);
	SetTextColor(g_mdc, RGB(255, 255, 255));
	wchar_t msg[20];
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"��%d��", g_PileNo);
	TextOut(g_mdc, 50, 30, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"Lv%d", Hero.level);
	TextOut(g_mdc, 190, 30, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"����ֵ %d", Hero.HP);
	TextOut(g_mdc, 50, 100, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"������ %d", Hero.damage);
	TextOut(g_mdc, 50, 140, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"������ %d", Hero.defence);
	TextOut(g_mdc, 50, 180, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"����ֵ %d", Hero.exp);
	TextOut(g_mdc, 50, 220, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"��� %d", Hero.goad);
	TextOut(g_mdc, 50, 260, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"         %d��", Hero.bronzekey);
	TextOut(g_mdc, 50, 323, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"         %d��", Hero.silverkey);
	TextOut(g_mdc, 50, 383, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"         %d��", Hero.goadkey);
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
//Ԥ��ս����Ӣ���Ƿ�����
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
//����ս���������
VOID Game_BattlePaint()
{
	SelectObject(g_mdc,g_battlepen);
	SelectObject(g_mdc, g_battlebrush);
	Rectangle(g_mdc, 4.5*SQUIREUNITWIDTH, 0.5*SQUIREUNITHEIGHT, WINDOW_WIDTH - 0.5*SQUIREUNITWIDTH, 7.5*SQUIREUNITHEIGHT);
	SelectObject(g_bufdc, g_msgbg);//��ս������
	StretchBlt(g_mdc, 4.5*SQUIREUNITWIDTH, 0.5*SQUIREUNITHEIGHT, 10 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, g_bufdc, 0, 0, 4*SQUIREUNITWIDTH, WINDOW_HEIGHT, SRCCOPY);
	SelectObject(g_bufdc, g_hSprite[1]);//��Ӣ��
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
	swprintf_s(msg, L"����ֵ %d", Hero.HP);
	TextOut(g_mdc, 4.75*SQUIREUNITWIDTH, 3.25*SQUIREUNITHEIGHT+60, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"������ %d", Hero.damage);
	TextOut(g_mdc, 4.75*SQUIREUNITWIDTH, 3.25*SQUIREUNITHEIGHT+120, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"������ %d", Hero.defence);
	TextOut(g_mdc, 4.75*SQUIREUNITWIDTH, 3.25*SQUIREUNITHEIGHT+180, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"����ֵ %d", g_CopyMonster.HP);
	TextOut(g_mdc, WINDOW_WIDTH - 2.75*SQUIREUNITWIDTH, 3.25*SQUIREUNITHEIGHT + 60, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"������ %d", g_CopyMonster.damage);
	TextOut(g_mdc, WINDOW_WIDTH - 2.75*SQUIREUNITWIDTH, 3.25*SQUIREUNITHEIGHT + 120, msg, wcslen(msg));
	memset(msg, 0, sizeof(msg));
	swprintf_s(msg, L"������ %d", g_CopyMonster.defence);
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
//�ж���һ���Ķ���
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
//������NPC�Ի����Լ�����
VOID Game_TalkingPaint()
{
	SelectObject(g_mdc, g_battlepen);
	SelectObject(g_mdc, g_battlebrush);
	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	HFONT hFont2 = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, NULL);
	HFONT hFont3 = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("΢���ź�"));
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�����ˣ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����˭���������");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������������ӣ��ղ��㱻��");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���С�ִ���ˡ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���������ҵĽ��أ�");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��Ľ������������ˣ���ֻ��");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ü�����ȳ�����");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ǣ������أ��������ȹ�����");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�����������棬��������ȥ��");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�򲻹������С�ֵġ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������ô�죬�Ҵ�Ӧ�˹���һ");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��Ҫ�ѹ����ȳ����ģ�������");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��Ӧ����ô���أ�");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���İɣ��Ұ��ҵ����������");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����Ϳ��Դ�Ӯ��ЩС���ˡ�");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����������Ȱ���ȥ��һ����");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�����ҵ��������������ҡ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�Ҷ�������ʲô������");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��һ��ʮ�ּܣ��м���һ�ź�");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ɫ�ı�ʯ��");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�Ǹ�������ʲô��ô��");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ұ������������ػ��ߣ��ɲ�");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��ǰ���ӱ�������һ����ħ��");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����ռ�����������������ҵ�");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ħ�����������ʮ�ּ����棬");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������ܽ�����������������");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��ħ����������ػָ�������");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ʱ�ұ���԰����������ȥ��");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�������ˡ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ðɣ������Կ���");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ղ���ȥ�����ˣ���Ľ�����");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����¥����Ķ�����¥�ϣ���");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�Ǹ�ʮ�ּܱ�������¥��Ҫ��");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��¥�������ȡ����Ľ��Ͷܡ�");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���⣬�����������¥���ϣ�");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����һЩ����˺ü�����ı�");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���ͱ������õ����ǣ���");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����Ը�������Ĺ��ｫ�к�");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�������");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���ǣ�����ô��ȥ�أ�");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������������Կ�ף�������ȥ");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���������滹�кܶ�������Կ");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ף���һ��Ҫ��ϡʹ�á�");
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
				swprintf_s(msg, L"���ӣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�¸ҵ�ȥ�ɣ���ʿ��");
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
					swprintf_s(msg, L"��ʿ��");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"���ӣ����Ѿ����Ǹ�ʮ�ּ���");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"���ˡ�");
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
					swprintf_s(msg, L"���ӣ�");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�����ĺܺá�");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"��ô�������ҾͿ�ʼ�������");
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
					swprintf_s(msg, L"���ӣ�");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"ǿ��������");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�������仩����");
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
					swprintf_s(msg, L"���ӣ�");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"���ˣ����Ѿ��������ڵ�����");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�����ˣ�");
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
					swprintf_s(msg, L"���ӣ�");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"��ס�������û���㹻��ʵ��");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�Ļ�����Ҫȥ�ڶ�ʮһ�㣡��");
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
					swprintf_s(msg, L"���ӣ�");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"��һ��������б���ķ���");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"����ʧȥ���ã�");
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
				swprintf_s(msg, L"�� �� �� ��");
				TextOut(g_mdc, 8 * SQUIREUNITWIDTH, 2.9*SQUIREUNITHEIGHT, msg, wcslen(msg));
				SelectObject(g_mdc, hFont);
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ˣ���ã�Ӣ�۵���ʿ�������ǹ���");
				TextOut(g_mdc, 7.0 * SQUIREUNITWIDTH - 1, 4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�̵꣬���������һЩ����������");
				TextOut(g_mdc, 6.6 * SQUIREUNITWIDTH, 4.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ʹ�÷�������͡������ڲ˵��н���");
				TextOut(g_mdc, 7.0 * SQUIREUNITWIDTH - 1, 5.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ѡ��ʹ�ÿո����������ȷ�����ѡ");
				TextOut(g_mdc, 6.6 * SQUIREUNITWIDTH, 5.8 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��");
				TextOut(g_mdc, 6.6 * SQUIREUNITWIDTH, 6.4 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ͬʱ�������˻��������˴����н���");
				TextOut(g_mdc, 7.0 * SQUIREUNITWIDTH - 1, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"Ҳ��һ���Ĳ���������");
				TextOut(g_mdc, 6.6 * SQUIREUNITWIDTH, 7.6 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"֪��������ʿ��");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���Ѿ��þ��ˣ�");
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
				swprintf_s(msg, L"�������ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"Ŷ���ҵĺ��ӣ�����̫��л��");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ˣ�");
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
				swprintf_s(msg, L"�������ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����ط������ֻ����������");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�������ȥ�ˡ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���߰ɣ��һ���ȥ�ȱ�������");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��Ĺ�����");
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
				swprintf_s(msg, L"�������ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"Ŷ���������ȹ����ģ�Ϊ�˱�");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ʾ����ĸ�л�������������");
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
				swprintf_s(msg, L"�������ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����ɣ��⻹���������ʱ��");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ù��ġ�");
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
				swprintf_s(msg, L"�������ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������ȥ��ȹ����ɣ�");
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
			break;//�Ի�����break������ֹ����ѡ��ҳ��
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
				swprintf_s(msg, L"�������ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��ã��¸ҵĺ��ӣ���������");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�������ˡ�");
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
				swprintf_s(msg, L"�������ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ҽ�����һ���ǳ��õı��");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������ʹ��Ĺ���������120");
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
				swprintf_s(msg, L"�������ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�㣬�������������500�㾭");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�������н���������һ���Ӱɣ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ðɣ��Ǿͽ��ǰѽ����Ұɣ�");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				//memset(msg, 0, sizeof(msg));
				//swprintf_s(msg, L"��Ĺ�����");
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
				swprintf_s(msg, L"�������ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�Ǻðɣ���ѽ��͸����ˣ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���Ѿ��þ��ˣ�");
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
				swprintf_s(msg, L"���ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"Ŷ���������̫��л���ˣ�");
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
				swprintf_s(msg, L"���ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���Ǹ����ˣ���֪Ϊʲô��ץ");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���������ˡ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���߰ɣ��������Ѿ������ˡ�");
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
				swprintf_s(msg, L"���ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"Ŷ���ԶԶԣ����Ѿ������ˡ�");
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
				swprintf_s(msg, L"���ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����������͸���ɣ�������");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��׼����Ǯ�ġ�");
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
				swprintf_s(msg, L"���ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����������һ���кܴ������");
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
			break;//�Ի�����break��������ֹ����ѡ��ҳ��
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
				swprintf_s(msg, L"���ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��������ӭ��ĵ�����������");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��һ��������˵�ǳ��õı��");
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
				swprintf_s(msg, L"���ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ֻҪ�������Ǯ���Ҿ�������");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ʲô���Ҫ����Ǯ��");
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
				swprintf_s(msg, L"���ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�������Ϸ��õĶ��ƣ�����");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ֵ��������120�㣬����ֻҪ");
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
				swprintf_s(msg, L"���ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��500����ҾͿ������¡�");
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
				swprintf_s(msg, L"���ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��ô��������500�������");
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
					swprintf_s(msg, L"��ʿ��");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"����500����ҡ�");
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
				swprintf_s(msg, L"���ˣ�");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ã��ɽ���");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���Ѿ��þ��ˣ�");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����������̫���ˣ����ֿ���");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��������Ѱ���ˣ�");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"Ŷ����û�����ҽ��ܣ��ҽн�");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ˣ����⸽��������С͵��ʲ");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ô�����Ʊ���������͵����");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������������ɲ���̫�ã���");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�����ͱ�ץ�ˡ�");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��������Ҵ����ţ����Ҿ�");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������һ���°ɡ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���߰ɣ����滹�кܶ���");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ҿ��ܹ˲����㡣");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���������������µġ���˵��");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��������ʲô��");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��Ὺ��ô��");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ǵ�Ȼ��");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�Ǿ�������Ҵ򿪵ڶ����");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�Űɣ�");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�Ǹ��򵥣�������������ܰ�");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���ҵ�һ��Ƕ�˺챦ʯ������");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ͷ�Ļ����һ������ͨ��ʮ��");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���·��");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"Ƕ�˺챦ʯ������ͷ���ðɣ�");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�Ұ������ҡ�");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ǳ��ĸ�л��һ���ұ�Ὣ��");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������Ŵ򿪡�");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������ҵ��Ǹ�����ͷ�Ļ���");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�������������ң�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�򣬿쿴�����ҵ���ʲô��");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"̫���ˣ����������Ȼ������");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�");
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
				swprintf_s(msg, L"С͵��");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ðɣ������ȥ�����޺õ�ʮ");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�˲��·�档");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��������þ��ˣ�");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�������������ҵ���");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ǵģ����Ƿ��������������");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��ġ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��������ҳ�ȥ�ɣ�");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�����һ������ߡ�");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"Ϊʲô�������浽�����Ƕ�ħ");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������Ϊ�����浽�����Ƕ�ħ");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�����ԲŲ����Ծ�������ȥ��");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��Ҫ�����Ǹ���ħ��ɱ����");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"Ӣ�۵���ʿ��������ܹ�����");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�����ħɱ�����Ҿͺ���һ��");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��ȥ��");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���ħ�����Ѿ�ɱ����һ��ħ");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���ħ������������ˣ���");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ɱ���Ŀ�����һ��С�ӳ�֮��");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�Ķ�ħ��");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�ã�������ţ�����ɱ���Ǹ�");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��ħ�����������㣡");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"���ħ����ղ�ɱ�����Ǹ���");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�����ˡ�");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�������������������ħ");
				TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�����Ĺ������ͷ�����������");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"������һ�����ϣ���ҪС�ģ�");
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
				swprintf_s(msg, L"������");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��һ��Ҫɱ����ħ����");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"����");
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
				swprintf_s(msg, L"����ħ����");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"ֹͣ�ɣ��޴������࣡");
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
				swprintf_s(msg, L"��ʿ��");
				TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"��ֹͣ�����㣡ħ������˵��");
				TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�����������");
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
				swprintf_s(msg, L"����ħ����");
				TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
				memset(msg, 0, sizeof(msg));
				swprintf_s(msg, L"�����Ӯ����˵�ɣ�");
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
					swprintf_s(msg, L"��ʿ��");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"��ħͷ��������ڵ��ˣ�");
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
					swprintf_s(msg, L"ڤ��ħ����");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"����������");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"��Ҳ��������˼������Ϊ����");
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
					swprintf_s(msg, L"ڤ��ħ����");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�Ǽһ������������Ϳ��Դ�");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"���ң��������㻹�����أ�");
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
					swprintf_s(msg, L"��ʿ��");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�ϻ���˵��ȥ���ɣ�");
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
					swprintf_s(msg, L"ڤ��ħ����");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�������㻹�������ӣ��б���");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�Ļ���21¥��");
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
					swprintf_s(msg, L"ڤ��ħ����");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�������Ϳ��Լ�ʶ������");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"����ʵ���ˣ�");
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
					swprintf_s(msg, L"ڤ��ħ����");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"������");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"��ô���ܣ�����ô���ܻᱻ��");
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
					swprintf_s(msg, L"ڤ��ħ����");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"��ܣ�");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"����������ǿ��ħ��������");
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
					swprintf_s(msg, L"ڤ��ħ����");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"����������������");
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
					swprintf_s(msg, L"��ŭ��ħ��չ���˾�������");
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
					swprintf_s(msg, L"��ʿ��");
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
					swprintf_s(msg, L"��ʿ��");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"ԭ��������Ǹ�Ϊ���ķ���Ѫ");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"Ӱ��");
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
					swprintf_s(msg, L"ѪӰ��");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"���������ã������Ҿ���Ѫ");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"Ӱ���ܰ��ұƵ�����ԭ�εķ�");
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
					swprintf_s(msg, L"ѪӰ��");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�ϣ��㻹��ͷһ������������");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�������һ���ˡ�");
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
					swprintf_s(msg, L"��ʿ��");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"��Ҳ�������һ������ԭ����");
					TextOut(g_mdc, 9.5 * SQUIREUNITWIDTH, 7.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�����Ҿ������е���ն������");
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
					swprintf_s(msg, L"��ʿ��");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"��ħͷ���Ծ��󻼣�");
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
					swprintf_s(msg, L"ѪӰ��");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"�ȿȡ�����һ�����񽣣���");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"���ݺ�һ����ȴ����������");
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
					swprintf_s(msg, L"ѪӰ��");
					TextOut(g_mdc, 5.5*SQUIREUNITWIDTH, 3.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"С������,�����ٴ�����һ��");
					TextOut(g_mdc, 6 * SQUIREUNITWIDTH, 4.45*SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"���û���ӵ�е�һ�У���");
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
					swprintf_s(msg, L"��ʿ��");
					TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
					memset(msg, 0, sizeof(msg));
					swprintf_s(msg, L"��Ȳ�����һ���ˣ������ɣ�");
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
//����ѡ����صĲ���
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
		swprintf_s(msg, L"��Ҫ�������������");
		TextOut(g_mdc, 8.1 * SQUIREUNITWIDTH, 2.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		if (g_PileNo == 3)
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"�������25����ң������ѡ��");
			TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		}
		else
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"�������100����ң������ѡ��");
			TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		}
		memset(msg, 0, sizeof(msg));
		swprintf_s(msg, L"��һ�");
		TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		SelectObject(g_mdc, selectpen);
		Rectangle(g_mdc, 6.75*SQUIREUNITWIDTH, 4.8*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT, 12.85*SQUIREUNITWIDTH, 5.55*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT);
		if (g_PileNo == 3)
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"���� 800 ������");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"���� 4 �㹥��");
			TextOut(g_mdc, 8.5 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"���� 4 �����");
			TextOut(g_mdc, 8.5 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"�뿪�̵�");
			TextOut(g_mdc, 9 * SQUIREUNITWIDTH, 7.25*SQUIREUNITHEIGHT, msg, wcslen(msg));
		}
		else
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"���� 4000 ������");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"���� 20 �㹥��");
			TextOut(g_mdc, 8.5 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"���� 20 �����");
			TextOut(g_mdc, 8.5 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"�뿪�̵�");
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
		swprintf_s(msg, L"��ã�Ӣ�۵����ֻ࣬Ҫ����");
		TextOut(g_mdc, 8.1 * SQUIREUNITWIDTH, 2.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		memset(msg, 0, sizeof(msg));
		swprintf_s(msg, L"�㹻�ľ��飬�ҾͿ���������");
		TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		memset(msg, 0, sizeof(msg));
		swprintf_s(msg, L"��ǿ��");
		TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
		SelectObject(g_mdc, selectpen);
		Rectangle(g_mdc, 6.75*SQUIREUNITWIDTH, 4.8*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT, 12.85*SQUIREUNITWIDTH, 5.55*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT);
		if (g_PileNo == 5)
		{
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"����һ������Ҫ100�㣩");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"���ӹ���5����Ҫ30�㣩");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"���ӷ���5����Ҫ30�㣩");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"�뿪�̵�");
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
			swprintf_s(msg, L"������������Ҫ270�㣩");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"���ӹ���17����Ҫ95�㣩");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"���ӷ���17����Ҫ95�㣩");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"�뿪�̵�");
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
			swprintf_s(msg, L"������һ�����������Ҫ��ֻ");
			TextOut(g_mdc, 8.1 * SQUIREUNITWIDTH, 2.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"���н�ң��ҾͿ��԰��㣺");
			TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			SelectObject(g_mdc, selectpen);
			Rectangle(g_mdc, 6.75*SQUIREUNITWIDTH, 4.8*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT, 12.85*SQUIREUNITWIDTH, 5.55*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT);
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"����һ��ͭԿ�ף�$ 10��");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"����һ����Կ�ף�$ 50��");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"����һ�ѽ�Կ�ף�$100��");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"�뿪�̵�");
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
			swprintf_s(msg, L"Ŷ����ӭ��ĵ������������");
			TextOut(g_mdc, 8.1 * SQUIREUNITWIDTH, 2.7 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"��ȱ�ٽ�ң��ҿ��԰��㣺");
			TextOut(g_mdc, 7.7 * SQUIREUNITWIDTH + 1, 3.2 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			SelectObject(g_mdc, selectpen);
			Rectangle(g_mdc, 6.75*SQUIREUNITWIDTH, 4.8*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT, 12.85*SQUIREUNITWIDTH, 5.55*SQUIREUNITHEIGHT + 0.75*g_selectofnow*SQUIREUNITHEIGHT);
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"����һ��ͭԿ�ף�$ 7��");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5 * SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"����һ����Կ�ף�$ 35��");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 5.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"����һ�ѽ�Կ�ף�$ 70��");
			TextOut(g_mdc, 8.25 * SQUIREUNITWIDTH, 6.5*SQUIREUNITHEIGHT, msg, wcslen(msg));
			memset(msg, 0, sizeof(msg));
			swprintf_s(msg, L"�뿪�̵�");
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
//��ȡ����֮���޸���������
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
//��õ��ߵ���ʾ�����
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
			swprintf_s(msg, L"�������һ��ͭԿ��");
			break;
		case -21:
			swprintf_s(msg, L"�������һ����Կ��");
			break;
		case -22:
			swprintf_s(msg, L"�������һ�ѽ�Կ��");
			break;
		case GAINBLUEJEWEL:
			swprintf_s(msg, L"�������һ������ʯ������������3");
			break;
		case GAINREDJEWEL:
			swprintf_s(msg, L"�������һ���챦ʯ������������3");
			break;
		case GAINSMALLBOTTLE:
			swprintf_s(msg, L"�������һ��СѪƿ������ֵ����200");
			break;
		case  GAINBIGBOTTLE:
			swprintf_s(msg, L"�������һ����Ѫƿ������ֵ����500");
			break;
		case GAINSWORD:
			swprintf_s(msg, L"�õ���������������10");
			break;
		case GAINMUTIPLEKEY:
			swprintf_s(msg, L"������˽�Կ�ף���Կ�ף�ͭԿ�׸�һ��!");
			break;
		case GAINARMOR:
			swprintf_s(msg, L"�õ����ܣ���������10");
			break;
		case GAINRADAR:
			swprintf_s(msg, L"��ù����״��F1��ʹ�ò鿴���������Ϣ,Esc���˳�");
			TextOut(g_mdc, 5 * SQUIREUNITWIDTH, 4.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			goto lop;
			break;
		case GAINSILVERSWORD:
			swprintf_s(msg, L"�õ���������������70");
			break;
		case GAINSMALLFLY:
			swprintf_s(msg, L"�������С���𣬵ȼ�����һ����");
			break;
		case GAINBULLION:
			swprintf_s(msg, L"�õ���飬���������300");
			break;
		case GAINGOADARMOR:
			swprintf_s(msg, L"�õ��ƽ�ܣ�����������85��");
			break;
		case GAINHOLYWATER:
			swprintf_s(msg, L"���ʥˮ������ֵ�ӱ�����");
			break;
		case GAINCOMPASS:
			swprintf_s(msg, L"�õ����̣���F2����ѡ����ת������ȥ����¥�㣬Esc���˳�");
			TextOut(g_mdc, 5 * SQUIREUNITWIDTH, 4.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			goto lop;
			break;
		case GAINCROSS:
			swprintf_s(msg, L"�������ʮ�ּܣ�����0������Ӵ��������ӹ�������������ֵ");
			TextOut(g_mdc, 5 * SQUIREUNITWIDTH, 4.75*SQUIREUNITHEIGHT, msg, wcslen(msg));
			goto lop;
			break;
		case GAINSTEELARMOR:
			swprintf_s(msg, L"�õ��ֶܣ�����������30��");
			break;
		case GAINSTEELSWORD:
			swprintf_s(msg, L"�õ��ֽ�������������30��");
			break;
		case  GAINBIGFLY:
			swprintf_s(msg, L"�õ�����𣡵ȼ�����������");
			break;
		case GAINSTARLIGHTSWORD:
			swprintf_s(msg, L"�õ��ǹ⽣������������120�㣡");
			break;
		case GAINSTARLIGHTARMOR:
			swprintf_s(msg, L"�õ��ǹ�ܣ�����������120�㣡");
			break;
		case GAINHAMMER:
			swprintf_s(msg, L"�õ��ǹ����ƣ�����С͵�ɴ�ͨ18�����ص���");
			break;
		case GAINGODSWORD :
			swprintf_s(msg, L"�õ��ǹ��񽣣�����������150��");
			break;
		case GAINGODARMOR:
			swprintf_s(msg, L"�õ���â��ܣ�����������190��");
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
//ս��������ý�Ҿ�������
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
		swprintf_s(msg, L"������� %d �㾭��ֵ��%d �����", g_CopyMonster.exp,g_CopyMonster.goad);
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
//�����״���ͼ����
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
		swprintf_s(msg, L"����   %d  ����   %d  ���   %d",g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].damage, g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].defence, g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].goad);
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
			swprintf_s(msg, L"����   %d  ����   %d  ��ʧ   ???", g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].exp, g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].HP );
		}
		else if (Hero.defence >= g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].damage&&Hero.damage > g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].defence)
		{
			swprintf_s(msg, L"����   %d  ����   %d  ��ʧ   %d", g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].exp, g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].HP,loss);
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
			swprintf_s(msg, L"����   %d  ����   %d  ��ʧ   %d", g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].exp, g_monster[(list[i] - 20) / 4][(list[i] - 20) % 4].HP,loss);
		}
		TextOut(g_mdc,5.5 * SQUIREUNITWIDTH + 1, (i+0.5)*SQUIREUNITHEIGHT, msg, wcslen(msg));
	}
	DeleteObject(hFont);
}
//������ͼ����
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
		swprintf_s(msg, L"��%d��", i);
		TextOut(g_mdc, (6.5+(i-1)/10*5) * SQUIREUNITWIDTH, 45+(i-1)%10*SQUIREUNITHEIGHT, msg, wcslen(msg));
	}
	loop:
	if (g_seleckeydown==TRUE)
	{
		g_compassswichtime++;
		SelectObject(g_mdc, g_cleanpen);//�˴���ʼ����Game_swich()���ִ��루�������Ҫ��һ�£�������
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
			swprintf_s(msg, L"��%d��", g_PileNo);
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
//¥����ת����������¥�����̻��ƺ���
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
		swprintf_s(msg, L"��%d��", g_PileNo);
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
//��������ѡ����ƺ���
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
				MessageBox(g_hwnd, L"��ľ�д浵Ŷ��", L"ħ�������ȹ���", 0);
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
			sum = MessageBox(NULL, L"���Ҫ�˳���", L"ħ�������ȹ���", MB_YESNO);
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
//������ƺ���
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
	swprintf_s(msg, L"��space��");
	TextOut(g_mdc, 7 * SQUIREUNITWIDTH, WINDOW_HEIGHT-30, msg, wcslen(msg));
	DeleteObject(hFont);
	if (g_guttime>3*WINDOW_HEIGHT)
	{
		mciSendString(L"close gut", NULL, 0, NULL);
		g_gut = FALSE;
		g_guttime = 0;
	}
}
//��ֻ��ƺ���
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
	swprintf_s(msg, L"��space��");
	TextOut(g_mdc, 7 * SQUIREUNITWIDTH, WINDOW_HEIGHT - 30, msg, wcslen(msg));
	DeleteObject(hFont);
	if (g_guttime > 3 * WINDOW_HEIGHT)
	{
		g_end = FALSE;
		g_guttime = 0;
		g_start = TRUE;
	}
}
//�������ֿ��ƺ���
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
//������һ�����ڵ��Թ������άλ��
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
//����������ͷ��ڴ�
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
