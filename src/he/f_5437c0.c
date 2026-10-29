/* 0x5437c0, the run handler of the event "party / cart announcement": after it was posted (state +0x70 = 0) it collects the up to 4 member figures (the building record 0x17 at +0x1c .. or the event's +0xac .. list), asks 0x494ac0 whether the group may start and, if not, tells the player the news 0x1787 followed by one line 0x1788 per member (0x178f + 3 * x + random 3); state 1 only re-posts (ROUGH) */

int gm_FindById();
int ai_Unk494ac0();

void he_Unk5437c0(int ev)
{
    int bld = 0;
    int rec = 0;
    int i, k, cnt;
    int *pm;
    int *pe;
    unsigned short us;
    unsigned u;
    int grp[15];
    int mem[8];
    char big[2048];
    char line[256];

    if (*(int *)(ev + 0x70) == -2 || *(int *)(ev + 0x70) == -1) {
        he_Unk53e1f0((int *)ev);
    } else if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        gm_FindById(&bld, 0, 0, *(int *)(ev + 0x10));
        if (bld != 0 && (rec = gm_Query(*(int *)(bld + 0x5d), 1, 0, 0x17)) == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            cm_SendExSetHE(-1, ev);
            return;
        }
        if (*(int *)(ev + 0x70) == 0) {
            memset(grp, 0, sizeof(grp));
            grp[13] = 4;
            grp[14] = (int)grp;
            cnt = 0;
            grp[0] = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
            if (rec == 0) {
                grp[7] = *(unsigned char *)(ev + 0xbd);
                grp[8] = *(unsigned char *)(ev + 0xbc);
                pm = (int *)0x1c;
                pe = (int *)(ev + 0xac);
                do {
                    if (*pm != -1 && (i = gm_FindChar(*pe)) != 0) {
                        grp[1 + cnt * 2] = i;
                        cnt++;
                    }
                    pm++;
                    pe++;
                } while ((int)pm < 0x2c);
            } else {
                grp[7] = *(unsigned char *)(rec + 0x2c);
                grp[8] = *(unsigned char *)(rec + 0x2d);
                pm = (int *)(rec + 0x1c);
                for (k = 4; k != 0; k--, pm++) {
                    if (*pm != -1 && (i = gm_FindChar(*pm)) != 0) {
                        grp[1 + cnt * 2] = i;
                        cnt++;
                    }
                }
            }
            *(unsigned char *)(grp + 12) = (unsigned char)cnt;
            i = ai_Unk494ac0(grp, 1);
            if (i == 0 && (*(char *)(grp[0] + 2) == 6 || *(char *)(grp[0] + 2) == 7)) {
                txt_Format(big, 0x1787, grp[11]);
                i = 0;
                if ((char)grp[12] != 0) {
                    do {
                        us = util_Random(3);
                        txt_Format(line, 0x1788, *(unsigned short *)grp[1 + i * 2], us + 0x178f + grp[2 + i * 2] * 3);
                        strcat(big, line);
                        i++;
                        u = grp[12] & 0xff;
                    } while (i < (int)u);
                }
                he_ShowNews(*(int *)(grp[0] + 4), -1, big, 0x20, 0x5fd, (char *)(unsigned)&ch_EmptyName);
            }
            *(HETIME *)(ev + 0x52) = ai_Time;
            cm_SendExSetHE(-1, ev);
            return;
        }
        if (*(int *)(ev + 0x70) == 1) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            cm_SendExSetHE(-1, ev);
            return;
        }
    }
}
