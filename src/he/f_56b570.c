/* 0x56b570, "he_AllocGebaeudeBauen", the alloc of an event of the type 0xd (when another one of the type is still below the state 5 the record is freed): clocks (+0x52 / +0x60) = the start clock, due now + 1, +0xc4 / +0xc8 = 0;
   the data of the object +0xb0 is looked up (0x58f640 / 0x58f7a0 in the scene 0, 0x4478e0 selects it) and every entry is checked (0x58fe10 against +0xd4); without entries or with a miss the record is freed,
   else the error text is formatted and +0x84 = -1 (ROUGH) */

int ai_BuildingName();
int ai_FindBauplaetze();
int ai_FindNearestObject(float *ref, int *list);  /* 0x58fe10 (unk_58bbd0.c) */

void he_Unk56b570(int ev)
{
    char buf[0x10];
    char tab[0x400];
    int e2, n, i, scene;
    int a[2];

    for (e2 = (int)he_FindEvent(1, 0, 0xd); e2 != 0; e2 = he_NextEvent()) {
        if (e2 != ev && *(int *)(e2 + 0x70) < 5)
            goto done;
    }
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    *(HETIME *)(ev + 0x60) = *(HETIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 0, 1, 0);
    *(int *)(ev + 0xc4) = 0;
    *(int *)(ev + 0xc8) = 0;
    if (ai_BuildingName(*(int *)(ev + 0xb0), a) == 0)
        goto done;
    scene = U_I((unsigned)&d3_CurrentScene);
    memset(tab, 0, sizeof(tab));
    d3if_SelectScene(0, 1);
    n = ai_FindBauplaetze(a, tab + 8);
    if (n == 0) {
        d3if_SelectScene(scene, 1);
        goto done;
    }
    for (i = 0; i < n; i++) {
        if (ai_FindNearestObject(ev + 0xd4, tab) == 0) {
            he_Unk53e1f0((int *)ev);
            d3if_SelectScene(scene, 1);
            return;
        }
    }
    txt_Format(buf, "he_AllocGebaeudeBauen: %1N1", *(unsigned short *)(ev + 8));
    d3if_SelectScene(scene, 1);
    *(int *)(ev + 0x84) = -1;
    return;
done:
    he_Unk53e1f0((int *)ev);
}
