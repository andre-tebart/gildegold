/* 0x5728d0, the exam result: the building 0x501190 (1, 1, +0xac) of the kind 0x1d (0x7e3f30) and the character +0xc: 0x4f0700 (character, building, +0xb0, +0xb4, +0xbc, +0xb8); when the character is not the building's owner record and its kind is 6 / 7: 0x5a97e0 (the owner record, character, building, +0xb0, +0xb8, (int)(+0xb8 * 0x5ee034[+0xb0])) */

void ai_Unk5a97e0(int param_1, unsigned short *param_2, int param_3, int param_4, int param_5, int param_6);                 /* 0x5a97e0 */
void ai_Unk5a9880(int param_1, unsigned short *param_2, int param_3, int param_4);                 /* 0x5a9880 */
int unk_Unk4f0700(int *g, char *o, int v, int b, short s, int x);                 /* 0x4f0700 */
int unk_Unk4f0900();                 /* 0x4f0900 */
void he_Unk5728d0(int ev);          /* 0x5728d0 */
void he_Unk5729c0(int ev);          /* 0x5729c0 */

void he_Unk5728d0(int ev)
{
    char *bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0xac));
    char *per, *own;
    int n;

    if (bld == 0 || *(char *)((unsigned)&ai_BTypes + *(unsigned char *)bld * 0x24d) != 0x1d)
        return;
    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    if (per == 0)
        return;
    unk_Unk4f0700(per, bld, HM_I(ev, 0xb0), HM_B(ev, 0xb4), *(unsigned short *)(ev + 0xbc), HM_I(ev, 0xb8));
    own = (char *)((unsigned)&ai_Chars + *(unsigned short *)(bld + 0x27) * 0x21c);
    if (per == own)
        return;
    if (HM_B(own, 2) != 6 && HM_B(own, 2) != 7)
        return;
    n = (int)((float)HM_I(ev, 0xb8) * *(float *)((unsigned)&unk_Tab5ee034 + HM_I(ev, 0xb0) * 4));
    ai_Unk5a97e0(own, per, bld, HM_I(ev, 0xb0), HM_I(ev, 0xb8), n);
}
