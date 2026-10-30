
extern unsigned int ai_MissionSeed;
extern int ai_Unk75e130;
extern char * ai_Unk75ec60;
extern char ai_Unk75fba0[2048];
extern int cm_SaveAcks[8][2];
/* ai_Unk770b89: view of ai_Chars (tools/data_views.py) */
/* ai_Unk770cec: view of ai_Chars (tools/data_views.py) */
extern int ai_Unk91b6e0[0x800][2];
extern char ai_Unk91f700[0x5a00];
extern int ai_Unk925510;
extern int ai_Unk92bca0;
extern int ai_Unk92bca4;
extern int ai_Unk92bcc0[128];
extern int ai_Unk92bec0;
extern unsigned short ai_Unk92bee0[768];
extern unsigned int ai_Unk92c4e0;
extern int ai_Unk92c500[1];
extern int ai_Unk92c504[1];
extern int ai_Unk92c508[1];
extern char ai_Unk92c50c[4];
extern int ai_Unk92c510[60];
extern char ai_Unk92c600[1152];
extern int ai_Unk92ca80[16];
extern int ai_Unk92cac0;
extern char ai_Unk92cae0[16][0x35c];
extern float ai_Unk9300a0[0xb4];
extern char * ai_Unk930380[32];
extern char ai_Unk930400[192];
extern G18POP ai_Unk9304c0[16];
extern char ai_Unk930600[21376];
extern char * ai_Unk935980[32];
extern char ai_Unk935a00[512];
extern float ai_Unk935d00;
extern int ai_Unk935d20[32];
extern G18LOG ai_Unk935da0[15];
extern int ai_Unk9363f4;
extern int ai_Unk936400;
extern int ai_Unk936404;
extern int ai_Unk936408;
extern unsigned int ai_Unk936420[9];
extern unsigned char ai_Unk936496;
extern int ai_ModeMask;
extern char ai_Unk9364e0[752];
extern int ai_Unk9367d0;
extern int ai_Unk9367d4;
extern G17MSG ai_Unk9367e0[0x2000];
extern int cm_LastSyncCount;
extern int ai_Unka687e4;
extern unsigned char ai_Unka68800[1024];
extern G17MSG * ai_Unka68c00;
extern unsigned int ai_Unka68c04;
extern int ai_Unka68c08;
extern G17FIELDS ai_Unka68c20;
extern G17FIELDS ai_Unka68ca0;
extern unsigned int ai_Unka68d1c;
extern int ai_Unka68d20;
extern G17MSG * ai_Unka68d24;
extern char * cm_FieldObject;
extern int ai_Unka68d30;
extern char * ai_Unka68d34;
extern unsigned char ai_Unka68d40[1024];
extern G17MSG ai_Unka69140[0x8000];
extern char * ai_Unkf31140;
extern int ai_Unkf31144;
extern G17STAT ai_Unkf31160[0x8000];
extern G17MSG * cm_SendQueue;
extern unsigned int ai_Unkf81164;
extern G17MSG ai_Unkf81180;
extern unsigned int ai_Unkf8121c;
extern char ai_Unkf895a0[0xb00];
extern char ai_Unkf8aae0[0x80];
extern int ai_Unkfa5f00;
extern int ai_Unkfa5f04;
/* ai_Unkfd7c50: view of d3_Scenes (tools/data_views.py) */
/* ai_Unk143a610: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143a680: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143a9f4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143ab0c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143b3d8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143b3e4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e674: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8a4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8a8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8ac: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8b0: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8b4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8bc: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8c0: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8c8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8e4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8f4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8f8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8fc: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e948: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e954: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e960: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1440c88: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk14413c8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441488: view of txt_Ptrs (build/mk_txt_views.py) */
extern char * ai_Unk1468ae0[0x300];
float ai_Unk5eb780 = 1.0f;  /* 0x5eb780 */
float ai_Unk5eb788 = 0.0f;  /* 0x5eb788 */
/* 0x5eb798 / 0x5eb79c are the pooled floats 70.0f / 0.25f (0x428c0000 / 0x3e800000), not a
   double: a 8-byte definition here claims two literals another unit owns. */
/* 0x5eb7ac is the pooled float 0.5f (0x3f000000): use the literal at the call sites. */
double ai_Unk5eb7b0 = 15.0;  /* 0x5eb7b0 */
float ai_Unk5eb7dc = 3.14159274f;  /* 0x5eb7dc */
float ai_Unk5eb7fc = 75.0f;  /* 0x5eb7fc */
float ai_Unk5eb81c = 30.0f;  /* 0x5eb81c */
float ai_Unk5eb878 = 1.39999998f;  /* 0x5eb878 */
/* 0x5eb8b0 is the pooled float 0.2f (0x3e4ccccd): use the literal at the call sites. */
double ai_Unk5eb8b8 = 1.2;  /* 0x5eb8b8 */
double ai_Unk5eb8e0 = 20.0;  /* 0x5eb8e0 */
float ai_Unk5eb94c = 0.75f;  /* 0x5eb94c */
float ai_Unk5eb988 = 5.0f;  /* 0x5eb988 */
double ai_Unk5eb990 = 10.0;  /* 0x5eb990 */
double ai_Unk5eb9b0 = 0.25;  /* 0x5eb9b0 */
const double ai_Unk5eb9b8 = 0.2857142857142857;  /* 0x5eb9b8 */
const char ai_AxisPair[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 63, 0, 0, 0, 0, 0, 0, -128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x5ebb74 */
const float ai_Unk5ebba4[] = { 0.0f, 0.0f, 0.0f, 0.0f };  /* 0x5ebba4, the origin (d3_Origin): read-only, the original keeps it in .rdata */
double ai_Unk5ebbe8 = 25.0;  /* 0x5ebbe8 */
float ai_Unk5ebc1c = 1.75f;  /* 0x5ebc1c */
double ai_Unk5ece60 = 0.01;  /* 0x5ece60 */
const float ai_Unk5ecebc = 1.25f;  /* 0x5ecebc */
double ai_Unk5edbd0 = 0.050000000000000003;  /* 0x5edbd0 */
const int ai_Unk5edc10[] = { 1155, 937, 770, 577, 385, 0 };  /* 0x5edc10 */
const double ai_Unk5edc28 = 1.8;  /* 0x5edc28 */
const float ai_Unk5edc50 = 525.0f;  /* 0x5edc50 */
const double ai_Unk5edc58 = 1.1000000000000001;  /* 0x5edc58, read-only: the original keeps it in .rdata */
const double ai_Unk5edc60 = 6.5012084524677184e-315;  /* 0x5edc60 */
const double ai_Unk5edc68 = 0.0055555555555555558;  /* 0x5edc68 */
int ai_Unk5f8438 = -1;  /* 0x5f8438 */
int ai_Unk5f8b10 = 6251111;  /* 0x5f8b10 */
float ai_Unk5fe708 = 1.0f;  /* 0x5fe708 */
int ai_Unk60bd80 = 770;  /* 0x60bd80 */
unsigned int cm_SpeedLevel = 2;  /* 0x60bd84, = 2 (0..4) */
int ai_Unk60bd88 = -1;  /* 0x60bd88, = -1 */
int ai_Unk60bd8c = -1;  /* 0x60bd8c, = -1 */
int ai_Unk60bd90 = -1;  /* 0x60bd90, = -1 */
int ai_Unk60bd94 = -1;  /* 0x60bd94, = -1 */
int (*ai_Unk60bd98[0x6a])(G17MSG *m, G17STAT *s) = { ai_Unk4c6a20, ai_Unk4c6a50, ai_AlwaysTrue, ai_Unk4c6a20, ai_Unk4c6a60, ai_Unk4c6a20, ai_Unk4c6a20, ai_Unk4c6a20, ai_Unk4c6a90, ai_Unk4c6ad0, ai_Unk4c6b20, ai_Unk4c6bf0, ai_Unk4c6cd0, ai_Unk4c6e10, ai_Unk4c6ec0, ai_Unk4c6f60, ai_Unk4c7010, ai_Unk4c70b0, ai_Unk4c78c0, ai_Unk4c7c30, ai_Unk4c7d80, ai_Unk4c7e20, ai_Unk4c7e90, ai_Unk4c7f70, ai_Unk4c8050, ai_Unk4c8110, ai_Unk4c81d0, ai_Unk4c8240, ai_Unk4c85c0, ai_Unk4c87a0, ai_Unk4c88a0, ai_Unk4c8990, ai_Unk4c89b0, ai_Unk4c8de0, ai_Unk4c8e70, ai_Unk4c9090, ai_Unk4c90d0, ai_Unk4c9110, ai_Unk4c9170, ai_Unk4c9280, ai_Unk4c9340, ai_Unk4c93b0, ai_Unk4c9400, ai_Unk4c9450, ai_Unk4c9520, ai_Unk4c9740, ai_Unk4c97c0, ai_Unk4c9830, ai_Unk4c99b0, ai_Unk4c9c10, ai_Unk4c9c80, ai_Unk4c9d00, ai_Unk4c9d80, ai_Unk4c9dc0, ai_Unk4ca8a0, ai_Unk4caa20, ai_Unk4caaa0, ai_Unk4cab40, ai_Unk4cac40, ai_Unk4cb100, ai_Unk4cb210, ai_Unk4cb250, ai_Unk4cb4a0, ai_Unk4cb6e0, ai_Unk4cb730, ai_Unk4cb7c0, ai_Unk4cb7f0, ai_Unk4cb850, ai_Unk4cb890, ai_Unk4cb8d0, ai_Unk4cb910, ai_Unk4cb940, ai_Unk4cb9b0, ai_Unk4cba10, ai_Unk4cbb90, ai_Unk4cbbd0, ai_Unk4cbca0, ai_Unk4cbff0, ai_Unk4cc050, ai_Unk4cc110, ai_Unk4cc1b0, ai_Unk4cc270, ai_Unk4cc330, ai_Unk4cc390, ai_Unk4cc510, ai_Unk4cc7b0, ai_Unk4cc8e0, ai_Unk4cc920, ai_Unk4cc980, ai_Unk4cca10, ai_Unk4ccaa0, ai_Unk4ccb10, ai_Unk4ccbd0, ai_Unk4ccc20, ai_Unk4ccce0, ai_Unk4cce00, ai_Unk4cce10, ai_Unk4cced0, ai_Unk4ccf10, ai_Unk4ccf90, ai_Unk4cd010, ai_Unk4cd130, ai_Unk4cd180, ai_Unk4cd1e0, ai_Unk4cd220, ai_Unk4cd2f0 };  /* 0x60bd98, the handler of each message type */
int (*ai_Unk60bf40[0x6a])(G17MSG *m) = { sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c6350, sv_ReturnZero, ai_Unk4c63e0, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c65b0, sv_ReturnZero, sv_ReturnZero, ai_Unk4c66c0, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c6750, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c6780, ai_Unk4c67a0, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c67c0, ai_Unk4c6870, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c69c0, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero };  /* 0x60bf40, the check of each type (groups 5..6) */
char ai_Unk60c4a0[20] = "cm_ExChrWalkToDummy";  /* 0x60c4a0 */
char ai_Unk60c6c0[4] = { 36, 65, 0, 0 };  /* 0x60c6c0 */
int ai_Unk60c7b0[2] = { 1061109567, 4144959 };  /* 0x60c7b0 */
char ai_Unk60c7c0[748] = { 84, 1, 0, 0, 120, 5, 20, 0, 1, 4, 1, 0, 0, 0, -96, 66, 2, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 17, 0, 0, 0, 0, 0, 0, 86, 1, 0, 0, 95, 10, 15, 0, 1, 4, 1, 0, 0, 0, -96, 66, 5, 107, 97, 109, 112, 102, 47, 115, 116, 114, 101, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 17, 0, 0, 0, 0, 0, 0, 88, 1, 0, 0, 80, 20, 40, 0, 1, 4, 1, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 104, 105, 101, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101, 17, 0, 0, 0, 0, 0, 0, 114, 1, 0, 0, 110, 5, 10, 0, 1, 5, 0, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 17, 0, 0, 0, 0, 0, 0, 110, 1, 0, 0, 100, 5, 10, 0, 1, 5, 0, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 17, 0, 0, 0, 0, 0, 0, 116, 1, 0, 0, 100, 0, 30, 1, 2, 6, 0, 0, 0, 0, 72, 67, 2, 107, 97, 109, 112, 102, 47, 119, 101, 114, 102, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 104, 17, 0, 0, 2, 0, 0, 0, 118, 1, 0, 0, 90, 0, 30, 1, 1, 6, 0, 0, 0, 0, 72, 67, 2, 107, 97, 109, 112, 102, 47, 119, 101, 114, 102, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 17, 0, 0, 2, 0, 0, 0, 94, 1, 0, 0, 60, 5, 20, 1, 1, 3, 0, 0, 0, 0, -56, 67, 5, 107, 97, 109, 112, 102, 47, 115, 99, 104, 105, 101, 115, 115, 101, 110, 95, 112, 105, 115, 116, 111, 108, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 106, 17, 0, 0, 1, 0, 0, 0, 96, 1, 0, 0, 70, 10, 30, 1, 1, 3, 0, 0 };  /* 0x60c7c0 */
float ai_Unk60cb58 = 1.0f;  /* 0x60cb58 */
int ai_Unk60cb5c = -1;  /* 0x60cb5c */
int ai_Unk60cb60 = -1;  /* 0x60cb60 */
int ai_Unk60cb64 = -1;  /* 0x60cb64 */
int ai_Unk60cb68 = -1;  /* 0x60cb68 */
int ai_Unk60d360 = 464838;  /* 0x60d360, the frame wait of the dialogs ( */
int ai_Unk60d364 = 10;  /* 0x60d364 */
int ai_Unk60d368 = -1;  /* 0x60d368 */
int ai_Unk60d36c = -1;  /* 0x60d36c, -1 or the object waited for ( */
int ai_Unk60d370 = 1;  /* 0x60d370 */
int ai_Unk60d374 = 1;  /* 0x60d374 */
char ai_Unk60dd24 = 48;  /* 0x60dd24 */
char ai_Unk60dd68 = 49;  /* 0x60dd68 */
/* 0x4db700 .. 0x4dd95d (one function; Ghidra's 0x4dc7c0 lies inside it), the trial scene (the run function of the scene kind 2, called with the party m) of a session request m (+0x30 count, +0x34 .. +0x48 the characters: accused, accuser, judge, two
   assessors ..): "Gericht.ed3" / "ADDON\\PROZESS_NEU.sbf", the scenes "cutscenes\\prozess\\prozess-*.esc" (create, judge, assessors, intro,
   accuser, accused), the texts 0x1491 .. 0x1557 through 0x515960 and the voices "_PROZESS_*" (0x4e32d0 / 0x4e32b0); the accusation, the
   charge (witnesses), the question of guilt, the torture sub-scenes "cutscenes\\prozess\\folter*" (the instrument names 0x60dad0 ..
   0x60db84 are the .esc names), the vote ("prozess-abstimmen-1 .. 3"), the verdict (guilty / not guilty, penalties 0x4f4420 / 0x4f48f0 /
   0x4f4460) and the cleanup of all sound slots. The stack frame is 0x760 bytes. ROUGH: the structure and the calls (read from the call
   digest, arguments in call order but mostly 0), not the arguments of 0x4f2720 / 0x4f3580 / 0x4f3b40 and the mood floats (0x3fb33333 ..) */
char ai_Unk60de4c = 50;  /* 0x60de4c, scene slots of the trial */
char ai_Unk60eae0[10][24] = { "DUMMY_NPC", "MEISTER_NPC", "POLITICIAN_NPC", "AI_NPC", "NPC", "PLAYER", "REMOTE_PLAYER", "RELATIVE_PLAYER", "USED_PLAYER", "INVISIBLE_NPC" };  /* 0x60eae0, the names of the NPC kinds */
char ai_Unk623ac0[] = { 115, 107, 121, 95, 115, 99, 104, 119, 101, 114, 95, 48, 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 64, -51, -52, 76, 63, 0, 0, -128, -66, 115, 107, 121, 95, 115, 99, 104, 119, 101, 114, 95, 48, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 64, -51, -52, 76, 63, 0, 0, -128, -66, 115, 107, 121, 95, 100, 117, 110, 107, 101, 108, 95, 48, 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 64, -51, -52, 76, 63, 0, 0, -128, -66, 0, 0, 0, 0 };  /* 0x623ac0, 76-byte sky records: Sky_Schwer / Sky_Mittel / Sky_Schoen */
int ai_Unk624570 = 1;  /* 0x624570 */
int ai_Unk624574 = 1;  /* 0x624574 */
int ai_Unk624578 = 1;  /* 0x624578 */
char ai_Unk625820[115][0x20] = { "HE_NULL", "HE_HISTORIE", "HE_UPGRADE_DUMMY", "HE_ABBAU", "HE_PRODUKTION", "HE_PRODUKTION_FERTIG_MSG", "HE_ARBEITER_EINSTELLEN", "HE_MEISTER_EINSTELLEN", "HE_VORKOMMEN_SUCHEN", "HE_NEUE_VORKOMMEN_SUCHEN", "HE_NEUE_VORKOMMEN_SUCHEN_MSG", "HE_AI_WATCH_TRANSPORT", "HE_OBJEKT_BAUEN", "HE_GEBAEUDE_BAUEN", "HE_MAUER_BAUEN", "HE_TRANSPORT", "HE_REPARATUR", "HE_MESSAGEBOX", "HE_LAGERSLOTS_BAUEN", "HE_LAGER_ERWEITERN", "HE_KRAEUTER_SAMMELN", "HE_GETIER_FANGEN", "HE_SPIELER_TRAINIEREN", "HE_GEHEIM_TREFFEN", "HE_SPIONAGE", "HE_SABOTAGE", "HE_PRUEGEL", "HE_GEB_VERKAUFEN", "HE_GEB_RENOVIEREN", "HE_GEB_VERMIETEN_DUMMY", "HE_GEB_UPGRADEN", "HE_GEB_RAUM_UPGRADEN", "HE_GEB_ABREISSEN", "HE_GEB_KAUFANGEBOT", "HE_SITZUNG", "HE_KREDIT", "HE_MEISTER_PRUEFUNG", "HE_MEISTER_PRUEFUNG_MSG", "HE_MEISTER_BESTANDEN", "HE_GELDBOTE", "HE_ERNTEN", "HE_STAMMTISCH_MSG", "HE_STAMMTISCH", "HE_GOTTESDIENST", "HE_PROZESS", "HE_DROHEN_MSG", "HE_DROHEN", "HE_ANSCHWAERZEN_MSG", "HE_BUCH_SCHREIBEN", "HE_ENTLASSEN", "HE_BEFOERDERN", "HE_PERSONAL_KUENDIGEN", "HE_PERSONAL_STEHLEN", "HE_PAMPHLET", "HE_FEST_MSG", "HE_FEST", "HE_GELAGE", "HE_KERKER_MEISTER_BESTECHEN", "HE_SIM_PERSONAL", "HE_MARKT_SUPERVISOR", "HE_ENTFUEHREN", "HE_SPIELER_ENTFUEHRT", "HE_LOESEGELD", "HE_EINBRUCH", "HE_GEB_SPIONAGE", "HE_WERBUNG", "HE_RANDOM_EVENT", "HE_STREIFE", "HE_RAZZIA", "HE_ERPRESSUNG", "HE_BELEIDIGUNG_MSG", "HE_BELEIDIGUNG", "HE_UEBERFALL", "HE_ANGRIFF", "HE_RANDOM_GESELLE", "HE_FURCHT_EINFLOESSEN", "HE_MISSION_SUPERVISOR", "HE_MISSION_DONE_MSG", "HE_EVT_GROSS_BRAND", "HE_EVT_BRAND", "HE_EVT_WIRBELSTURM", "HE_EVT_RAUBRITTER", "HE_EVT_KOMMENTARE", "HE_EVT_STADTFEST", "HE_EVT_SPENDE", "HE_EVT_BETEILIGUNG_MSG", "HE_EVT_BETEILIGUNG", "HE_EVT_SOELDNER_PLUENDERN", "HE_EVT_SOELDNER_MARODIEREN", "HE_EVT_PEST", "HE_ESKORTE_VERHINDERN", "HE_GELDSTRAFE", "HE_PRANGER", "HE_NACHWUCHS", "HE_BERUFSAUSBILDUNG", "HE_UNI", "HE_TALENT_SPIELER", "HE_TASCHENDIEB", "HE_SCHUTZGELD", "HE_SCHUTZGELD_MSG", "HE_BETRIEBS_PRUEFUNG", "HE_UMLAND_PATROL", "HE_SIM_UNFALL", "HE_PACHT", "HE_SIM_WIRTSHAUS", "HE_TITEL_VERGABE", "HE_AMT_VERGABE", "HE_BARDE", "HE_HEILEN", "HE_SIM_EINWANDERUNG", "HE_INIT_BETRIEB", "HE_GELIEBTE", "HE_SIM_KRANKHEITEN", "HE_DIE_PLAYER", "HE_KILL_PLAYER" };  /* 0x625820, names ("HE_NULL", ..) */
int ai_Unk631064 = 0;  /* 0x631064 */
int ai_Unk631084 = 0;  /* 0x631084 */
char * ai_Unk6332b4 = 0;  /* 0x6332b4 */
int ai_Unk6333cc = 0;  /* 0x6333cc, a clock */
unsigned int ai_Unk63349c = 0;  /* 0x63349c */
int ai_Unk6f0cd0 = 0;  /* 0x6f0cd0, received messages */
int ai_Unk6f0cd4 = 0;  /* 0x6f0cd4 */
int ai_Unk6f0cd8 = 0;  /* 0x6f0cd8 */
int ai_Unk6f0cdc = 0;  /* 0x6f0cdc */
char * ai_Unk6f0ce0 = 0;  /* 0x6f0ce0 */
int ai_Unk6f0ce4 = 0;  /* 0x6f0ce4, fight paused */
int ai_Unk6f0ce8 = 0;  /* 0x6f0ce8 */
int ai_Unk6f0cec = 0;  /* 0x6f0cec */
char * ai_Unk6f0cf0 = 0;  /* 0x6f0cf0 */
int ai_Unk6f0cf4 = 0;  /* 0x6f0cf4 */
int ai_Unk6f0cf8 = 0;  /* 0x6f0cf8 */
int ai_Unk6f0cfc = 0;  /* 0x6f0cfc */
char * ai_Unk6f0d04 = 0;  /* 0x6f0d04 */
int ai_Unk6f0d08 = 0;  /* 0x6f0d08 */
int ai_Unk6f0d0c = 0;  /* 0x6f0d0c */
int ai_Unk6f0d10 = 0;  /* 0x6f0d10 */
int ai_Unk6f0d14 = 0;  /* 0x6f0d14 */
int ai_Unk6f0d18 = 0;  /* 0x6f0d18 */
int ai_Unk6f0d1c = 0;  /* 0x6f0d1c */
int ai_Unk6f0d20 = 0;  /* 0x6f0d20 */
int ai_Unk6f0d24 = 0;  /* 0x6f0d24 */
int ai_Unk6f0d28 = 0;  /* 0x6f0d28 */
int ai_Unk6f0d2c = 0;  /* 0x6f0d2c */
int ai_Unk6f0d30 = 0;  /* 0x6f0d30 */
int ai_Unk6f0d34 = 0;  /* 0x6f0d34 */
int ai_Unk6f0d38 = 0;  /* 0x6f0d38 */
int ai_Unk6f0d3c = 0;  /* 0x6f0d3c */
int ai_Unk6f0d40 = 0;  /* 0x6f0d40 */
int ai_Unk6f0d44 = 0;  /* 0x6f0d44 */
int ai_Unk6f0d48 = 0;  /* 0x6f0d48 */
int ai_Unk6f0d4c = 0;  /* 0x6f0d4c */
int ai_Unk6f0d50 = 0;  /* 0x6f0d50 */
int ai_Unk6f0d54 = 0;  /* 0x6f0d54 */
int ai_Unk6f0d58 = 0;  /* 0x6f0d58 */
int ai_Unk6f0d5c = 0;  /* 0x6f0d5c */
int ai_Unk6f0d60 = 0;  /* 0x6f0d60 */
int ai_Unk6f0d64 = 0;  /* 0x6f0d64 */
int ai_Unk6f0d68 = 0;  /* 0x6f0d68 */
int ai_Unk6f0d6c = 0;  /* 0x6f0d6c */
int ai_Unk6f0d70 = 0;  /* 0x6f0d70 */
char * ai_Unk6f0d74 = 0;  /* 0x6f0d74 */
int ai_Unk6f0d78 = 0;  /* 0x6f0d78 */
int ai_Unk6f0d7c = 0;  /* 0x6f0d7c */
int ai_Unk6f0d80 = 0;  /* 0x6f0d80 */
int ai_Unk6f0d84 = 0;  /* 0x6f0d84 */
int ai_Unk6f0d88 = 0;  /* 0x6f0d88 */
int ai_Unk6f0d8c = 0;  /* 0x6f0d8c */
int * ai_Unk6f0d90 = 0;  /* 0x6f0d90 */
int ai_Unk6f0d94 = 0;  /* 0x6f0d94 */
unsigned int ai_Unk6f0d98 = 0;  /* 0x6f0d98 */
int ai_Unk6f0d9c = 0;  /* 0x6f0d9c, the scene nesting depth */
char * ai_Unk6f0da0 = 0;  /* 0x6f0da0, the fade */
unsigned int ai_Unk6f0da4 = 0;  /* 0x6f0da4, fight start (d3_Time) */
unsigned char ai_Unk6f0da8[2] = {0};  /* 0x6f0da8, +2 / +3: the left / right insult won, +4 / +5 the shot won */
unsigned char ai_Unk6f0daa = 0;  /* 0x6f0daa */
int ai_Unk6f0db0 = 0;  /* 0x6f0db0 */
int ai_Unk6f0db4 = 0;  /* 0x6f0db4 */
int ai_Unk6f0db8 = 0;  /* 0x6f0db8 */
int ai_Unk6f0dbc = 0;  /* 0x6f0dbc */
int ai_Unk6f0dc0 = 0;  /* 0x6f0dc0 */
char ai_Unk6f0dc4 = 0;  /* 0x6f0dc4 */
char * ai_Unk6f22e8 = 0;  /* 0x6f22e8 */
int ai_Unk6f22f8 = 0;  /* 0x6f22f8 */
int ai_Unk6f2438 = 0;  /* 0x6f2438 */
int ai_Unk6f244c = 0;  /* 0x6f244c */
int cm_NetFlags = 0;  /* 0x6f2594, flags */
int cm_RequestedSaveSlot = 0;  /* 0x6f25c8 */
char cm_RequestedSaveName[72] = {0};  /* 0x6f25d0 */
/* ai_Unk6f35ca: view of ai_GfxFloor (tools/data_views.py) */
unsigned int ai_MissionSeed = 0;  /* 0x75e120, the random seed of the missions (as unk_4b39f0.h) */
int ai_Unk75e130 = 0;  /* 0x75e130, the year of the clock copy 0x75e130..0x75e13e (an AITIME; its other fields are ls.c's ai_Unk75e134 / 75e138 / 75e13c) */
char * ai_Unk75ec60 = 0;  /* 0x75ec60 */
char ai_Unk75fba0[2048] = {0};  /* 0x75fba0 */
int cm_SaveAcks[8][2] = {0};  /* 0x76fa20 */
/* ai_Unk770d8c: view of ai_Chars (tools/data_views.py) */
int ai_Unk91b6e0[0x800][2] = {0}; /* 0x91b6e0 */
char ai_Unk91f700[0x5a00] = {0}; /* 0x91f700, the crime records (0x2d bytes each) up to 0x925100; f3gm's goods
                                    table ai_Unk91f716 is the same storage, a view at +0x16 */
int ai_Unk925510 = 0;  /* 0x925510 */
G28REC ai_Unk925520[96] = {0};  /* 0x925520 */
int ai_Unk92bca0 = 0;  /* 0x92bca0 */
int ai_Unk92bca4 = 0;  /* 0x92bca4 */
int ai_Unk92bcc0[128] = {0};  /* 0x92bcc0 */
int ai_Unk92bec0 = 0;  /* 0x92bec0 */
unsigned short ai_Unk92bee0[768] = {0};  /* 0x92bee0 */
unsigned int ai_Unk92c4e0 = 0;  /* 0x92c4e0, the random seed of the scenes */
int ai_Unk92c500[1] = {0};  /* 0x92c500, the scene kind table: run, callbacks, flag, check */
int ai_Unk92c504[1] = {0};  /* 0x92c504, the scene kind table (5 ints per kind) */
int ai_Unk92c508[1] = {0};  /* 0x92c508 */
char ai_Unk92c50c[4] = {0};  /* 0x92c50c */
int ai_Unk92c510[60] = {0};  /* 0x92c510, the answer callbacks of the kind table (5 ints per kind) */
char ai_Unk92c600[1152] = {0};  /* 0x92c600 */
int ai_Unk92ca80[16] = {0};  /* 0x92ca80 */
int ai_Unk92cac0 = 0;  /* 0x92cac0 */
char ai_Unk92cae0[16][0x35c] = {0};  /* 0x92cae0 */
float ai_Unk9300a0[0xb4] = {0};  /* 0x9300a0 */
char * ai_Unk930380[32] = {0};  /* 0x930380 */
char ai_Unk930400[192] = {0};  /* 0x930400 */
G18POP ai_Unk9304c0[16] = {0};  /* 0x9304c0 */
char ai_Unk930600[21376] = {0};  /* 0x930600 */
char * ai_Unk935980[32] = {0};  /* 0x935980 */
char ai_Unk935a00[512] = {0};  /* 0x935a00 */
float ai_Unk935d00 = 0;  /* 0x935d00 */
int ai_Unk935d20[32] = {0};  /* 0x935d20 */
G18LOG ai_Unk935da0[15] = {0};  /* 0x935da0 */
int ai_Unk9363f4 = 0;  /* 0x9363f4 */
int ai_Unk936400 = 0;  /* 0x936400 */
int ai_Unk936404 = 0;  /* 0x936404 */
int ai_Unk936408 = 0;  /* 0x936408 */
unsigned int ai_Unk936420[9] = {0};  /* 0x936420, the count then the eight player states (0x24 bytes) */
unsigned char ai_Unk936496 = 0;  /* 0x936496 */
int ai_ModeMask = 0;  /* 0x9364dc */
char ai_Unk9364e0[752] = {0};  /* 0x9364e0 */
int ai_Unk9367d0 = 0;  /* 0x9367d0 */
int ai_Unk9367d4 = 0;  /* 0x9367d4 */
G17MSG ai_Unk9367e0[0x2000] = {0};  /* 0x9367e0, the free receive records */
int cm_LastSyncCount = 0;  /* 0xa687e0, "cm_LastSyncCount" (net.h) */
int ai_Unka687e4 = 0;  /* 0xa687e4, d3_Time of the start */
unsigned char ai_Unka68800[1024] = {0};  /* 0xa68800, a long message to split (ai_Unka68d30 bytes) */
G17MSG * ai_Unka68c00 = 0;  /* 0xa68c00, the free list */
unsigned int ai_Unka68c04 = 0;  /* 0xa68c04 */
int ai_Unka68c08 = 0;  /* 0xa68c08, bytes used in ai_Unka68c20.buf */
G17FIELDS ai_Unka68c20 = {0};  /* 0xa68c20 */
G17FIELDS ai_Unka68ca0 = {0};  /* 0xa68ca0 */
unsigned int ai_Unka68d1c = 0;  /* 0xa68d1c, "cm_LastRequCount" */
int ai_Unka68d20 = 0;  /* 0xa68d20, bytes used in ai_Unka68ca0.buf */
G17MSG * ai_Unka68d24 = 0;  /* 0xa68d24, the received messages */
char * cm_FieldObject = 0;  /* 0xa68d2c, the object of the field block (fields are offsets into it) */
int ai_Unka68d30 = 0;  /* 0xa68d30, length of ai_Unka68800 */
char * ai_Unka68d34 = 0;  /* 0xa68d34 */
unsigned char ai_Unka68d40[1024] = {0};  /* 0xa68d40, a joined long message */
G17MSG ai_Unka69140[0x8000] = {0};  /* 0xa69140, the send ring */
char * ai_Unkf31140 = 0;  /* 0xf31140, the load buffer ("cm_LoadBuf") */
int ai_Unkf31144 = 0;  /* 0xf31144 */
G17STAT ai_Unkf31160[0x8000] = {0};  /* 0xf31160 */
G17MSG * cm_SendQueue = 0;  /* 0xf81160, the messages to send */
unsigned int ai_Unkf81164 = 0;  /* 0xf81164, length of ai_Unka68d40 */
G17MSG ai_Unkf81180 = {0};  /* 0xf81180, the message being received */
unsigned int ai_Unkf8121c = 0;  /* 0xf8121c */
char ai_Unkf895a0[0xb00] = {0}; /* 0xf895a0 */
char ai_Unkf8aae0[0x80] = {0}; /* 0xf8aae0, the d2 caption / status text buffer (gfx.h's d2_CaptionCopy, 0x80 bytes up to d2_InputDragValue at 0xf8ab60) */
/* ai_Unkf8f39c: view of d2_Wins (tools/data_views.py) */
int ai_Unkfa5f00 = {0}; /* 0xfa5f00 */
int ai_Unkfa5f04 = {0}; /* 0xfa5f04 */
/* ai_Unk144a160: view of txt_Ptrs (build/mk_txt_views.py) */
char * ai_Unk1468ae0[0x300] = {0}; /* 0x1468ae0, the objects */
