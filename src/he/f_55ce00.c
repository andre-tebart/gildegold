/* 0x55ce00, the allocation handler of the master exam passed event: the record of type 0x25 of the same player (0x53e400 (2, 2, id, 0, 0x25)) is ended (state -1), the due clock is now */

void he_Unk55ce00(int ev)
{
    char *e;

    gm_FindChar(*(int *)(ev + 0xac));
    e = he_FindEvent(2, 2, *(unsigned short *)(ev + 8), 0, 0x25);
    if (e != 0)
        *(int *)(e + 0x70) = -1;
    *(HETIME *)(ev + 0x52) = ai_Time;
}
