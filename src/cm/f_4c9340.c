/* 0x4c9340, handler of type 0x28: allocates a cutscene ("cm_ExAllocCutscene") */

int ai_Unk4c9340(G17MSG *m, G17STAT *s)
{
    char buf[0x114];
    char *o;

    memcpy(buf, ai_Unka68d40, 0x114);
    o = ai_Unk4e5150(buf);
    if (o == 0) {
        err_Message("cm_ExAllocCutscene(): Cutscene-Queue is full!");
        return 1;
    }
    if (ai_GameFlags & 4)
        o[0x26] |= 0x80;
    if (s != 0) {
        s->state = 1;
        s->unk01 = 9;
        s->value = (int)o;
    }
    return 0;
}
