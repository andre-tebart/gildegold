/* 0x568aa0, calls the handler 0x541200 for every event of the type 0x23 with the flags +0x78 & 3 whose +0xb0 equals the day +4 of this event */

void he_Unk568aa0(int ev)
{
    int o;

    for (o = (int)he_FindEvent(1, 0, 0x23); o != 0; o = (int)he_NextEvent()) {
        if ((*(unsigned char *)(o + 0x78) & 3) != 0 && *(int *)(o + 0xb0) == *(int *)(ev + 4))
            he_Unk541200(o);
    }
}
