/* 0x552920, the run handler of the two-person event (ROUGH): states -1 / -2 free the record, flag 4 or a pending handle (+0x84) does nothing. State 0: with +0xb0 set 0x4e78a0 picks a person +0xac (none: the record is reset like 0x552880 with the minute word 0x11 and 0x4c4c60 (0, ev)), 0x4e7aa0 picks the two partners +0xb4 and (one time in 4 less) +0xb8 and the event is posted 1 later; state 1 checks both (0x4c3ae0: 1 = arrived: 0x4e7b60 (the person of the handle 0x4c3b10, +0xac), 2 = gone) and posts 5 later when both are done */

int cm_GetSlotValue(int h);
int amt_InitApplicant();
int amt_GetOpenOffice();
int amt_CreateApplicant();

void he_Unk552920(int ev)
{
    int st = *(int *)(ev + 0x70);
    int n, r;

    if (st == -1 || st == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(int *)(ev + 0x84) = -1;
    if (st == 0) {
        if (*(int *)(ev + 0xb0) == 0) {
            goto reset;
        }
        *(int *)(ev + 0xac) = amt_GetOpenOffice(*(int *)(ev + 0xb0));
        if (*(int *)(ev + 0xac) == 0) {
reset:
            gm_AddTime(ev + 0x52, 0x18, 0, 0);
            *(unsigned short *)(ev + 0x56) = 0x11;
            *(int *)(ev + 0x58) = 0;
            *(int *)(ev + 0xac) = 0;
            *(int *)(ev + 0xb0) = 0x1b;
            *(int *)(ev + 0xb4) = -1;
            *(int *)(ev + 0xb8) = -1;
            *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
            return;
        }
        *(int *)(ev + 0xb4) = amt_CreateApplicant(*(int *)(ev + 0xac));
        if ((unsigned short)util_Random(3) != 0)
            *(int *)(ev + 0xb8) = amt_CreateApplicant(*(int *)(ev + 0xac));
        else
            *(int *)(ev + 0xb8) = -1;
    } else if (st == 1) {
        n = 0;
        if (*(int *)(ev + 0xb4) == -1) {
            n = 1;
        } else {
            r = cm_GetSlotState(*(int *)(ev + 0xb4));
            if (r == 1) {
                n = 1;
                amt_InitApplicant(cm_GetSlotValue(*(int *)(ev + 0xb4)), *(int *)(ev + 0xac));
                *(int *)(ev + 0xb4) = -1;
            } else if (r == 2) {
                *(int *)(ev + 0xb4) = -1;
                n = 1;
            }
        }
        if (*(int *)(ev + 0xb8) == -1) {
            n++;
        } else {
            r = cm_GetSlotState(*(int *)(ev + 0xb8));
            if (r == 1) {
                n++;
                amt_InitApplicant(cm_GetSlotValue(*(int *)(ev + 0xb8)), *(int *)(ev + 0xac));
                *(int *)(ev + 0xb8) = -1;
            } else if (r == 2) {
                *(int *)(ev + 0xb8) = -1;
                n++;
            }
        }
        if (n == 2) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 5, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
            return;
        }
    } else {
        return;
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
}
