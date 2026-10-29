/* 0x5432f0, a Ghidra miss (disassembled): the master exam ("Meisterpruefung") event of the three figures +0xac / +0xb0 / +0xb4 (p, q, r): state 0 opens the window "Berufe\Messagebox_Meisterpruefung" (0x53d6f0, 0x5ec) and the news 0x1763; state 1 waits for the due time or the answer (0x6f8a668: 0 = yes, 1 = no, the click must be on the window), state 2 settles it with the news 0x1764 / 0x1765 and the relation changes (-0x14 / -0xf) (ROUGH) */

int io_PrintText(char *fmt, ...);
int snd_PlayVoice(int id, int bank, char *text, int var);
float gm_Mix(int a, int b, int c);

void he_Unk5432f0(int ev)
{
    int p, q, r, st;
    float f;
    char buf[0x100];
    int h;

    p = gm_FindChar(*(int *)(ev + 0xac));
    q = gm_FindChar(*(int *)(ev + 0xb0));
    r = gm_FindChar(*(int *)(ev + 0xb4));
    st = *(int *)(ev + 0x70);
    if (st == -2 || p == 0 || q == 0 || r == 0) {
        he_Unk53e1f0((int *)ev);
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        return;
    }
    if (st == -1 || (*(char *)(r + 2) != 6 && *(char *)(r + 2) != 7))
        *(int *)(ev + 0x70) = 2;
    st = *(int *)(ev + 0x70);
    if (st == 0) {
        he_Unk53d6f0(ev, 0, 0, (char *)"Berufe\Messagebox_Meisterpruefung", 0x5ec);
        if (*(int *)(ev + 0x74) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 0);
        io_PrintText(0x1763, *(unsigned short *)q, *(unsigned short *)p);
        *(HETIME *)(ev + 0x44) = ai_Time;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        snd_PlayVoice(-7, 0, "_NACHRICHTEN_HS_20", 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    }
    if (st == 1) {
        if (gm_CompareTime((unsigned)&ai_Time, ev + 0x52) > 0) {
            he_Unk53d800(ev);
            *(int *)(ev + 0x70) = 2;
            return;
        }
        if (U_I((unsigned)&d2_Unkf8aac4) != *(int *)(*(int *)(ev + 0x74) + 8))
            return;
        h = U_I((unsigned)&d2_ClickedId);
        if (h == -1)
            return;
        if (h == 0) {
            txt_Format(buf, 0x1764, *(unsigned short *)r, *(unsigned short *)p);
            he_ShowNews(*(int *)(q + 4), *(int *)(q + 4), buf, 0, 0x5fa, (char *)0);
            cm_SendRelations(*(int *)(p + 4), *(int *)(r + 4), -20, 1);
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (h == 1) {
            txt_Format(buf, 0x1765, *(unsigned short *)r, *(unsigned short *)p);
            he_ShowNews(*(int *)(q + 4), *(int *)(q + 4), buf, 0, 0x5fa, (char *)0);
            cm_SendRelations(*(int *)(q + 4), *(int *)(r + 4), -15, 1);
            cm_SendRelations(*(int *)(r + 4), *(int *)(q + 4), -15, 1);
        }
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 2) {
        f = gm_Mix(q, p, 4);
        if (!(util_RandomF() + f < *(float *)(unsigned)&ai_Unk5eb780)) {
            txt_Format(buf, 0x1764, *(unsigned short *)r, *(unsigned short *)p);
            he_ShowNews(*(int *)(q + 4), *(int *)(q + 4), buf, 0, 0x5fa, (char *)0);
            cm_SendRelations(*(int *)(p + 4), *(int *)(r + 4), -20, 1);
        } else {
            txt_Format(buf, 0x1765, *(unsigned short *)r, *(unsigned short *)p);
            he_ShowNews(*(int *)(q + 4), *(int *)(q + 4), buf, 0, 0x5fa, (char *)0);
            cm_SendRelations(*(int *)(q + 4), *(int *)(r + 4), -15, 1);
            cm_SendRelations(*(int *)(r + 4), *(int *)(q + 4), -15, 1);
        }
        he_Unk53e1f0((int *)ev);
    }
}
