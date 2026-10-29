/* 0x548f30, the run handler of a one-shot event (like 0x548230 without the pending check) */

void he_Unk548f30(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st >= -2) {
        if (st < 0) {
            he_Unk53e1f0((int *)ev);
        } else if (st == 0 && (*(unsigned char *)(ev + 0x78) & 2) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        }
    }
}
