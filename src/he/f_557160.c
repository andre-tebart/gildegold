/* 0x557160, the allocation handler of a player event: state 0, +0xec = 0; a figure that is not a player character (kind 6) frees the record at once */

void he_Unk557160(int ev)
{
    *(int *)(ev + 0x70) = 0;
    *(int *)(ev + 0xec) = 0;
    if (*(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(ev + 8) * 0x21c) != 6)
        he_Unk53e1f0((int *)ev);
}
