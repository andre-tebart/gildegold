/* 0x489480, the same for a master with three skills */

int ai_Unk489480(AICHAR *ch)
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
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == c->bld && ai_CharsT[i].title &&
            (ai_CharsT[i].job == 0 || ai_CharsT[i].job->kind == 4 || ai_CharsT[i].job->kind == -0x74) && !(ai_CharsT[i].flags1c8 & 4) &&
            ((float)ai_CharsT[i].sk81 < 168.0f || (float)ai_CharsT[i].sk82 < 168.0f || (float)ai_CharsT[i].sk84 < 168.0f))
            found = 1;
        i++;
        if (found)
            break;
    }
    if (found && util_Random(0x80) < 0x60)
        return 0;
    if (!found) {
        if (((float)ai_CharsT[i].sk81 < 252.0f || (float)ai_CharsT[i].sk82 < 252.0f ||
             (float)ai_CharsT[i].sk84 < 252.0f) && util_Random(0x80) > 0x20)
            return 0;
    }
    for (i = 0; i < 768; i++) {
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == c->bld && ai_CharsT[i].title &&
            (ai_CharsT[i].job == 0 || ai_CharsT[i].job->kind == 4 || ai_CharsT[i].job->kind == -0x74) && !(ai_CharsT[i].flags1c8 & 4))
            ai_CharsT[i].flags1b4 |= 0x10;
    }
    return 1;
}
