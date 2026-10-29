/* 0x5740f0, 1 when an event of the type 0x9f of the player word of `a` exists (0x53e400 (2, 0, 0x9f, 2, word)) */

int ai_Unk5740f0(char *a)
{
    return he_FindEvent(2, 0, 0x9f, 2, *(unsigned short *)a) != 0;
}
