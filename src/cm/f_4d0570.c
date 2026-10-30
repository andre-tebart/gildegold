/* 0x4d0570, the order of the right click for the character c of the player record rec to the picked object
   ai_ClickedObject (type 2: 0x4cffd0; "sp_ESCAPE" 0x4d0200; "sp_CONQUER" 0x4d02d0; "WARE" 0x4d0380) or, without a pick, to
   the ground point under the cursor (kind 1); returns 1 when an order was given. Rough draft. */

#define G19_4D0380 ((int (*)(char *, char *, char *))ai_Unk4d0380)

char ai_Unk4d0570(char *rec, char *c)
{
    char ret = 0;
    char *sel, *o, *q;
    int L[11];
    int x, z;

    if (dm_L.lup != 0) {
        sel = (char *)ai_ClickedObject;
        if (sel != 0) {
            if (*(int *)(sel + 0x220) == 2) {
                o = *(char **)(sel + 0x200);
                if (ai_Unk4ce270(c, o) != 0 || kb_Down[0x36] != 0) {
                    if (ai_Unk4cffd0(rec, c, o, 0, 0) != -1)
                        ret = 1;
                }
            } else if (strcmp(sel, "sp_ESCAPE") == 0) {
                ai_Unk4d0200(rec, c, sel);
                ret = 1;
            } else if (strncmp(sel, "sp_CONQUER", 10) == 0) {
                ai_Unk4d02d0(rec, c, (char *)ai_ClickedObject);
                ret = 1;
            } else if (strncmp((char *)ai_ClickedObject, "WARE", 4) == 0) {
                G19_4D0380(rec, c, (char *)ai_ClickedObject);
                ret = 1;
            }
        } else if ((char)d3sm_PickK(*(char **)(ai_Unk5fb5b0 + 0xb0), dm_L.x + 0x1a, dm_L.y + 0x1a, &x, &z)) {
            q = (char *)ai_Unk4cdf00(rec, c);
            q[0xc] = 0;
            memcpy(L, q, 0x2c);
            L[0] = *(int *)(c + 4);
            L[4] = x;
            L[1] = 1;
            L[5] = z;
            ai_Unk4c5c40(*(int *)(rec + 0x10), L);
            return 1;
        } else
            sv_SetStatusText((char *)ai_Unk143e8b0);
    }
    return ret;
}
