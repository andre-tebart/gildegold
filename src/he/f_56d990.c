/* 0x56d990, the alloc of the research event 0x97: like 0x56d610 but the amount is 0x4eb0b0 (character, the +0xb0 kind, &clock +0xbc), no separate record; the clock +0xc0 and the registration as there */

float ahm_ShareProgress();
float ahm_Share();
int ai_Unk5a9b50();
int ahm_SendSetShare();  /* @original: he.c's call at 14930 passes 2 arguments to ahm.h's 3-parameter prototype, so this TU was compiled against the non-prototype declaration */

void he_Unk56d990(int ev)
{
    char *c;
    int v;

    c = (char *)gm_FindChar(*(int *)(ev + 0xac));
    if (c == 0)
        goto done;
    *(float *)(ev + 0xb4) = ahm_ShareProgress(c, *(int *)(ev + 0xb0), (void *)(ev + 0xbc));
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
