/* 0x4debe0, announces the result of a duel (the message m: +0x34 / +0x38 the duellists, +0xc the owner): the winner is the one with the
   better 0x504ec0(.., 3); when he is a person of the player's side the text 0x10c7 (+ 0x10c8 for a woman) goes to every character of the
   kinds 6 / 7 (0x53dda0) */

void ai_Unk4debe0(char *m)
{
    char buf[0x400];
    char *a, *b, *w;
    float fa;
    int ids[2];
    int i;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a == 0 || b == 0)
        return;
    fa = (float)gm_GetSkill(a, 3);
    w = b;
    if (gm_GetSkill(b, 3) < fa)
        w = a;
    if (w[0x166] == 0)
        return;
    if (amt_FindOffice(w, buf + 0x18, ids) == 0)
        return;
    if (*(int *)(m + 0xc) != ai_CharInts[ai_PlayerId][0])
        return;
    amt_SendOfficeMessage(ids[0], 0, 0, 4, 0xff);
    for (i = 0; i < 768; i++) {
        if (ai_Chars[i][2] == 6 || ai_Chars[i][2] == 7) {
            txt_Format(buf, 0x10c7, *(unsigned short *)w, (w[9] != 0) + 0x10c8);
            he_ShowNews(*(int *)(ai_Chars[i] + 4), -1, buf, 0, 0x5fe, &ch_EmptyName);
        }
    }
}
