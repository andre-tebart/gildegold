/* 0x4cfc10, builds the portrait boxes of ai_Unk930400[6] (see 0x4cf9c0) for the selected characters ai_Unk7603a0[]:
   window objects, the picture of the job, the texts, 0x80 pixels apart. Rough draft. */

#define G19_R(id, o, t) (*(t *)(d2_Objs + (id) * 0x2ec + (o)))

void ai_Unk4cfc10()
{
    char *e, *o, **pp;
    int x = 0, t;

    for (e = ai_Unk930400; e < ai_Unk930400 + 0xc0; e += 0x20) {
        G19_E(e, 8) = -1;
        G19_E(e, 0) = -1;
        G19_E(e, 0xc) = -1;
        G19_E(e, 0x10) = -1;
        G19_E(e, 0x18) = -1;
        G19_E(e, 0x14) = -1;
    }
    e = ai_Unk930400;
    for (pp = ai_Unk7603a0; e < ai_Unk930400 + 0xc0 && (int)pp < ((unsigned)&ai_Unk7603a0 + 0x7c); pp++) {
        if (*pp == 0)
            continue;
        if (!(ai_Unk4ce170(*(unsigned short *)ai_Unk6f0d74, *pp) & 0x800))
            continue;
        G19_E(e, 4) = d2_CreatePicObj(x + 0x18, 0xf, 0x5da);
        G19_E(e, 8) = d2_CreateWindow(x, 0, 0x80, 0x4b, 0x110);
        o = ai_Unk4cda50(*pp);
        if (o != 0 && *(int *)o != 0) {
            G19_E(e, 0x14) = d2_WinOpenPic(0x54, 0xf, *(int *)o + 0xdf, d2_LastWindow);
            d2_ScaleObject(G19_E(e, 0x14), -2);
            G19_E(e, 0x18) = d2_CreateWinText(0x54, 0x26, &ch_EmptyName, d2_LastWindow);
            G19_R(G19_E(e, 0x18), 0x74, short) = 0x54;
            G19_R(G19_E(e, 0x18), 0x14, short) = 0x18;
            G19_R(G19_E(e, 0x18), 0x5c, int) = 1;
        }
        G19_E(e, 0x1c) = (int)*pp;
        *(short *)(ai_Unkf8f39c - 0x268 + G19_E(e, 8) * 0x3c4) = 0;
        *(short *)(ai_Unkf8f39c + G19_E(e, 8) * 0x3c4) = 0x54;
        G19_E(e, 0) = d2_WinOpenPic(0x28, 0x11, *(int *)(*pp + 0x18c), d2_LastWindow);
        G19_R(G19_E(e, 0), 0x48, int) = 1;
        G19_E(e, 0x10) = d2_CreateWinText(0, 0, &ch_EmptyName, d2_LastWindow);
        G19_R(G19_E(e, 0x10), 0x74, short) = 0x54;
        G19_R(G19_E(e, 0x10), 0x14, short) = 0x80;
        G19_R(G19_E(e, 0x10), 0x5c, int) = 1;
        t = gm_Unk504e00();
        sv_Unk52eaa0(0x24, 0x41, (int)(double)t, 0x4df);
        x += 0x80;
        e += 0x20;
    }
    ai_Unk4cf9c0();
}
