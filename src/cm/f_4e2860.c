/* 0x4e2860, the scene of a sale (the message m: +0x14 the object, +0x34 the seller, +0x38 .. +0x44 the buyers): the scene of the object
   (0x4fea50 -> 0x4e2910 with 0x17) or "ob_SALON.ed3", a pause of 5, then every buyer changes his opinion of the seller by +10 (0x4c4b00, both ways) */

void ai_Unk4e2860(char *m)
{
    int o, f, i;
    int *p;

    f = 0;
    ai_Unk4e3620(0);
    gm_FindById(&o, 0, 0, *(int *)(m + 0x14));
    if (o != 0)
        ai_Unk4e2910(o, 0x17);
    else {
        f = 1;
        ai_Unk4e2e60("ob_SALON.ed3");
    }
    ai_Unk4e3420(5);
    p = (int *)(m + 0x38);
    for (i = 0; i < 4; i++, p++) {
        if (*p != -1) {
            cm_SendRelations(*(int *)(m + 0x34), *p, 10, 1);
            cm_SendRelations(*p, *(int *)(m + 0x34), 10, 1);
        }
    }
    if (f != 0) {
        ai_Unk4e3060();
        ai_Unk4e30c0();
    }
    ai_Unk4e3640();
}
