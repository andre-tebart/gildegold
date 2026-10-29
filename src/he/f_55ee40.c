/* 0x55ee40, a Ghidra start: the chance of a figure to be seen at its place (ROUGH): for a figure with the flag 1 of +0x5a and objects (+0x61 / the target's +0x170 +0x61) the 3D distance of the two objects scales a factor against 0x5eb780 (1.0), 0x5edbb8 and the building value (0x5092e0 (.., 9, 1)); a nearby contact (0x4ffe90 (.., 1, 0, 0x120)) with an object returns 1, else 0 */

void gm_SetTime(struct GCLOCK *c, unsigned char hour, unsigned min, unsigned sec);
void ai_Unk4c5150(int a, int b, int c, const char *s);
void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s);
void cm_SendGoodsDelta(char *p, unsigned char k);
float ai_Unk4ffa10();
int gm_ListAmount();
int gm_Unk509ee0(char *c, char p);
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);
void gm_GetGebaeudeHauptraum(unsigned char *b);

int he_Unk55ee40(int p)
{
    int o1, o2, c;
    float dx, dy, dz, d;

    if (p == 0 || (*(unsigned char *)(p + 0x5a) & 1) == 0)
        return 0;
    o1 = *(int *)(p + 0x61);
    if (o1 == 0 || *(int *)(p + 0x170) == 0)
        return 0;
    o2 = *(int *)(*(int *)(p + 0x170) + 0x61);
    if (o2 == 0)
        return 0;
    dx = *(float *)(o1 + 0x4c) - *(float *)(o2 + 0x4c);
    dy = *(float *)(o1 + 0x50) - *(float *)(o2 + 0x50);
    dz = *(float *)(o1 + 0x54) - *(float *)(o2 + 0x54);
    d = (float)sqrt(dx * dx + dy * dy + dz * dz);
    gm_BuildingSkill(p, 9, 1);
    if (d > 1.0f)
        return 0;
    c = gm_Query(*(int *)(p + 0x5d), 1, 0, 0x120);
    return c != 0 && *(int *)(c + 0x61) != 0;
}
