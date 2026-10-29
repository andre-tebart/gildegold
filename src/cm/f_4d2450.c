/* 0x4d2450, a callback of the object walk: collects the objects named "sp_ESCAPE" into list[ai_Unk6f0d60++] */

char ai_Unk4d2450(char *o, int *list)
{
    if (_stricmp(o, "sp_ESCAPE") == 0) {
        list[ai_Unk6f0d60] = (int)o;
        ai_Unk6f0d60++;
    }
    return 1;
}
