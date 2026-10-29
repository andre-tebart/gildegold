/* 0x587c90, the graveyard idle-chatter tick (329 B, the argument is not used): only every 0x1194 units of the
   clock 0x6333cc (0x62a538 the last run); with U_I (0x6f2448) == 0 and 0x42ec20 () < 0.9f no chatter. The scene's
   0x20 object slots 0x7603a0 .. 0x760420: the objects of the kind 0x14 (byte +2) whose building [obj + 0x184]
   is 0x5fb5b0 (+0x88) are collected; none -> return; one (x) and, with more than one, a second different one (y)
   are picked at random: a = (x + 4) % 3, or 3 + ((x + 4) & 1) when x + 9 == 1; b = -1, or the same for y; the
   script "locations\friedhof\gespraeche.esc" (0x62a8a8) -> 0x4287b0 and 0x429ab0 (the script, 3,
   x's building + 0x184, a, b) */

void ai_Unk587c90(char *unused)
{
    int list[0x20];
    int cnt;
    int n;
    int a;
    int b;
    char *x;
    char *y;
    char *p;
    int i;
    int s;

    if (U_I((unsigned)&ai_Unk6333cc) < U_I((unsigned)&full_data_62a538) + 0x1194)
        return;
    U_I((unsigned)&full_data_62a538) = U_I((unsigned)&ai_Unk6333cc);
    if (U_I((unsigned)&ai_Unk6f2448) == 0 && util_RandomF() < 0.9f)
        return;
    cnt = 0;
    for (i = 0; i < 0x20; i++) {
        p = *(char **)((unsigned)&ai_Unk7603a0 + i * 4);
        if (p != 0 && *(unsigned char *)(p + 2) == 0x14 && *(int *)(p + 0x184) != 0 &&
            *(int *)(*(int *)(p + 0x184) + 0x88) == U_I((unsigned)&ai_Unk5fb5b0))
            list[cnt++] = (int)p;
    }
    if (cnt == 0)
        return;
    n = util_Random(cnt) & 0xffff;
    x = (char *)list[n];
    y = 0;
    if (cnt > 1) {
        n = util_Random(cnt) & 0xffff;
        y = (char *)list[n];
        if (y == x) {
            n++;
            y = (char *)list[n % cnt];
        }
    }
    a = 0;
    if (x != 0) {
        if (*(unsigned char *)(x + 9) == 1)
            a = (*(int *)(x + 4) & 1) + 3;
        else
            a = *(int *)(x + 4) % 3;
    }
    b = -1;
    if (y != 0) {
        if (*(unsigned char *)(y + 9) == 1)
            b = (*(int *)(y + 4) & 1) + 3;
        else
            b = *(int *)(y + 4) % 3;
    }
    s = (int)evt_Load((char *)(unsigned)&full_data_62a8a8);
    if (s != 0)
        evt_RunScriptArgs((EVTSCRIPT *)s, 3, *(int *)(x + 0x184), a, b);
}
