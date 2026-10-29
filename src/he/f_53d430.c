/* 0x53d430, the allocation handler of an event of an unknown type (debug text only) */

void he_Unk53d430(unsigned char *ev)
{
    char buf[256];

    sprintf(buf, "he_AllocNone(): Invalid HE-Type : %i, von Spieler %s", *ev, (char *)(((unsigned)&ai_Unk770b89 + 0x27) + *(unsigned short *)(ev + 8) * 0x21c));
}
