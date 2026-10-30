/* 0x54da80, a Ghidra miss (disassembled; 960 B): the run handler of the talent teaching event (a 3D "DrBh" object: magic 0x68427244): state 0 draws the talent (0x4ac150 / 0x4ac170), the due clock 3 + random(4) ahead; state 1 shows the news 0x1e29 to every player figure; state 2 creates the object (0x4287b0 "0x626e8c" + 0x429ab0, the model of the talent record 0x4ac1e0) and waits (+0x56 >= 0x14 frees it with 0x429e90), clears the flag 0x20000 of the own player; state 3 clears the flag 0x20000 (+0x1c8) of everybody and ends (ROUGH) */

void he_Unk54da80(int ev)
{
    char text[0x200];
    int st, rec, a, b, obj, tal;

    st = *(int *)(ev + 0x70);
    if (st == -1 || st == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(int *)(ev + 0x84) = -1;
    switch (st) {
    case 0:
        if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
            return;
        a = hmt_RandomRecord(*(unsigned short *)(ev + 0xac));
        *(unsigned short *)(ev + 0xac) = (unsigned short)a;
        *(unsigned short *)(ev + 0xae) = (unsigned short)hmt_RandomTextOffset(a);
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + (unsigned short)util_Random(4) + 3;
        *(short *)(ev + 0x56) = 0xa;
        *(int *)(ev + 0x58) = 0;
        cm_SendExSetHE(1, ev);
        *(int *)(ev + 0x84) = -1;
        return;
    case 1:
        if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
            return;
        txt_Format(text, 0x1e29);
        for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_CharsV + 0x65400); rec += 0x21c) {
            if (*(unsigned short *)rec != 0xffff && (*(char *)(rec + 2) == 6 || *(char *)(rec + 2) == 7))
                he_ShowNews(*(int *)(rec + 4), *(int *)(ev + 4), text, 0, 0x601, "");
        }
        gm_AddTime(ev + 0x52, 0, 5, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
        return;
    case 2:
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 5, 0);
        obj = (int)evt_FindById(*(int *)(ev + 0xb4));
        if (*(unsigned short *)(ev + 0x56) >= 0x14) {
            if (obj != 0 && *(int *)obj == 0x68427244)
                evt_Stop((EVTSCRIPT *)obj);
            gm_AddTime(ev + 0x52, 0, 5, 0);
            *(int *)(ev + 0x70) = 3;
            return;
        }
        if (obj == 0 || *(int *)obj != 0x68427244) {
            tal = hmt_GetRecord(*(unsigned short *)(ev + 0xac));
            *(int *)(ev + 0xb4) = -1;
            sprintf(text, (char *)(unsigned)"%i", *(int *)(tal + 8));
            b = U_I((unsigned)&d3_CurrentScene);
            d3if_SelectScene(0, 1);
            obj = evt_Load("DrB");
            if (obj != 0) {
                *(int *)(ev + 0xb4) = *(int *)(obj + 0x100);
                *(int *)obj = 0x68427244;
                evt_RunScriptArgs((EVTSCRIPT *)obj, 1, *(int *)(tal + 8));
                *(int *)(obj + 0x104) = 0;
            }
            d3if_SelectScene(b, 1);
        }
        *(int *)(ev + 0x70) = 2;
        if (*(int *)(ev + 0x88) != 0 && (U_I(((unsigned)&ai_Selected + 0x40) + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c) & 0x20000) != 0) {
            he_Unk53e840(*(int *)(ev + 0x88));
            *(int *)(ev + 0x88) = 0;
        }
        return;
    case 3:
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
            for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_CharsV + 0x65400); rec += 0x21c) {
                if (*(unsigned short *)rec != 0xffff && (U_I(rec + 0x1c8) & 0x20000) != 0)
                    cm_SendSetField(*(int *)(rec + 4), 0x1c8, 4, 0, 0x20000);
            }
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
        *(short *)(ev + 0x56) = 6;
        *(int *)(ev + 0x58) = 0;
        if (*(int *)(ev + 0x88) != 0) {
            he_Unk53e840(*(int *)(ev + 0x88));
            *(int *)(ev + 0x88) = 0;
        }
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0)
            *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
        *(int *)(ev + 0x84) = -1;
        return;
    default:
        return;
    }
}
