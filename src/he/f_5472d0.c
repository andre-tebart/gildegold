/* 0x5472d0, a Ghidra miss (disassembled): the outcome of a duel (see 0x544840) lost by q: p (kind 6 / 7) hears the news 0x1d81 (0x1d68 + 1 when q is not of party 0) and both lose the flag 0x400 of +0x1c8; q gets the ability 0x4ea550 (its 0x4e6030 record or a random 1 .. 3, the amount -9 - 0x504ec0 (p, 4) * 8.0 - random(8)), the relation p -> q falls by 0x14, the event ev ends (ROUGH) */

void amt_ChangeDistrictOpinion();
int amt_FindOffice();
int ai_AmountRowStatus();
int ai_Unk5be720();
struct SBSAMPLE * sb_PlaySample();
int gm_FindById();
int cm_SendMessage3d(char *p, unsigned char b, int c, int d);
int gm_AddTime2();
int cm_SendLongMessage(char *msg);
int gm_CompareTime();
int d2_SetForm(int view, int n);
int io_PrintText(char *fmt, ...);

void he_Unk5472d0(int ev, int p, int q)
{
    char text[0x400];
    int a1[6];
    int a2[8];
    int bx = 0, amt;

    if (*(char *)(p + 2) == 6 || *(char *)(p + 2) == 7) {
        txt_Format(text, 0x1d81, *(unsigned short *)q, *(unsigned short *)q, (*(char *)(q + 9) != 0) + 0x1d68);
        he_ShowNews(*(int *)(p + 4), *(int *)(p + 4), text, 0, 0x5fb, (char *)0);
        cm_SendSetField(*(int *)(p + 4), 0x1c8, 4, 0, 0x400);
        cm_SendSetField(*(int *)(q + 4), 0x1c8, 4, 0, 0x400);
    }
    if (*(char *)(q + 0x166) != 0) {
        if (amt_FindOffice(q, a1, a2) != 0)
            bx = a1[1];
        else
            bx = (unsigned short)util_Random(3) + 1;
    }
    amt = -9 - (int)(gm_GetSkill(p, 4) * 8.0f);
    amt -= (unsigned short)util_Random(8);
    amt_ChangeDistrictOpinion(q, bx, amt);
    cm_SendRelations(*(int *)(p + 4), *(int *)(q + 4), -0x14, 1);
    *(HETIME *)(ev + 0x52) = ai_Time;
    cm_SendExSetHE(-1, ev);
}
