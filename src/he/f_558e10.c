/* 0x558e10, a run handler: state 3 posts the event 1 later, state 9 frees the record */

void he_Unk558e10(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st == 9) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 3) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
    }
}
