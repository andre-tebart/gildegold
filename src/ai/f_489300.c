/* 0x489300, a thief master: are there free characters, and what do they know */

int ai_CheckFreeThieves(AICHAR *ch)
{
    AICHAR *c = (AICHAR *)ch;
    int i, found;

    if (c->flags1c8 & 4)
        return 0;
    found = 0;
    c->flags1c8 |= 4;
    i = 0;
    while (1) {
        if (i >= 768)
            break;
        if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == c->bld && ai_Chars[i].title && ai_Chars[i].job == 0 && !(ai_Chars[i].flags1c8 & 4) &&
            ((float)ai_Chars[i].sk83 < 168.0f || (float)ai_Chars[i].sk82 < 168.0f))
            found = 1;
        i++;
        if (found)
            break;
    }
    if (found && util_Random(0x80) < 0x20)
        return 0;
    if (!found) {
        if (((float)ai_Chars[i].sk83 < 252.0f || (float)ai_Chars[i].sk82 < 252.0f) && util_Random(0x80) > 0x60)
            return 0;
    }
    for (i = 0; i < 768; i++) {
        if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == c->bld && ai_Chars[i].title && ai_Chars[i].job == 0 && !(ai_Chars[i].flags1c8 & 4))
            ai_Chars[i].flags1b4 |= 0x10;
    }
    return 1;
}
