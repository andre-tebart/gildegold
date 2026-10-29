/* 0x4e2dc0, 0x429e90 of every used slot (of 0x80 x 0xc98 bytes at 0x6332b4) whose +0x104 is -2 and +0x124 has bit 1 */

int ai_Unk4e2dc0(void)
{
    int i;
    char *e;

    for (i = 0; i < 0x80; i++) {
        e = ai_Unk6332b4 + i * 0xc98;
        if (*(int *)(e + 0x100) != -1 && (*(unsigned char *)(e + 0x124) & 1) && *(int *)(e + 0x104) == -2)
            evt_Stop((EVTSCRIPT *)e);
    }
    return 0;
}
