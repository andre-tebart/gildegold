/* 0x570ea0, 1 when no running event 0x88 has the player +8 == b + 4 and the building +0xb0 == a + 4; 0 when one does */

int ai_Unk4a5d30(struct AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id); /* 0x4a5d30 */
int unk_Unk4f24a0();                 /* 0x4f24a0 */
int gm_GetWorth(unsigned short id);                 /* 0x50a640 */
int gm_ConvertMoney(int a, unsigned char k); /* 0x5085b0 */
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); /* 0x4c4830 */
void cm_SendSetField(int a, int b, int c, int d, int e); /* 0x4c4a60 */
void he_Unk570bf0(int *a, int b);   /* 0x570bf0 */
void he_Unk570c70(int *a, int b);   /* 0x570c70 */
int he_Unk53d6f0();                 /* 0x53d6f0 */
int gm_ClockSeason();                 /* 0x4fe350 */
int gm_BuildingSkill();                 /* 0x5092e0 */
int sv_UsedGroupSlots();                 /* 0x5241b0 */
int gm_FreeCapacity();                 /* 0x50ab80 */
char *gm_FindBuildingById(int id); /* 0x502020 */
int unk_Unk4ef570(char *r, int i, unsigned char n);                 /* 0x4ef570 */
int unk_Unk4f0c20();                 /* 0x4f0c20 */
int gm_Unk509ee0(char *c, char p);                 /* 0x509ee0 */
int ai_Unk5a9910();                 /* 0x5a9910 */

int he_Unk570ea0(int a, int b)
{
    int r;

    for (r = (int)he_FindEvent(1, 0, 0x88); r != 0; r = (int)he_NextEvent()) {
        if (*(unsigned int *)(b + 4) == *(unsigned short *)(r + 8) && HM_I(r, 0xb0) == HM_I(a, 4))
            return 0;
    }
    return 1;
}
