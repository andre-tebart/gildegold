/* 0x599af0, the tavern contact listener "contact_WUERFELSPIEL" / "contact_STAMMTISCH" / "ob_DUNKLE_ECKE"
   (440 B): the pump 0x5309a0 (0x599af0, 0x67fff), the contact list 0x58f510 (param_1) and the three lookups
   0x52e1c0 (0x144038c / 0x14403ec / 0x144046c, class 0x16); the clicked contact 0x6f22e4 opens the dice game
   0x598170 (param_1), the regulars table 0x599240 or the dark corner 0x599aa0 (param_1, param_2); with the gate
   0x6f2450 set and the input key 0x1468410 byte 0x12 (enter), an AIMSG (0x74) goes to 0x4c4b70: type +4, the
   character 0x770b84 + player * 0x21c at +8, -1 at +0xc, the clock 0x770ae0 .. +0x28, the byte 2 at +0x36, the
   buttons of param_1 + 1 at +0x5c and of param_2 + 2 at +0x60; the key 0x17 calls 0x599aa0 */

void sv_ReleaseNamePlates(void); int d3t_PlaceAtObject(); void d2_SetBackgroundTexture(); 
int sv_ClearNamePlates(); struct D3OBJECT * d3if_Pick(float x, float y, int flags, int mask); int gm_GetGoodsValue(); int gm_Unk509e30(void *list, unsigned char p);
int gm_PlayerFactor(int a, unsigned char k); int gm_Unk50a250(); int gm_Unk50a150(); void d2_EnableViewObjects();
void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2); void he_Unk53e1f0(); void d2_RadioClear(int g); int d2_CreateRadioGroup(int n, ...);
int ai_PersonnelBook(); int ai_StorageWindow(); void ai_MasterLetterWindow(); int ai_OpenTransport();
int ai_Unk5b5a20(); int ai_Unk5b5690(); int ai_AmountRowStatus(); int ai_AmountRow();
void ai_Unk5bd1f0(); void cm_BeginGroup(const char *s); void ai_Unk4c5d40(void *p); void cm_SendProofExecuted(int a, int b);

void ai_Unk599af0(int param_1, int param_2)
{
    char msg[0xf8];
    int p1;
    int p2;
    int p3;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk599af0, 0x67fff) != 0) {
        ai_MarketContact((AIBLD *)param_1);
        p1 = sv_RegisterNamePlate("contact_WUERFELSPIEL", U_I((unsigned)&full_data_144038c), 0x16);
        p2 = sv_RegisterNamePlate("contact_STAMMTISCH", U_I((unsigned)&full_data_14403ec), 0x16);
        p3 = sv_RegisterNamePlate("ob_DUNKLE_ECKE", U_I((unsigned)&full_data_144046c), 0x16);
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == p1)
                ai_Unk598170(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == p2)
                ai_Unk599240(param_1, param_2);
            else if (U_I((unsigned)&ai_ClickedObject) == p3)
                ai_Unk599aa0(param_1, param_2);
        }
        if (U_I((unsigned)&full_data_6f2450) != 0) {
            if (*(unsigned char *)(unsigned)&kb_Last == 0x12) {
                memset(msg, 0, 0xf8);
                *(unsigned char *)(msg + 4) = 0x74;
                *(unsigned char *)(msg + 0x36) = 2;
                *(int *)(msg + 8) = U_I((unsigned)&ai_CharInts + U_W((unsigned)&ai_PlayerId) * 0x87);
                *(int *)(msg + 0xc) = -1;
                *(int *)(msg + 0x28) = U_I((unsigned)&ai_Time);
                *(int *)(msg + 0x2c) = U_I((unsigned)&ai_TimeHour);
                *(int *)(msg + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                *(unsigned short *)(msg + 0x34) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
                *(int *)(msg + 0x5c) = U_I(param_1 + 1);
                *(int *)(msg + 0x60) = U_I(param_2 + 2);
                cm_SendLongMessage(msg);
            } else if (*(unsigned char *)(unsigned)&kb_Last == 0x17) {
                ai_Unk599aa0(param_1, param_2);
            }
        }
    }
}
