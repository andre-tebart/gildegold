/* 0x5aa4b0, frees the controls of a Domizil record (106 B): +0x1c and +0x10, then the six pairs at +0x40
   (+0xa .. +0xf and +0x10 .. +0x15), each set to -1 */

void d2_RemoveObj(); int gm_Unk509060(); int unk_Unk4fb770(); int gm_Unk50a0f0(int y);
void d2_SetObjFont(); void d2_SetCaption(); void sv_SelectPanelObject(unsigned char *o, int b, char *c);

void ai_Unk5aa4b0(int param_1)
{
    int *p;
    int i;

    if (*(int *)(param_1 + 0x1c) != -1) {
        d2_RemoveObj(*(int *)(param_1 + 0x1c));
        *(int *)(param_1 + 0x1c) = -1;
    }
    if (*(int *)(param_1 + 0x10) != -1) {
        d2_RemoveObj(*(int *)(param_1 + 0x10));
        *(int *)(param_1 + 0x10) = -1;
    }
    for (p = (int *)(param_1 + 0x40), i = 6; i != 0; i--) {
        if (p[-6] != -1) {
            d2_RemoveObj(p[-6]);
            p[-6] = -1;
        }
        if (*p != -1) {
            d2_RemoveObj(*p);
            *p = -1;
        }
        p++;
    }
}
