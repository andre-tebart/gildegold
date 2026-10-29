/* 0x55a5f0, the allocation handler of the brawl event "PRUEGEL" (ROUGH): due now; with no building (+0x10 = -1) the first of the kind 0x16 (else 0xf) is taken, the message 0x4c59f0 (0x11, +0x10, -1, 0, 2, 0x154, "PRUEGEL") is created (+0xc8 = -1, handle +0xcc), and the target building +0xd0 is the opponent's own (+0x170) or its first of the type (0x501190 (1, 4, id)), else a free spot near it (0x4a63f0 (player, .., 100.0)) */

int ai_Unk494d90();
int gm_EquipmentValue(unsigned char *b, int unused);
int gm_Unk509e30(void *list, unsigned char p);
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);

void he_Unk55a5f0(int ev)
{
    int p, b, own, spot;
    float v;

    p = gm_FindChar(*(int *)(ev + 0xb4));
    *(HETIME *)(ev + 0x52) = ai_Time;
    if (*(int *)(ev + 0x10) == -1) {
        b = gm_BuildQuery(1, 5, 0x16);
        if (b == 0)
            b = gm_BuildQuery(1, 5, 0xf);
        if (b != 0)
            *(int *)(ev + 0x10) = *(int *)(b + 1);
    }
    *(int *)(ev + 0xc8) = -1;
    *(int *)(ev + 0xcc) = ((int (*)(int, int, int, int, int, int, const char *))ai_Unk4c59f0)(0x11, *(int *)(ev + 0x10), -1, 0, 2, 0x154, "PRUEGEL");
    p = gm_FindChar(*(int *)(ev + 0xb4));
    own = *(int *)(p + 0x170);
    if (own == 0)
        own = gm_BuildQuery(1, 4, *(unsigned short *)p);
    if (own != 0) {
        *(int *)(ev + 0xd0) = *(int *)(own + 1);
        return;
    }
    spot = 0;
    if (AI_4A63F0_7((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, 6, *(int *)((unsigned)&ai_Unk5eeab8 + 0x2b0), *(int *)((unsigned)&ai_Unk5eeab8 + 0x2b4), spot, 100.0f, &v) != 0)
        *(int *)(ev + 0xd0) = *(int *)(gm_BuildQuery(1, 1, (int)v) + 1);
    else
        *(int *)(ev + 0xd0) = *(int *)(own + 1);
}
