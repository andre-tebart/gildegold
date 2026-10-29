/* 0x4cf9c0, the portrait boxes ai_Unk930400[6] of 0x20 bytes (+0 scene, +4 / +8 window objects, +0x10 text, +0x14 picture,
   +0x18 second text, +0x1c the character): shown for the selected characters, with the load (x 100 as "%1N1 %i%"), the
   picture of the job and its count. Rough draft. */

#define G19_E(e, o) (*(int *)((e) + (o)))
#define G19_5176D0 ((void (*)(char *, char *, ...))txt_Format)

void ai_Unk4cf9c0()
{
    char buf[0x100];
    char *e, *c, *o, *p;
    int t;

    if (d2_ClickedId != -1) {
        for (e = ai_Unk930400; e < ai_Unk930400 + 0xc0; e += 0x20) {
            if (G19_E(e, 8) != -1 && ai_Unk5fe710 == G19_E(e, 0)) {
                if (*(char **)(d2_Objs + G19_E(e, 0) * 0x2ec + 0x28) != 0)
                    *(char *)(*(char **)(e + 0x1c) + 0x188) = 1;
                else
                    *(char *)(*(char **)(e + 0x1c) + 0x188) = 0;
            }
        }
    }
    d2_ClearPointerItems();
    ai_Unk6f0d08 = 0;
    for (e = ai_Unk930400; e < ai_Unk930400 + 0xc0; e += 0x20) {
        if (G19_E(e, 8) == -1)
            continue;
        c = *(char **)(e + 0x1c);
        if (*(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) == 1) {
            d2_EnableObject(*(int *)(ai_Unkf8f39c - 0x10 + G19_E(e, 8) * 0x3c4), 0);
            d2_EnableObject(*(int *)(ai_Unkf8f39c - 0x10 + G19_E(e, 4) * 0x3c4), 0);
            continue;
        }
        if (c[0x188] != 0) {
            d2_AddPointerItem(*(int *)(c + 0x18c), 1);
            d2_SetObjValue(G19_E(e, 0), 1);
            ai_Unk6f0d08++;
        } else
            d2_SetObjValue(G19_E(e, 0), 0);
        t = (int)(unk_Unk4fc8e0(c) * 100.0);
        G19_5176D0(buf, "%1N1 %i%", *(unsigned short *)c, t);
        d2_SetObjText(G19_E(e, 0x10), buf);
        o = ai_Unk4cda00(c);
        if (o == 0 || *(int *)o == 0)
            continue;
        if (G19_E(e, 0x14) != -1) {
            if (*(int *)(d2_Objs + G19_E(e, 0x14) * 0x2ec + 8) != *(int *)o + 0xdf) {
                if (G19_E(e, 0x14) != -1)
                    d2_RemoveObj(G19_E(e, 0x14));
                d2_LastWindow = G19_E(e, 8);
                G19_E(e, 0x14) = d2_WinOpenPic(0x54, 0xf, *(int *)o + 0xdf, G19_E(e, 8));
                d2_ScaleObject(G19_E(e, 0x14), -2);
            }
        } else {
            d2_LastWindow = G19_E(e, 8);
            G19_E(e, 0x14) = d2_WinOpenPic(0x54, 0xf, *(int *)o + 0xdf, G19_E(e, 8));
            d2_ScaleObject(G19_E(e, 0x14), -2);
        }
        if (G19_E(e, 0x18) == -1)
            continue;
        if (*(int *)(o + 0x58) == 1 || *(int *)(o + 0x58) == 2) {
            p = ai_Unk4cdac0(c);
            if (p != 0) {
                sprintf(buf, "%i", *(int *)(p + 0x20));
                d2_SetObjText(G19_E(e, 0x18), buf);
                continue;
            }
        }
        if (G19_E(e, 0x18) != -1)
            d2_SetObjText(G19_E(e, 0x18), &ch_EmptyName);
    }
}
