/* 0x4d6ba0, the action of one fighter slot f (+0 id, +4 active, +0x10 target id, +0x14 / +0x18 the attacked place,
   +0x1d hit, +0x20 kill, +0x24 damage, +0x28 the target's new health): the target's health is set, and the attacker
   gets the strike animation / event 0x4d3550 (damage 1.4), for the kinds 0x174 / 0x176 the bomb events 0x4d3980 / 0x4d3a30
   (0x174 also picks the pot spot 0x4d2170 and sends 0x4c5bc0). The slot is cleared after a kill. Rough draft. */

void ai_Unk4d6ba0(char *f)
{
    char *h = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *c, *tgt, *k, *side, *x, *ev, *s, *d;
    int dmg, kill, ox, oy, mode;
    float py;

    if (*(int *)(f + 4) == 0 || *(int *)f == -1)
        return;
    c = ai_Unk4ce130(*(int *)f);
    k = ai_Unk4cda50(c);
    side = *(char **)(c + 0x16c);
    tgt = ai_Unk4ce130(*(int *)(f + 0x10));
    if (tgt != 0)
        *(int *)(tgt + 0x24) = *(int *)(f + 0x28);
    if (!(k != 0 && (*(int *)k == 0x174 || *(int *)k == 0x176))) {
        if (f[0x1d] != 0 && ((x = *(char **)(*(char **)(c + 0x184) + 0x128)) == 0 || x[9] == 0x2d || x[9] == 0x3a)) {
            if ((unsigned short)util_Random(10) > 8) {
                if (*(int *)(side + 4) == *(int *)(G20_DEF + 4))
                    snd_QueueVoice(G20_US(c), ai_Unk6f0d2c, "_K_C2C_ANGRIFF", -1, 500);
                else
                    snd_QueueVoice(G20_US(c), ai_Unk6f0d30, "_K_C2C_ANGRIFF", -1, 500);
            }
            kill = *(int *)(f + 0x20);
            dmg = *(int *)(f + 0x24);
            if (kill != 0)
                *(int *)(tgt + 0x24) -= dmg;
            if (k == 0)
                ch_QueueAnim(*(char **)(c + 0x184), "kampf/schlagen1", 1);
            else {
                if (*(int *)k == 0)
                    mode = kill != 0 ? 8 : 2;
                else
                    mode = kill != 0 ? 6 : 2;
                ev = (char *)ch_AddActionCBK(*(char **)(c + 0x184), (unsigned)&ai_Unk4d3550, mode, 0x2e, 1);
                *(float *)(ev + 0x178) = 1.4f;
                *(char **)(ev + 0x17c) = tgt;
                *(char **)(ev + 0x180) = c;
                *(int *)(ev + 0x184) = dmg;
                *(int *)(ev + 0x188) = *(int *)(side + 4);
                for (s = k + 0x11, d = ev + 0xf0; (*d++ = *s++) != 0;)
                    ;
                if ((*(int *)k == 0x16e || *(int *)k == 0x172) && kill != 0) {
                    *(int *)f = -1;
                    *(int *)(f + 4) = 0;
                }
            }
            ch_AddAction(*(char **)(c + 0x184), 0x3b, ai_Unk4e52f0(0x1e) + 0x8c);
        }
    }
    if (*(int *)k == 0x174 && *(int *)(f + 0x20) != 0) {
        ev = (char *)ch_AddActionCBK(*(char **)(c + 0x184), (unsigned)&ai_Unk4d3980, 6, 0x2e, 1);
        *(char **)(ev + 0x17c) = tgt;
        *(char **)(ev + 0x180) = c;
        *(int *)(ev + 0x184) = *(int *)(f + 0x14);
        *(int *)(ev + 0x188) = *(int *)(f + 0x18);
        strcpy(ev + 0xf0, "spezial/einhaendig_vom_boden_nehmen");
        if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c) {
            if (!d3_GetHeightAt(h, *(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c, &ai_Unkfa5f00, &ai_Unkfa5f04, &py))
                return;
            ai_Unk4d2170(c, ai_Unkfa5f00, ai_Unkfa5f04, 8, &ox, &oy);
            ai_Unk4c5bc0(*(char **)(*(char **)(c + 0x184) + 0x34), ox, oy, 0);
            *(int *)f = -1;
            *(int *)(f + 4) = 0;
        }
    }
    if (*(int *)k == 0x176 && f[0x1d] != 0) {
        ev = (char *)ch_AddActionCBK(*(char **)(c + 0x184), (unsigned)&ai_Unk4d3a30, 5, 0x2e, 1);
        *(char **)(ev + 0x17c) = c;
        *(int *)(ev + 0x180) = *(int *)(f + 0x14);
        *(int *)(ev + 0x184) = *(int *)(f + 0x18);
        *(int *)(ev + 0x188) = *(int *)(side + 4);
        for (s = k + 0x11, d = ev + 0xf0; (*d++ = *s++) != 0;)
            ;
        ch_AddAction(*(char **)(c + 0x184), 0x3b, ai_Unk4e52f0(0x1e) + 0x8c);
        sb_PlaySampleI(ai_Unk6f0d14, "topfwerfen_s", 1, 0x3f, 0x7f, 0, 0);
        *(int *)f = -1;
        *(int *)(f + 4) = 0;
    }
}
