/* 0x4de900, the intro text of the duel round of the fighter a against b ("_DUELL_INTRO_C2_AUSWAHL_*": kind of the round at +0x94 of the
   record 0x4e50e0: 2 insult (laestern: the better insult value 0x504ec0(.., 4) + a roll wins, flag 0x6f0daa / 0x6f0dab), 3 a roll against
   0x504ec0(a, 2) (flag 0x6f0dac / 0x6f0dad), 4 the pistol shot (an object with the callback 0x4de640, a / b in +0x17c / +0x180)). ROUGH */

void ai_Unk4de900(char *a, char *b)
{
    char *rec;
    int n;
    double fa, fb, ra, rb;

    if (ai_Unk6f0d94 != 0)
        return;
    rec = (char *)ai_Unk4e50e0(*(int *)(a + 4));
    d2_SetForm(ai_Unk60d368, 0);
    io_PrintText("$C");
    if (rec[0x94] == 2) {
        d2_SetForm(ai_Unk60d368, 0);
        io_PrintText(0x10e8, *(unsigned short *)a);
        n = ai_Unk4de610(8);
        io_PrintText(n + 0x10e9);
        snd_PlayVoice(*(unsigned short *)a, 0, "_DUELL_INTRO_C2_AUSWAHL_LAESTERN", n);
        io_PrintText("$N");
        ch_QueueAnim(*(char **)(a + 0x184), "gestik/abfaellig", 1);
        ai_Unk4e3420(2);
        fa = gm_GetSkill(a, 4);
        fb = gm_GetSkill(b, 4);
        ra = ai_Unk4e5360();
        rb = ai_Unk4e5360();
        d2_SetForm(ai_Unk60d368, 0);
        if ((float)(ra + (float)fa) <= rb + (float)fb) {
            if (b[2] == 6 || b[2] == 7)
                io_PrintText(0x10f4, *(unsigned short *)b);
            else
                io_PrintText(0x10f5, *(unsigned short *)b);
        } else {
            if (b[2] == 6 || b[2] == 7)
                io_PrintText(0x10f2, *(unsigned short *)b);
            else
                io_PrintText(0x10f3, *(unsigned short *)b);
            if ((int)a == ai_Unk92bca4)
                ai_Unk6f0da8[2] = 1;
            else
                ai_Unk6f0da8[3] = 1;
        }
    } else if (rec[0x94] == 3) {
        d2_SetForm(ai_Unk60d368, 0);
        fa = gm_GetSkill(a, 2);
        fb = ai_Unk4e5360();
        if ((float)fa <= fb) {
            if (a[2] == 6 || a[2] == 7)
                io_PrintText(0x10f8, *(unsigned short *)a);
            else
                io_PrintText(0x10f9, *(unsigned short *)a);
        } else {
            if (a[2] == 6 || a[2] == 7)
                io_PrintText(0x10f6, *(unsigned short *)a);
            else
                io_PrintText(0x10f7, *(unsigned short *)a);
            if ((int)a == ai_Unk92bca4)
                ai_Unk6f0da8[4] = 1;
            else
                ai_Unk6f0da8[5] = 1;
        }
    } else if (rec[0x94] == 4) {
        d2_SetForm(ai_Unk60d368, 0);
        n = ai_Unk4de610(7);
        io_PrintText(n + 0x10fa, *(unsigned short *)a);
        snd_PlayVoice(*(unsigned short *)a, 0, "_DUELL_INTRO_C2_AUSWAHL_SCHIESSEN", n);
        ai_Unk4e3420(2);
        io_PrintText("$N");
        n = ch_AddActionCB(*(char **)(a + 0x184), ai_Unk4de640, 6, 0x2e, 1);
        *(char **)(n + 0x17c) = b;
        *(char **)(n + 0x180) = a;
        strcpy((char *)n + 0xf0, "kampf/schiessen_pistole");
        ai_Unk4e3420(4);
        return;
    } else {
        ai_Unk4e3420(4);
        return;
    }
    io_PrintText("$N");
    ai_Unk4e3420(4);
}
