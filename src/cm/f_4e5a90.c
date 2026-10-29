/* 0x4e5a90, the four names (0x143ab0c) of the bytes +0x166 .. +0x169 of a person */

void ai_Unk4e5a90(char *c, char *buf)
{
    sprintf(buf, "%s\t%s\t%s\t%s\t", ai_Unk143ab0c[(unsigned char)c[0x166]], ai_Unk143ab0c[(unsigned char)c[0x167]], ai_Unk143ab0c[(unsigned char)c[0x168]],
            ai_Unk143ab0c[(unsigned char)c[0x169]]);
}
