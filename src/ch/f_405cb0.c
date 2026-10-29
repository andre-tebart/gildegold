/* 0x405cb0 */

CHACTION *ch_UseGate(CHARACTER *ch, char *name, char *gate, int room, int unk174, int unk178)
{
    CHACTION *a = ch_Unk40b270(ch);

    if (!a)
        return 0;
    a->func = (CHACTFUNC)ch_ActUsingGate;
    a->type = 0x34;
    strcpy(CH_ACTOBJ(a)->name, name);
    strcpy(CH_ACTOBJ(a)->name2, gate);
    CH_ACTOBJ(a)->unk170 = room;
    CH_ACTOBJ(a)->unk174 = unk174;
    if (CH_ACTOBJ(a)->unk178 == -1)
        err_Message("ch_UseGate: room_id == ID_NONE");
    CH_ACTOBJ(a)->unk178 = unk178;
    return a;
}
