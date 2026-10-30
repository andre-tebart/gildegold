/* 0x502a60, for the building object o of the player in the building b with the character c at the scene 0x6f2310: the five slots of the character (bytes +0x80 .. of the record 0x770c00 + c * 0x21c) give the numbers 0x17f + (the scaled value) + 6k, 0 for an empty slot; the scene's objects (0x438d40 with the callback 0x5029f0 into a 0x40-entry list) whose key matches one of them are switched on (0x447300 (obj, 1)), the others off. ROUGH */

void gm_Unk502a60(int unused, int scene, int c)
{
    int want[5];
    int list[0x40 * 2 + 1];
    int i, k, n, v;
    char *s;

    if (sv_PanelShownI == scene) {
        k = 0;
        s = ai_Chars[c] + 0x80;
        i = 0;
        do {
            v = (int)(float)0;
            v = v + 0x17f + k;
            if (*s == 0)
                v = 0;
            want[i] = v;
            k += 6;
            s++;
            i++;
        } while (k < 0x1e);
        memset(list, 0, sizeof(list));
        d3_ForEachObject(ai_Unk5fb5b0, 0, 0xc0, gm_Unk5029f0, list);
        n = list[0x40 * 2];
        for (i = 0; i < n; i++) {
            d3if_ShowTree(list[i], 0);
            for (k = 0; k < 5; k++)
                if (list[i + 0x40] == want[k])
                    d3if_ShowTree(list[i], 1);
        }
    }
}
