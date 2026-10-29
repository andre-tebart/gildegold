
AICHAR * ai_Partner = 0;  /* 0x6f0468 */
int ai_TradeMsgCount = 0;  /* 0x6f046c, the trade messages sent since the last reset */
float ai_Rating = 0;  /* 0x6f1ba0, the smoothed game rating (statistics out[5]; the running rating ai_RatingBase follows it) */
AIBLD * ai_MarketBuilding = 0;  /* 0x6f1bc8, the market building (gm_BuildQuery(1, 5, 10)); @original: f3snd.h declares G16BLDVIEW * (f3snd.c reads its + */
unsigned short * ai_PlayerChar = 0;  /* 0x6f1bf8 */
int ai_CurrentPlayer = 0;  /* 0x6f2590, the player index whose turn the local round loop simulates */
AIGRID ai_Grid[8][8] = {0}; /* 0x91ae60 */
/* ai_ActionFlags: view of ai_Unkf83ec0 (tools/data_views.py) */
int ai_TradeMsgOff = 0;  /* 0xf86a24, read-only in every source: nonzero suppresses the trade
                                           message and its count (never set in this build) */
int ai_NSlots = 0;  /* 0xf86a28 */
int ai_NEntries = 0;  /* 0xf86a2c */
AISLOT ai_Slots[128] = {0};  /* 0xf86a40 */
AIENTRY ai_Entries[32] = {0};  /* 0xf88a40 */
AIOBJ * ai_Carts[16] = {0};  /* 0xf89540, the master's kind- */
int ai_UpdateStart = 0;  /* 0xf89580, the character index the per-frame update share starts at */
AITIME ai_LastUpdateTime = {0};  /* 0xf89584, the game time of the last per-frame AI update */
float ai_CountCarry = 0;  /* 0xf89594, the fractional character count carried to the next frame */
int ai_WeaponList2[10] = {0};  /* 0xf8a0a0, goods ids of the second weapon list (0-terminated; ends before */
AICHAR * ai_LoggedMaster = 0;  /* 0xf8a0c8, the master whose decisions are logged */
int ai_NumCarts = 0;  /* 0xf8a0cc, how many of ai_Carts are set (0 .. 16) */
int ai_WeaponList[10] = {0};  /* 0xf8a0e0, goods ids of the first weapon list (0-terminated) */
int ai_NetPlayer = 0;  /* 0x1460af4, the player index this client runs (the host sets it, cm.c message type 7) */
const unsigned int ai_BuildingTypeMasks[4] = { 2692498336, 1, 3766305712, 1 };  /* 0x5eed70, bit masks of building types (two 64 bit masks) */
float ai_DifficultyValue = 1.0f;  /* 0x5fef04 */
float ai_DifficultyScale = 0.5f;  /* 0x5fef08 */
char ai_AmbushSpots[8][32] = { "sp_AUFLAUERLEGEN_NORDEN", "sp_AUFLAUERLEGEN_OSTEN", "sp_AUFLAUERLEGEN_SUEDEN", "sp_AUFLAUERLEGEN_WESTEN", "sp_AUFLAUERLEGEN_STADT_1", "sp_AUFLAUERLEGEN_STADT_2", "sp_AUFLAUERLEGEN_STADT_3", "sp_AUFLAUERLEGEN_STADT_4" };  /* 0x625720, "sp_AUFLAUERLEGEN_NORDEN" ... */
