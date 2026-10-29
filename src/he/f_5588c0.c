/* 0x5588c0, the allocation handler of the sabotage event (ROUGH): the saboteur +0xb0 (0x505df0; gone or with a job of the kind 0x16 at +0x17c: free) and the building +0x10 are looked up (+0xb4 optionally the target figure 0x4ffe90 (.., 2, 6, 1, id)); the start / due clocks are now, the saboteur is stored for the group (0x524260 (building, target, id, ev)), +0x60 = now, posted 0x1e later, +0xb8 = the due clock + 6 units, and the word +0xbc is set to 0x1d + (+0xbc - the minute word) when it is below 6 */

void ai_Unk4c59f0(unsigned char a, int b, int c, unsigned char d, unsigned char e, unsigned char f, const char *s);
int gm_Unk509400();
void gm_GetGebaeudeHauptraum(unsigned char *b);

void he_Unk5588c0(int ev)
{
    int p, b, tgt = 0;

    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0 || (*(int *)(p + 0x17c) != 0 && *(unsigned char *)*(int *)(p + 0x17c) == 0x16)) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(int *)(ev + 0xb4) != -1)
        tgt = gm_Query(*(int *)(b + 0x5d), 2, 6, 1, *(int *)(ev + 0xb4));
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0x44) = ai_Time;
    sv_MoveToGroup(b, tgt, *(unsigned short *)p, ev);
    *(HETIME *)(ev + 0x60) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0x1e, 0);
    *(HETIME *)(ev + 0xb8) = *(HETIME *)(ev + 0x52);
    gm_AddTime(ev + 0xb8, 6, 0, 0);
    if (*(unsigned short *)(ev + 0xbc) < 6)
        *(unsigned short *)(ev + 0xbc) = (unsigned short)(*(unsigned short *)(ev + 0xbc) - *(unsigned short *)(ev + 0x56) + 0x1d);
}
