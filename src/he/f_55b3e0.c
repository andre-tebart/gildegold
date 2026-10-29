/* 0x55b3e0, the allocation handler of a building event (ROUGH): the building +0xac (none: free) is the event place (+0x10), its figure gets the message 0x5a / 2 / 4, both clocks are now, the due clock + 0xa units, and the end clock +0xb8 = now + (the owner rank factor 0x5eb7b8 / 0x5ee020 / 0x5eb828 / 1.0 by the rank byte +0xd (>= 9 / 8 / 7) * +0xb0 * 0x5eb990) units */

int gm_Unk50cad0();
void ai_Unk4c54e0(int a);
void ai_Unk4c5510(int a);
int ai_Unk4fdcb0();

void he_Unk55b3e0(int ev)
{
    int b, rank;
    float f;

    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    *(int *)(ev + 0x10) = *(int *)(b + 1);
    cm_SendSetField(*(int *)(b + 1), 0x5a, 2, 4, 0);
    *(HETIME *)(ev + 0x60) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0xa, 0);
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(HETIME *)(ev + 0xb8) = *(HETIME *)(ev + 0x52);
    rank = *(unsigned char *)((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c + 0xd);
    if (rank >= 9)
        f = (0.400000006f);
    else if (rank == 8)
        f = *(float *)(unsigned)&full_rdata_5ee020;
    else if (rank == 7)
        f = (0.800000012f);
    else
        f = 1.0f;
    gm_AddTime(ev + 0xb8, 0, (int)(f * *(int *)(ev + 0xb0) * *(double *)(unsigned)&ai_Unk5eb990), 0);
}
