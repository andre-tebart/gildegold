
char ai_Chars[768][0x21c];  /* 0x770b80, the characters */
char * ai_BTypes;  /* 0x7e3f30 */
signed char ai_Relations[768][768];  /* 0x7e5360 */
signed char ai_RelationMemory[768][768];  /* 0x889f80 */
int ahm_Tab[8] = { 1, 3, 7, 11, 13, 17, 19, 21 };  /* 0x60f1b0 */
/* ai_Unk144a7a8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk144a80c: view of txt_Ptrs (build/mk_txt_views.py) */
int ai_WalkX = 0;  /* 0x6f0dc8 */
int ai_WalkDX = 0;  /* 0x6f0dcc */
int ai_WalkY = 0;  /* 0x6f0dd0 */
int ai_WalkDY = 0;  /* 0x6f0dd4 */
int ai_WalkIndex = 0;  /* 0x6f0dd8 */
AHMSEED ai_WalkSeed = {0};  /* 0x6f0ddc */
AHMSTEP ai_WalkSteps[3] = {0};  /* 0x6f0de0 */
int ai_PlayerPrototypeCount = 0;  /* 0x6f0e28 */
AHMGRID * ai_PlayerPrototypes = 0;  /* 0x6f0e2c */
int ai_RecipePrototypeCount = 0;  /* 0x6f0e30 */
AHMGRID * ai_RecipePrototypes = 0;  /* 0x6f0e34 */
unsigned char ai_OwnPlayer;  /* 0x6f1bc5 */
