
extern int ai_Unk75ec20;
extern int ai_Unk75fb80;
extern int ai_Unk75fb84;
extern int ai_Unk75fb88;
extern int ai_Unk75fb90;
extern int ai_Unk75fb94;
extern int ai_Unk75fb98;
extern float ai_Unk760420;
extern int ai_Unk760440;
extern int ai_Unk760464;
extern int ai_Unk760480;
extern int ai_Unk760484;
extern int ai_Unk760548;
extern int ai_Unk76054c;
extern float ai_Unk760550[0x18];
extern float ai_Unk7605b0[0x18];
extern int ai_Unk760610;
extern int ai_Unk760614;
extern int ai_Unk76061c;
extern int ai_Unk760620;
extern int ai_Unk760624;
extern int ai_Unk760628;
extern int d2_HitWindow;
extern int ai_Unkfb9060;
extern int ai_Unkfb9064;
extern int ai_Unkfb9068;
extern int ai_Unkfb906c;
extern int ai_Unkfba9c0[168];
/* ai_Unk143a3a8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f68: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f6c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f70: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f74: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f78: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441f7c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144adb0: view of txt_Ptrs (build/mk_txt_views.py) */
int ai_Unk5fcc78 = 1128792064;  /* 0x5fcc78 */
int ai_Unk5fcc7c = 1128792064;  /* 0x5fcc7c */
int ai_Unk5fcc80 = 1128792064;  /* 0x5fcc80 */
int ai_Unk5fe710 = -1;  /* 0x5fe710 */
float sv_FramesPerTick = 30.0f;  /* 0x62378c */
int sv_TickFrames = 30;  /* 0x623790, the frames drawn since the last timer tick (counted by the frame loop) */
char ai_Unk623794[] = "\377\376\005\005%\377\377/\021\005\377\030\3746\343\377\231\005\005\377\366z\005\005\266W\377<\005P\031H\005\013\304^\305\005g\357\234\231\005\010r";  /* 0x623794, the table of */
int sv_StatusTime = -1;  /* 0x6237c4, when the status line was set (d3_Time) */
int ai_Unk6237cc = 1;  /* 0x6237cc */
/* 0x6237d0, the camera limits: three settings of (height, angle), first the near view pair and then (+3 pairs)
   the far one. ai_ApplyGfxOptions (0x5d5c40) copies the pair of the camera_limits option (ai_GfxFloor.byte[1])
   into sv_CameraHeightNear .. sv_CameraAngleFar; setting 0 has the same near and far pair (no zoom) */
float sv_CameraLimits[12] = {
    1200.0f, -0.9f,
    650.0f, -0.471238911f,
    190.0f, -0.104719758f,
    1200.0f, -0.9f,
    1800.0f, -1.08210409f,
    2400.0f, -1.04719758f,
};  /* 0x6237d0 */
float sv_CameraHeightNear = 450.0f;  /* 0x623800 */
float sv_CameraAngleNear = -0.471238911f;  /* 0x623804 */
float sv_CameraHeightFar = 1600.0f;  /* 0x623808 */
float sv_CameraAngleFar = -1.08210409f;  /* 0x62380c */
float ai_Unk623818 = 1.0f;  /* 0x623818 */
char ai_Unk62381c = 1;  /* 0x62381c */
int ai_Unk623820 = -1;  /* 0x623820 */
int ai_Unk623824 = -1;  /* 0x623824 */
int ai_Unk623828 = -1;  /* 0x623828 */
int ai_Unk62382c = -1;  /* 0x62382c */
int ai_Unk623830 = -1;  /* 0x623830 */
int ai_Unk623834 = -1;  /* 0x623834 */
int ai_Unk623838 = -1;  /* 0x623838 */
int ai_Unk62383c = -1;  /* 0x62383c */
int ai_Unk623840 = -1;  /* 0x623840 */
int ai_Unk623844 = -1;  /* 0x623844 */
int ai_Unk623848 = -1;  /* 0x623848 */
int ai_Unk62384c = -1;  /* 0x62384c */
int ai_Unk623850 = -1;  /* 0x623850 */
int sv_PanelPic = -1;  /* 0x623854 */
int sv_PlayerBarForm = -1;  /* 0x62385c, the player bar's form, -1 = closed */
int sv_PanelForm = -1;  /* 0x623860 */
int ai_Unk623864 = -1;  /* 0x623864 */
int ai_Unk623868 = -1;  /* 0x623868 */
int ai_Unk62386c = -1;  /* 0x62386c */
int ai_Unk62387c = -1;  /* 0x62387c */
int ai_Unk623880 = -1;  /* 0x623880 */
int ai_Unk623884 = -1;  /* 0x623884 */
int ai_Unk623888 = -1;  /* 0x623888 */
int ai_Unk62388c = -1;  /* 0x62388c */
int ai_Unk623890 = -1;  /* 0x623890 */
int ai_Unk623894 = -1;  /* 0x623894 */
int ai_Unk623898 = -1;  /* 0x623898 */
int ai_Unk6238a4 = -1;  /* 0x6238a4 */
/* 0x623ba8, the byte the day loop 0x525a00 writes with its first p[-1] (the tail of the preceding table);
   without it the write lands in the last byte of ai_Unk62a8cc in the full link and turns the drag view id
   into 0x06ffffff */
ALIGN1 char sv_Unk623ba8 = 4;
/* 0x623ba9, the day phase times: one 277-byte object (the original packs the int table 1 byte after the
   flag byte, so ai_Unk623baa is a view at +1); the 0x623ba8 byte the day loop writes with p[-1] is the
   preceding object's tail */
ALIGN1 char ai_Unk623ba9[277] = { 0, 64, 56, 0, 0, 0, 0, 0, 0, 6, 0, 96, 84, 0, 0, 0, 0, 0, 0, 12, 0, -64, -88, 0, 0, 0, 0, 0, 0, 16, 0, 0, -31, 0, 0, 0, 0, 0, 0, 18, 0, 32, -3, 0, 0, 0, 0, 0, 0, 19, 0, 48, 11, 1, 0, 0, 0, 5, 67, 0, 0, 3, 67, 0, 0, 127, 67, 0, -128, -103, 67, 0, 0, -18, 66, 0, 0, 15, 67, 0, 0, 90, 67, 0, 0, -81, 67, 0, 0, 85, 67, 0, 0, 52, 67, 0, 0, 32, 67, 0, -128, -69, 67, 0, 0, 127, 67, 0, 0, 78, 67, 0, 0, -10, 66, 0, -128, -1, 67, 0, 0, 127, 67, 0, 0, 57, 67, 0, 0, -84, 66, 0, -128, -1, 67, 0, 0, 34, 67, 0, 0, 1, 67, 0, 0, 69, 67, 0, 0, -93, 67, 0, 0, 5, 67, 0, 0, 3, 67, 0, 0, 127, 67, 0, -128, -103, 67, 0, 0, 5, 67, 0, 0, -32, 66, 0, 0, 127, 67, 0, 0, -20, 67, 0, 0, -34, 66, 0, 0, 13, 67, 0, 0, 53, 67, 0, -128, -30, 67, 0, 0, 59, 67, 0, 0, 83, 67, 0, 0, 26, 67, 0, 0, -71, 67, 0, 0, 127, 67, 0, 0, 66, 67, 0, 0, -22, 66, 0, -128, -1, 67, 0, 0, 127, 67, 0, 0, 47, 67, 0, 0, 104, 66, 0, 0, -49, 67, 0, 0, 76, 67, 0, 0, 64, 67, 0, 0, 53, 67, 0, -128, -47, 67, 0, 0, 5, 67, 0, 0, -32, 66, 0, 0, 127, 67, 0, 0 };
int ai_Unk623cc0[] = { 1104150528, 1097859072, 1116602368, 0, 1102577664, 1107820544, 1113325568, 0, 1116209152, 1112276992, 1110179840, 0, 1120272384, 1117519872, 1109393408, 0, 1116602368, 1113587712, 1108606976, 0, 1107820544, 1102053376, 1112539136, 0, 1104150528, 1097859072, 1116602368, 0, 1122500608, 1124270080, 1132396544, 1133019136, 1121452032, 1125056512, 1128792064, 1133969408, 1123680256, 1127284736, 1126170624, 1134592000, 1124990976, 1128726528, 1130168320, 1135247360, 1126629376, 1127153664, 1126957056, 1135411200, 1124990976, 1124728832, 1128792064, 1135706112, 1122107392, 1124270080, 1132396544, 1132691456, 1124401152, 1121976320, 1132396544, 1134592000, 1122893824, 1125384192, 1125122048, 1138425856, 1123287040, 1125253120, 1125122048, 1136361472, 1125318656, 1127612416, 1128595456, 1135706112, 1124597760, 1124139008, 1125122048, 1135083520, 1125777408, 1128267776, 1129185280, 1134264320, 1124401152, 1121976320, 1132396544, 1134428160 };  /* 0x623cc0 */
int ai_Unk623e10[] = { 1099431936, 1097859072, 1116602368, 0, 1096810496, 1104674816, 1114112000, 0, 1096810496, 1102053376, 1114112000, 0, 1101004800, 1107296256, 1113325568, 0, 1101004800, 1103101952, 1114898432, 0, 1101004800, 1094713344, 1110966272, 0, 1099431936, 1097859072, 1116602368, 0 };  /* 0x623e10 */
int ai_Unk623e80 = -1;  /* 0x623e80 */
int ai_Unk623e88 = 1;  /* 0x623e88 */
int ai_Unk623e8c = -1;  /* 0x623e8c */
float ai_Unk623e94 = 0.5f;  /* 0x623e94 */
int ai_Unk623ea0 = -1;  /* 0x623ea0 */
int ai_Unk623ea4 = -1;  /* 0x623ea4 */
int ai_Unk623ea8 = -1;  /* 0x623ea8 */
int ai_Unk623ec4 = -1;  /* 0x623ec4 */
int ai_Unk623ec8 = -1;  /* 0x623ec8 */
int sv_ViewRectLeft = 0;  /* 0x6248a4 */
int sv_ViewRectTop = 0;  /* 0x6248a8 */
unsigned ai_Unk623ecc = 4294967295;  /* 0x623ecc */
unsigned char ai_Unk623ee8[8] = { 95, 67, 46, 98, 109, 112, 0, 0 };  /* 0x623ee8 */
int sv_ViewRectRight = 694;  /* 0x6248ac */
int sv_ViewRectBottom = 600;  /* 0x6248b0 */
int ai_Unk62a8cc = -1;  /* 0x62a8cc */
int ai_Unk631078 = 0;  /* 0x631078 */
short d2_PointerLook = 0;  /* 0x6ed9d0, the pointer direction / panel layout code (1 .. 8) */
int ai_Unk6f1c40 = 0;  /* 0x6f1c40 */
float ai_Unk6f1c44 = 0;  /* 0x6f1c44 */
int ai_Unk6f1c48 = 0;  /* 0x6f1c48 */
int ai_Unk6f1c50 = 0;  /* 0x6f1c50 */
float ai_Unk6f1c64[3] = {0};  /* 0x6f1c64 */
float ai_Unk6f1c74[3] = {0};  /* 0x6f1c74 */
int ai_Unk6f1c84 = 0;  /* 0x6f1c84 */
int ai_Unk6f1c88 = 0;  /* 0x6f1c88 */
int ai_Unk6f1c8c = 0;  /* 0x6f1c8c */
int ai_Unk6f2290 = 0;  /* 0x6f2290 */
int ai_Unk6f2298 = 0;  /* 0x6f2298 */
int sv_FrameDepth = 0;  /* 0x6f229c, the depth of the frame callback stack */
int sv_PopRequest = 0;  /* 0x6f22a0 */
int sv_FrameBase = 0;  /* 0x6f22a4, the depth a screen was entered at (the base of the pop count) */
int ai_Unk6f22b4 = 0;  /* 0x6f22b4 */
int ai_Unk6f22c0 = 0;  /* 0x6f22c0 */
int ai_Unk6f22c4 = 0;  /* 0x6f22c4 */
int ai_Unk6f22c8 = 0;  /* 0x6f22c8 */
int ai_Unk6f22cc = 0;  /* 0x6f22cc */
int ai_Unk6f22d0 = 0;  /* 0x6f22d0 */
float sv_CameraHeightFactor = 0;  /* 0x6f22d4 */
float sv_CameraAngleFactor = 0;  /* 0x6f22d8 */
int ai_Unk6f2304 = 0;  /* 0x6f2304 */
char * ai_PlayerBuilding = 0;  /* 0x6f2308 */
char * ai_FallbackBuilding = 0;  /* 0x6f230c */
unsigned short * sv_PanelShown = 0;  /* 0x6f2310, the scene record (room) the info panel shows now */
int ai_Unk6f2334 = 0;  /* 0x6f2334 */
int sv_ResearchClass = 0;  /* 0x6f233c */
char ai_Unk6f2340 = 0;  /* 0x6f2340 */
char sv_MessageBoxOpen = 0;  /* 0x6f2350 */
char sv_MessageBoxGreenOpen = 0;  /* 0x6f2351 */
char sv_MessageBoxYesNoOpen = 0;  /* 0x6f2352 (the yes / no box) */
int ai_Unk6f2358 = 0;  /* 0x6f2358 */
int ai_Unk6f235c = 0;  /* 0x6f235c */
int ai_Unk6f2360 = 0;  /* 0x6f2360 */
char ai_Unk6f2368 = 0;  /* 0x6f2368 */
int ai_Unk6f236c = 0;  /* 0x6f236c */
int ai_Unk6f2370 = 0;  /* 0x6f2370 */
float ai_Unk6f2374 = 0;  /* 0x6f2374 */
int ai_Unk6f2378 = 0;  /* 0x6f2378 */
float ai_Unk6f2388 = 0;  /* 0x6f2388 */
float ai_Unk6f238c = 0;  /* 0x6f238c */
float ai_Unk6f2390 = 0;  /* 0x6f2390 */
int ai_Unk6f2394 = 0;  /* 0x6f2394 */
int ai_Unk6f2398 = 0;  /* 0x6f2398 */
int ai_Unk6f239c = 0;  /* 0x6f239c */
int ai_Unk6f23a0 = 0;  /* 0x6f23a0 */
int ai_Unk6f23a4 = 0;  /* 0x6f23a4 */
float ai_Unk6f23a8 = 0;  /* 0x6f23a8 */
float ai_Unk6f23ac = 0;  /* 0x6f23ac */
float ai_Unk6f23b0 = 0;  /* 0x6f23b0 */
float ai_Unk6f23b4 = 0;  /* 0x6f23b4 */
int ai_Unk6f23b8 = 0;  /* 0x6f23b8 */
int ai_GameState = 0;  /* 0x6f25ac */
int ai_OptionScrollSpeed = 0;  /* 0x6f3614 */
int sv_PanelFlag = 0;  /* 0x75e8c0, 1 when the panel is up (an int; cm.h / f3init.c) */
char * sv_PanelObject = 0;  /* 0x75e8c4, the object the info panel shows */
unsigned short * sv_PanelRecord = 0;  /* 0x75e8c8, its scene record */
char sv_PanelText[76] = {0};  /* 0x75e8d4, the panel text */
int sv_SelectionFlags[0xc0] = {0};  /* 0x75e920, the mirror of the character selection flags (byte i per character) */
int ai_Unk75ec20 = 0;  /* 0x75ec20 */
unsigned short * ai_Unk75ec5c = 0;  /* 0x75ec5c */
int ai_Unk75f280[576] = {0};  /* 0x75f280, the member panel: */
int ai_Unk75fb80 = 0;  /* 0x75fb80 */
int ai_Unk75fb84 = 0;  /* 0x75fb84 */
int ai_Unk75fb88 = 0;  /* 0x75fb88 */
int ai_Unk75fb90 = 0;  /* 0x75fb90 */
int ai_Unk75fb94 = 0;  /* 0x75fb94 */
int ai_Unk75fb98 = 0;  /* 0x75fb98 */
float ai_Unk760420 = 0;  /* 0x760420 */
int ai_Unk760440 = 0;  /* 0x760440 */
int ai_Unk760464 = 0;  /* 0x760464 */
int ai_Unk760480 = 0;  /* 0x760480 */
int ai_Unk760484 = 0;  /* 0x760484 */
unsigned ai_Unk760488[0x18] = {0};  /* 0x760488 */
int ai_Unk760548 = 0;  /* 0x760548 */
int ai_Unk76054c = 0;  /* 0x76054c */
float ai_Unk760550[0x18] = {0};  /* 0x760550 */
float ai_Unk7605b0[0x18] = {0};  /* 0x7605b0 */
int ai_Unk760610 = 0;  /* 0x760610 */
int ai_Unk760614 = 0;  /* 0x760614 */
int ai_Unk76061c = 0;  /* 0x76061c */
int ai_Unk760620 = 0;  /* 0x760620 */
int ai_Unk760624 = 0;  /* 0x760624 */
int ai_Unk760628 = 0;  /* 0x760628 */
SVPLATE sv_NamePlates[0x20] = {0};  /* 0x760640, the name plate records (200 bytes each) */
char sv_StatusText[136] = {0};  /* 0x761f40, the status line text */
int sv_BubbleObjects[4286] = {0};  /* 0x761fc8, the object of each speech bubble ( */
int ai_Unk7662c0 = 0;  /* 0x7662c0 */
int ai_Unk7662e0[15] = {0};  /* 0x7662e0, 15 widget ids */
int cm_Bombs[0x40] = {0};  /* 0x935c00, cm's 32 bomb records (object, time), cm.h */
int d2_HitWindow = 0;  /* 0xf8aad0 */
int ai_Unkfb9060 = {0}; /* 0xfb9060 */
int ai_Unkfb9064 = {0}; /* 0xfb9064 */
int ai_Unkfb9068 = {0}; /* 0xfb9068 */
int ai_Unkfb906c = {0}; /* 0xfb906c */
int ai_Unkfba9c0[168] = {0}; /* 0xfba9c0 */
char * ai_Unkfc2dec = {0}; /* 0xfc2dec, the camera object */
