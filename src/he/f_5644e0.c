/* 0x5644e0, the allocation handler of the fire event (sounds "Brand_Beginn" / "Brand_Ende" 0x4c27d0; ROUGH): the burning building and a second one (0x458a20 positions), a message record (0x4c4b70), the other fires of the type list (0x53e400 / 0x53e300) are checked and the due clock is now + 0x14 or 7 units by the minute word (0x42ec00 rolls) */

void he_Unk5644e0(int ev)
{
    char *e;
    int b;
    unsigned char m[0xf8];

    snd_QueueVoice(-7, 0, "Brand_Beginn", 0, 0);
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    (void)b;
    memset(m, 0, sizeof(m));
    cm_SendLongMessage(m);
    for (e = he_FindEvent(1, 0, 0x3d); e != 0; e = he_NextEvent()) {
        if ((int)e != ev)
            break;
    }
    snd_QueueVoice(-7, 0, "Brand_Ende", 0, 0);
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, *(unsigned short *)(ev + 0x56) >= 0x14 ? (unsigned short)util_Random(5) : 7, 0);
}
