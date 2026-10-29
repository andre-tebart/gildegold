/* 0x591170, the jail / building-question helper (323 B): in the '2' state or when the two window ids match
   (0x5fe710 against 0x623838 / 0x62383c), the record 0x5b2180 (1, the 0x50-byte message local zeroed with
   +0x13 = 0x71f, 0, 0, 0) is resolved: the pending contact 0x6f230c (when set) gets 0x521960 (text 0xfd / -1);
   else 0x502520 and the 'G' kind get 0x521960 (0xfd), the other kinds 0x527840 (the plain open) or 0x521960
   (the record's +0x29 text) */

int ai_Unk5b2180(); int gm_IsHomeType(); int sv_SelectPanelObject(); int sv_Unk527840(); void sv_Unk527bb0();
int unk_Unk503530(); void ai_Unk59f410(); int init_ShowBuildingScene(); int init_FinishBuildingScene();
int d3t_PlaceObjectV(struct D3OBJECT *camera, struct D3VECTOR *pos, struct D3VECTOR *dir, int unk, char flags); int gm_ClockSeason(); int unk_Unk513740(); void d3fl_MakeDivide();
int ai_Unk5bc110(); void snd_StartMarketAmbience(void); int tb_SetFunctionOff(); void snd_StopMarketAmbience(void);
void d2_CopyCaption(char *caption); void sv_SetStatusText(char *s); int sv_Unk52adb0();
int sv_Unk528490(); int ai_Unk576340();
int gm_IsType10(unsigned char *b); void ai_Unk59e420(); void ai_Unk59ee40(); void ai_Unk59a800();
int gm_Unk50c340(); int d2_GetWinObj(); int sv_SetResearchClass(); int ai_Unk5b7dd0();
float gm_ItemPrice(int item, unsigned char n); int ai_Unk5049e0(); int sv_CheckStock(); int sv_CheckGoodAmount();
void cm_BeginGroup(const char *s); void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); void cm_SendMoveGoods(int a, int b, unsigned char c, int d); short * gm_PlayerRecord(char *c);
int cm_SendLongMessage(char *msg); void cm_EndGroup(const char *s); int cm_GetSlotState(); void cm_SendMoneyDelta(int a, int b);
char *_strupr(char *); int ai_Unk50b9f0(); int gm_Unk50c4d0(); int ai_Unk5ac110();
int ai_Unk5bb350(); void ai_MasterLetterWindow(); int ai_PersonnelBook(); void sv_ReleaseNamePlates(void);

void ai_GameFrame(void);
void ai_Unk591ad0(void);

void ai_Unk591170(void)
{
    int msg[0x14];
    char *p;
    int r;
    int u;

    if (U_I((unsigned)&kb_Last) == '2' ||
        (U_I(((unsigned)&dm_L + 0x18)) != 0 &&
         (U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&ai_Unk623838) || U_I((unsigned)&ai_Unk5fe710) == U_I((unsigned)&ai_Unk62383c)))) {
        memset(msg, 0, 0x50);
        msg[4] = 0;
        msg[2] = 0;
        msg[0x13] = 0x71f;
        p = (char *)ai_Unk5b2180(1, msg, 0, 0, 0);
        if (p != 0 && *(char *)(unsigned)&full_data_6f2338 == 0) {
            if (U_I((unsigned)&ai_FallbackBuilding) != 0) {
                r = gm_IsHomeType(p);
                if (r != 0 && *p == 'G') {
                    sv_SelectPanelObject(p, 0xfd, (void *)(unsigned)&ch_EmptyName);
                    return;
                }
                sv_SelectPanelObject(p, -1, (void *)(unsigned)&ch_EmptyName);
                return;
            }
            r = gm_IsHomeType(p);
            if (r == 0 || *p != 'G') {
                if (U_I((unsigned)&ai_FallbackBuilding) == 0 && U_I((unsigned)&ai_PlayerBuilding) == 0) {
                    sv_Unk527840(p);
                    return;
                }
                sv_SelectPanelObject(p, *(unsigned short *)(p + 0x29), (void *)(unsigned)&ch_EmptyName);
            } else {
                u = unk_Unk503530(U_W((unsigned)&ai_PlayerId), p, 0, 0);
                if (u & 1) {
                    sv_SelectPanelObject(p, 0xfd, (void *)(unsigned)&ch_EmptyName);
                    return;
                }
            }
        }
    }
}
