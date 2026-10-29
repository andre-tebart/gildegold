/* 0x543ac0, a Ghidra miss (disassembled): the master exam event of a building (the examiner of +0xbc: 0 = the building +0x10 with its guild master slot list +0x1c, 1 = the figure +0xc0 with the list +0xac of the event); the examinee is the figure (word +8). State 0 opens "Berufe\Messagebox_Meisterpruefung" (0x53d6f0) and the news 0x1770 / 0x1771; states 1 / 2 wait for the due time or the answer (0xf8a668: 0 = yes, 1 = no) and tell the examinee 0x1785 / 0x1786 (state 2 is the exit of a figure that is no master any more) (ROUGH) */

int ai_Unk494a30();
void he_Unk543ac0(int ev)
{
    int bld = 0;
    int e = 0;
    int o = 0;
    int cr, me, i, h, st;
    int neg1;
    char buf[0x1f0];

    st = *(int *)(ev + 0x70);
    if (st == -1 || st == -2)
        goto fail;
    if (*(int *)(ev + 0xbc) == 0) {
        gm_FindById(&o, 0, 0, *(int *)(ev + 0x10));
        if (o == 0)
            goto fail;
        bld = gm_Query(*(int *)(o + 0x5d), 1, 0, 0x17);
        if (bld == 0)
            goto fail;
        if (*(unsigned short *)(o + 0x27) == 0xffff)
            goto fail;
        cr = (unsigned)&ai_Chars + *(unsigned short *)(o + 0x27) * 0x21c;
        if (cr == 0)
            goto fail;
    } else if (*(int *)(ev + 0xbc) == 1) {
        e = (int)he_FindEvent(2, 1, *(int *)(ev + 0xc0), 0, 0x37);
        if (e == 0)
            goto fail;
        cr = (unsigned)&ai_Chars + *(unsigned short *)(e + 8) * 0x21c;
        if (cr == 0)
            goto fail;
    } else {
        goto done;
    }
    me = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
    if (*(char *)(me + 2) != 6 && *(char *)(me + 2) != 7)
        *(int *)(ev + 0x70) = 2;
    switch (*(int *)(ev + 0x70)) {
    case 0:
        he_Unk53d6f0(ev, 0, 0, (char *)"Berufe\Messagebox_Meisterpruefung", 0x5ec);
        if (*(int *)(ev + 0x74) == 0)
            goto done;
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 0);
        if (*(int *)(ev + 0xbc) == 0)
            io_PrintText(0x1770, *(unsigned short *)(o + 0x27), *(unsigned short *)me, ev + 0xac, o, *(unsigned short *)cr, *(unsigned short *)me);
        else
            io_PrintText(0x1771, *(unsigned short *)(o + 0x27), *(unsigned short *)me, ev + 0xac, *(unsigned short *)cr, *(unsigned short *)me);
        *(HETIME *)(ev + 0x44) = ai_Time;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        snd_PlayVoice(-7, 0, "_NACHRICHTEN_HS_28", 0);
        *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        return;
    case 1:
        if (gm_CompareTime((unsigned)&ai_Time, ev + 0x52) > 0) {
            he_Unk53d800(ev);
            *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
            return;
        }
        if (U_I((unsigned)&d2_Unkf8aac4) != *(int *)(*(int *)(ev + 0x74) + 8))
            return;
        neg1 = -1;
        h = U_I((unsigned)&d2_ClickedId);
        if (h == -1)
            return;
        break;
    case 2:
        h = ai_Unk494a30(cr, me);
        h = (h == 1) ? 0 : 1;
        neg1 = -1;
        break;
    default:
        return;
    }
    /* h: 0 = the examinee passed (0x1785), 1 = failed (0x1786) */
    i = 0;
    if (*(int *)(ev + 0xbc) == 0) {
        while (i < 4 && *(int *)(bld + 0x1c + i * 4) != *(int *)(me + 4))
            i++;
        if (i >= 4)
            goto fail;
        if (h != 0) {
            cm_BeginFieldBlock(bld, *(int *)(bld + 2));
            cm_AddFieldDelta(4, 1, i * 4 + 0x1c, &neg1);
            cm_SendFieldDeltas();
        }
    } else {
        while (i < 4 && *(int *)(ev + 0xac + i * 4) != *(int *)(me + 4))
            i++;
        if (i >= 4)
            goto fail;
        if (h != 0) {
            *(int *)(ev + 0xac + i * 4) = -1;
            cm_SendExSetHE(0, ev);
        }
    }
    txt_Format(buf, h == 0 ? 0x1785 : 0x1786, *(unsigned short *)me);
    he_ShowNews(*(int *)(cr + 4), -1, buf, 0, 0x5ec, (char *)0);
    if (st == 2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    he_Unk53d800(ev);
    he_Unk53e1f0((int *)ev);
    return;
fail:
    if (*(int *)(ev + 0x74) != 0)
        he_Unk53d800(ev);
done:
    he_Unk53e1f0((int *)ev);
}
