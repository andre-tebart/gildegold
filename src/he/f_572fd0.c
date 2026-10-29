/* 0x572fd0, the run of the event 0x94 (an employee at work): -2 / -1 free. State 0: the character +0xac (0x505df0) and its
   building +0x16c; when the building word +0x27 and the character word compare as -0x1a or less (0x50c520): the group of
   the building +0x5d (0x4ffe90 (.., 2, 6, 0, 0x2a), else .. 0x116) and its members (0x4ffe90 (group +0x14, 1, 5), then
   0x4ffcc0), one picked at random (0x42ec00); then 0x5089c0 (word, 0x6f1bc5), 0x4c4880 (-1, group +2, word, 1, 0x6f1bc5, 0),
   the text 0x414b / 0x229 (0x5176d0) for the results 0 .. 2 of 0x5190a0 and the message 0x53dee0 (0x5f2, 0x629000, the
   building +1, the owner +2) to the player word of the event (+8); then free */

void he_Unk572fd0(int ev)
{
    char *per, *owner, *ch, *grp, *e;
    int bld, cnt;
    char *lst[0x20];
    char text[0x400];

    switch (HM_I(ev, 0x70)) {
    case -2:
    case -1:
        he_Unk53e1f0((int *)ev);
        return;
    case 0:
        break;
    default:
        return;
    }
    per = (char *)gm_FindChar(HM_I(ev, 0xac));
    if (per == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    bld = HM_I(per, 0x16c);
    if (bld == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (gm_Opinion(*(unsigned short *)(bld + 0x27), *(unsigned short *)per) > -0x1a) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    grp = (char *)gm_Query(HM_I(bld, 0x5d), 2, 6, 0, 0x2a);
    if (grp == 0)
        grp = (char *)gm_Query(HM_I(bld, 0x5d), 2, 6, 0, 0x116);
    owner = (char *)gm_BuildingMainObject(bld);
    if (grp == 0)
        goto free;
    cnt = 0;
    memset(lst, 0, sizeof lst);
    e = (char *)gm_Query(HM_I(grp, 0x14), 1, 5);
    while (e != 0) {
        lst[cnt++] = e;
        e = (char *)gm_QueryNext();
    }
    if (cnt == 0)
        goto free;
    ch = lst[util_Random(cnt) & 0xffff];
    if (ch == 0)
        goto free;
    ((float (*)(unsigned short, unsigned char))gm_GoodsPrice)(*(unsigned short *)ch, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    ((int (*)(int, int, unsigned short, int, unsigned char, int))cm_SendExSellObject)(-1, HM_I(grp, 2), *(unsigned short *)ch, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
    switch (ls_GetLanguage()) {
    case 0:
    case 2:
        txt_Format(text, 0x414b, 0x229, HM_I(per, 0x16c), *(unsigned short *)ch * 2 + 0x9a8);
        break;
    case 1:
        txt_Format(text, 0x414b, 0x229, HM_I(per, 0x16c), *(unsigned short *)ch * 2 + 0x9a8);
        break;
    }
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5f2, HM_I(bld, 1), HM_I(owner, 2), 0, (char *)(unsigned)&full_data_629000);
free:
    he_Unk53e1f0((int *)ev);
}
