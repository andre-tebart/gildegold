/* 0x56d610, the alloc of the research event 0x96: the character +0xac, its research record 0x4eae60 (the +0xb0 kind) and the amount 0x4eafd0 (character, record, &clock +0xbc) into +0xb4; above the constant 0x5eb788: +0xb8 = the clock +0xbc, the clocks +0x52 / +0xc0 = now, due 5, state 0, the building registered (0x4ffe90 / 0x524260); else the event frees itself */

union AHMGRID * ahm_FindRecipe();
float ahm_ResearchProgress();

void he_Unk56d610(int ev)
{
    char *c, *p;
    int v;

    c = (char *)gm_FindChar(*(int *)(ev + 0xac));
    if (c == 0)
        goto done;
    p = (char *)ahm_FindRecipe(*(int *)(ev + 0xb0));
    if (p == 0)
        goto done;
    *(float *)(ev + 0xb4) = ahm_ResearchProgress(c, p, (void *)(ev + 0xbc));
    if (*(float *)(ev + 0xb4) > *(float *)(unsigned)&ai_Unk5eb788)
        goto done;
    *(int *)(ev + 0xb8) = *(int *)(ev + 0xbc);
    *(HETIME *)(ev + 0xc0) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 5, 0);
    *(int *)(ev + 0x70) = 0;
    v = gm_Query(*(int *)(*(int *)(c + 0x16c) + 0x5d), 2, 6, 0, 0x24b, *(unsigned short *)c, ev);
    ((void (*)(int, int))sv_MoveToGroup)(*(int *)(c + 0x16c), v);
    snd_PlayFavourComment();
    return;
done:
    he_Unk53e1f0((int *)ev);
}
