/* 0x53f430, for every figure of kind 5 / 0x16 (inn) checks the four staff slots of its record; a slot whose figure is gone or dead gets the script message "veal" ("stammtisch supervisor") */

void he_Unk53f430(void)
{
    int f, r, k, o;
    int *p;
    int arg[3];

    f = gm_BuildQuery(1, 5, 0x16);
    while (f != 0) {
        r = gm_Query(*(int *)(f + 0x5d), 1, 0, 0x120);
        if (r != 0 && (r = gm_Query(*(int *)(r + 0x14), 1, 0, 0x12d)) != 0) {
            p = (int *)(r + 0x1c);
            for (k = 4; k != 0; k--) {
                if (*p != -1 && ((o = gm_FindChar(*p)) == 0 || *(char *)(o + 8) == 0 || *(char *)(o + 2) == 0xf)) {
                    arg[2] = *(int *)(f + 1);
                    arg[1] = *p;
                    arg[0] = 0x6c656176;
                    cm_BeginGroup("stammtisch supervisor");
                    ai_Unk4c5d80(arg);
                    cm_EndGroup("stammtisch supervisor");
                }
                p++;
            }
        }
        f = gm_BuildNext();
    }
}
