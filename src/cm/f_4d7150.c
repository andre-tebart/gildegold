/* 0x4d7150, the character of the record r is the winner so far (0x6f0cf4): text 0x11b7 */

void ai_Unk4d7150(char *r)
{
    char buf[0x100];
    char *c;

    c = gm_FindChar(*(int *)r);
    ai_Unk6f0cf4 = (int)c;
    txt_Format(buf, 0x11b7, G20_US(c));
    sv_SetStatusText(buf);
}
