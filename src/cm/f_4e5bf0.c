/* 0x4e5bf0, the debug dump of a person (one line made by 0x4e58e0, 0x4e5980, 0x4e5a40, 0x4e5a90, 0x4e5af0 and 0x4e5b50 in a 0x1fb8 byte buffer; the end of the cut_ code) */

void ai_Unk4e5bf0(char *c)
{
    char buf[0x1fb8];

    if (c != 0 && *(unsigned short *)c != 0xffff) {
        memset(buf, 0, sizeof(buf));
        ai_Unk4e58e0(c, buf + strlen(buf));
        ai_Unk4e5980(c, buf + strlen(buf));
        ai_Unk4e5a40(c, buf + strlen(buf));
        ai_Unk4e5a90(c, buf + strlen(buf));
        ai_Unk4e5af0(c, buf + strlen(buf));
        ai_Unk4e5b50(c, buf + strlen(buf));
    }
}
