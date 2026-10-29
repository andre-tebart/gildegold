/* 0x512e80, the scene callback (a function start Ghidra missed) for the objects of a building b whose state is not 2 / 3 / 4 and whose name does not start with "pfl_" / "vg_" / "!vg_": registers (0x446f30) the object with its list for every slot 0 .. (the building kind's rank byte (+0x247) - 1, + 1 for the type 2) */


char init_BuildingTextureSetHook(int unused, unsigned char *b, int o)
{
    char *p;
    int n, i;
    int st;

    p = ai_BTypes + (unsigned)*b * 0x24d;
    n = (unsigned char)p[0x247] - 1;
    if (*p == 2)
        n++;
    st = *(int *)(o + 0x220);
    if (st != 2 && st != 4 && st != 3) {
        if (strncmp((char *)o, "pfl_", 4) != 0 && strncmp((char *)o, "vg_", 3) != 0 && strncmp((char *)o, "!vg_", 4) != 0) {
            i = 0;
            if (n >= 0) {
                do {
                    d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), i, 1);
                    i++;
                } while (i <= n);
            }
        }
    }
    return 1;
}
