/* 0x525940, copies the sky colour table (0x623cc0, or 0x623e10 when raining) into the sky object records (+0x128 .. +0x130 of the object 0x5fb5b0, 6 rows of 0x60) and the globals 0x5fcc78 .. / 0xfba9c0 ..; only
   when the object has its mesh (+0xac) */

void sv_UpdateSkyColors(void)
{
    int *p;
    int i;

    if (*(int *)(ai_Unk5fb5b0 + 0xac) == 0)
        return;
    d3_ForEachObject(ai_Unk5fb5b0, 0, 8, sv_Unk525490, 0);
    p = ai_Unk623cc0;
    if (ai_Unk760610 != 0)
        p = ai_Unk623e10;
    p += 2;
    i = 0;
    do {
        *(int *)(ai_Unk5fb5b0 + i + 0x128) = p[-2];
        *(int *)(ai_Unk5fb5b0 + i + 300) = p[-1];
        *(int *)(ai_Unk5fb5b0 + i + 0x130) = p[0];
        ai_Unk5fcc78 = p[-2];
        ai_Unk5fcc7c = p[-1];
        ai_Unk5fcc80 = p[0];
        *(int *)((char *)ai_Unkfba9c0 + i) = p[-2];
        *(int *)((char *)ai_Unkfba9c0 + i + 4) = p[-1];
        *(int *)((char *)ai_Unkfba9c0 + i + 8) = p[0];
        i += 0x60;
        p += 4;
    } while (i < 0x2a0);
}
